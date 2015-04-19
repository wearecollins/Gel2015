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
var messageTimeoutSeconds = 0.01;

/******************************************************************
	STORAGE
******************************************************************/

var messages = [];

function Message( ID, dir ){
	this.time 		= Date.now();
	this.id   		= ID;
	this.direction 	= dir;
    this.power      = 0;
    this.sent       = false;
}

// running value
var currentValue = -1;
var currentPower = 0;

// send rate
var sendRate = 500;
var lastSent = 0;

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
	setInterval( update, 16.67 ); //16.67 = 60 fps
}

function onCustomMessage( name, value, type ){
	if ( name == "touch" ){
		var res = value.split(":");

		// NOTE TO SELF: THIS PREVENTS THE MULTI-TAPPER FROM
		// SWEWING MESSAGE. HM!
		if ( res.length > 0 ){
            var id = res[1];
			var m = new Message();
            m.direction = parseInt(res[0]);
			m.id = res[1];
            if ( res.length > 2 ){
                m.power = parseInt(res[2]);
            }
			messages.push(m);
		}
	}	
}

function update(){
	// cleanup messages
    var now = Date.now();
    
    // updated?
    var bShouldSend = false;

    var length = messages.length;

    // process current value
    if ( length > 0 ){
        switch (mode) {
            case "MODE_AVERAGE":
            {
                var total = 0;
                var totalPower = 0;
			    for (var i=0; i<length; i++) {
                    messages[i].sent = true;
                	total += messages[i].direction;
                    totalPower += messages[i].power;
                }

                total /= length;
                totalPower /= length;
                currentValue = total;
                currentPower = totalPower;
            }
            break;
                
      //       case "MODE_RANDOM_LEAD":
      //       {
      //           // first average
      //           var total = 0;
      //           var totalPower = 0;
			   //  for (var key in messages) {
			  	// 	if (messages.hasOwnProperty(key)) {
      //                   messages[key].sent = true;
      //               	total += messages[key].direction;
      //                   totalPower += messages[key].power;
      //               }
      //           }

      //           total /= length;
                
      //           var index = Math.floor(Math.random() * Object.keys(messages).length);
      //           var key = Object.keys(messages)[index];
      //           total = total * .5 + messages[key].direction * .5;
      //           totalPower = totalPower * .5 + messages[key].power * .5;

      //           currentValue = total;
      //           currentPower = totalPower;
      //       }
      //       break;
                
      //       case "MODE_LIVE":
      //       {
      //           // found the most recent point
      //           var least = 99999999;
      //           var tempCurrent = currentValue;
      //           var tempPower = currentPower;

			   //  for (var key in messages) {
			  	// 	if (messages.hasOwnProperty(key)) {
						// var timediff = now - messages[key].time;
	     //                if ( timediff < least ){
      //                       messages[key].sent = true;
	     //                    tempCurrent = messages[key].direction;
      //                       tempPower = messages[key].power;
	     //                    least = timediff;
	     //                }
      //           	}
      //           }
      //           currentValue = tempCurrent;
      //           currentPower = tempPower;
      //       }
      //       break;
                
            default:
                break;
        }
        bShouldSend = true;

        // console.log( currentValue )
        if ( now - lastSent > sendRate ){
            lastSent = now;
           spacebrewLocal.send("average", "pad", "averager:"+ currentValue+":"+currentPower);
       }

    } else {
        bShouldSend = false;
        // drift back to 0
        // currentValue = currentValue * .9;
    }
    
    // jam through messages
    for (var i=messages.length-1; i>=0; i--) {
        // if (messages.hasOwnProperty(key)) {
            var timediff = now - messages[i].time;
            if ( timediff / 1000.0 > messageTimeoutSeconds && messages[i].sent === true ){
                delete messages[i];
                messages.splice(i,1);
            }
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