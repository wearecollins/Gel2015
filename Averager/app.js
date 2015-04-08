/******************************************************************
	SPACEBREW
******************************************************************/

var Spacebrew = require("./Spacebrew.js").Spacebrew;

var spacebrewLocal, spacebrewRemote;

var spacebrewHost = "localhost";
// var remoteAddress = "spacebrew.robotconscience.com";

/******************************************************************
	AVERAGING
******************************************************************/

var MODES = [ "MODE_AVERAGE", "MODE_RANDOM_LEAD", "MODE_LIVE"];
var mode = "MODE_AVERAGE";
var messageTimeoutSeconds = .2;

/******************************************************************
	STORAGE
******************************************************************/

var messages = {};

function Message( ID, dir ){
	this.time 		= Date.now();
	this.id   		= ID;
	this.direction 	= dir;
}

// running value
var currentValue = -1;

/******************************************************************
	APP
******************************************************************/

function main(){
	console.log( "CONNECTING TO "+ spacebrewHost);
	// setup connection: Local
	spacebrewLocal = new Spacebrew.Client(spacebrewHost, "averager", "");

	// pad== "dir:id", dir == 0-4 (U,R,D,L, Look)
	spacebrewLocal.addSubscribe("touch", "pad");
	spacebrewLocal.addPublish("average", "pad");
	spacebrewLocal.connect();

	spacebrewLocal.onCustomMessage = onCustomMessage;
	setInterval( update, 33 ); //16.67 = 60 fps
}

function onCustomMessage( name, value, type ){
	if ( name == "touch" ){
		var res = value.split(":");

		// NOTE TO SELF: THIS PREVENTS THE MULTI-TAPPER FROM
		// SWEWING MESSAGE. HM!
		if ( res.length > 0 ){
            var id = res[1];
			var m = new Message();
            messages[id] = m;
            messages[id].direction = parseInt(res[0]);
			messages[id].id = res[1];
		}
	}	
}

function update(){
	// cleanup messages
    var now = Date.now();

    // jam through messages
    for (var key in messages) {
  		if (messages.hasOwnProperty(key)) {
			var timediff = now - messages[key].time;
			if ( timediff / 1000.0 > messageTimeoutSeconds ){
				delete messages[key];
			}
		}
    }
    
    // updated?
    var bShouldSend = false;

    var length = Object.keys(messages).length;

    // process current value
    if ( length > 0 ){
        switch (mode) {
            case "MODE_AVERAGE":
            {
                var total = 0;
			    for (var key in messages) {
			  		if (messages.hasOwnProperty(key)) {
                    	total += messages[key].direction;
                    }
                }

                total /= length;
                currentValue = total;
            }
            break;
                
            case "MODE_RANDOM_LEAD":
            {
                // first average
                var total = 0;
			    for (var key in messages) {
			  		if (messages.hasOwnProperty(key)) {
                    	total += messages[key].direction;
                    }
                }

                total /= length;
                
                var index = Math.floor(Math.random() * Object.keys(messages).length);
                var key = Object.keys(messages)[index];
                total = total * .5 + messages[key] * .5;

                currentValue = total;
            }
            break;
                
            case "MODE_LIVE":
            {
                // found the most recent point
                var least = 99999999;
                var tempCurrent = currentValue;

			    for (var key in messages) {
			  		if (messages.hasOwnProperty(key)) {
						var timediff = now - messages[key].time;
	                    if ( timediff < least ){
	                        tempCurrent = messages[key];
	                        least = timediff;
	                    }
                	}
                }
                currentValue = tempCurrent;
            }
            break;
                
            default:
                break;
        }
        bShouldSend = true;

        // console.log( currentValue )
        spacebrewLocal.send("average", "pad", "averager:"+ currentValue);

    } else {
        bShouldSend = false;
        // drift back to 0
        // currentValue = currentValue * .9;
    }
}

function setDefaultHost( host ){
	spacebrewHost = host;
}

function setDefaultPort( port ){
	spacebrewPort = port;
}

var processArguments = function(){
    var argv = process.argv;
    for(var i = 2; i < argv.length; i++){
        switch(argv[i]){
            case "--host":
                setDefaultHost(argv[++i]);
                break;
            case "-p":
            case "--port":
                setDefaultPort(argv[++i]);
                break;
            case "-h":
            case "--help":
                // printHelp();
                break;
        }
    }
};

/******************************************************************
	RUN RUN RUN
******************************************************************/

processArguments();
main();