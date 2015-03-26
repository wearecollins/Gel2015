var app;

window.onload = function(){
	app = new App();
}

var map = function(value, _oldMin, _oldMax, _min, _max){    
   return _min + ((value-_oldMin)/(_oldMax-_oldMin)) * (_max-_min);
}

function get_local_url()
{
	var pcol = "";
	var u = document.URL;

	/*
	 * We open the websocket encrypted if this page came on an
	 * https:// url itself, otherwise unencrypted
	 */

	if (u.substring(0, 5) == "https") {

		u = u.substr(8);
	} else {
		if (u.substring(0, 4) == "http")
			u = u.substr(7);
	}

	u = u.split('/');

	u = u[0];
	var p = u.indexOf(":");
	u = u.substr(0, p);

	return pcol + u;
}

var App = function(){
	/** @type {Spacebrew.Client} */
	var sb;

	/** @type {String} Name in Spacebrew */
	var app_name = "gel_output_";

	/** @type {Object} Fill color */
	var color = {r:0, g:0, b:0};

	var touchWidth = 100;

	var logLevel = 0; // 0 = debug, 1 = error, 2 = none

	function setup(){
		var random_id = "0000" + Math.floor(Math.random() * 10000);

		app_name = app_name + ' ' + random_id.substring(random_id.length-4);

		sb = new Spacebrew.Client("spacebrew.robotconscience.com");// get_local_url());

		sb.name(app_name);

		// configure the publication and subscription feeds
		sb.addSubscribe("average", "point2d");

		// override Spacebrew events - this is how you catch events coming from Spacebrew
		sb.onCustomMessage = onCustomMessage;
		sb.onOpen = onOpen;
		sb.onClose = onClose;

		// connect to spacbrew
		sb.connect();
		setupEvents( document.body );
		// all done
	}

	function setupEvents( el ){
	}

	function draw(){
		window.requestAnimationFrame(draw.bind(this));

	}

	// @begin 	Spacebrew events
	function onOpen(){
		var o = {r:color.r, g:color.g, b:color.b, id:app_name};
		sb.send("announce", "announce", JSON.stringify(o));
	}

	function onClose(){}

	var t = null;

	function onCustomMessage( name, value, type ){
		if ( name == "average" ){
			if ( t != null ) clearTimeout(t);
			var x = (value.x);
			// value = JSON.parse(value);
			var y = (value.y);

			var angle = map(x, -1.,1, -90, 90);
			if ( angle < 0 ){
				if ( angle < -45 ){
					angle = -90;
				} else {
					angle = 0;
				}
			} else {
				if ( angle > 45 ){
					angle = 90;
				} else {
					angle = 0;
				}
			}
			$("#arrow").css("transform", "rotate(" + angle + "deg)");
			$("#arrow").css("opacity", "1");
			t = setTimeout(function(){
				$("#arrow").css("opacity", "0");
			}, 200);
		}
	}

	// @end 	Spacebrew events

	// usage: log ( 0, stuff, stuff, ...)
	function log( level ){
		if ( arguments.length == 1 ){
			log(0,arguments);
		}
		if ( logLevel <= level ){
			for ( var i=1; i<arguments.length; i++)
				console.log( arguments[i] );
		}
	}

	setup();
	draw(); // kickoff animation loop
};