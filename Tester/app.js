/******************************************************************
	SPACEBREW
******************************************************************/

var Spacebrew = require("./Spacebrew.js").Spacebrew;

var spacebrewLocal, spacebrewRemote;

var spacebrewHost = "localhost";
// var remoteAddress = "spacebrew.robotconscience.com";

/******************************************************************
	APP
******************************************************************/

var name = "gel_client_"

function main(){
	console.log( "CONNECTING TO "+ spacebrewHost);
    name = name + Math.floor(Math.random() * 10000);

	// setup connection: Local
	spacebrewLocal = new Spacebrew.Client(spacebrewHost, name, "");

	// pad== "dir:id", dir == 0-4 (U,R,D,L, Look)
	spacebrewLocal.addPublish("touch", "pad");
	spacebrewLocal.connect();

    setTimeout(sendRandom, 50 + Math.random() * 2000 );
}

function sendRandom(){
    var val = Math.round( Math.random() * 2 );
    spacebrewLocal.send("touch", "pad", val+ ":"+ name );

    setTimeout(sendRandom, 50 + Math.random() * 2000 );
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