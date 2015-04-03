var app;

window.onload = function(){
	app = new App();
}

var App = function(){
	var sender = new Sender();

	/** @type {String} Name in Spacebrew */
	var app_name = "gel_client_";

	/** @type {Object} Fill color */
	var color = {r:0, g:0, b:0};

	var touchWidth = 100;

	var logLevel = 0; // 0 = debug, 1 = error, 2 = none

	function setup(){
		// setup Sender, which connects to Spacebrew + handles in/out
		sender.setup("spacebrew.robotconscience.com", app_name);// get_local_url());

		color.r = Math.floor(Math.random() * 255.);
		color.g = Math.floor(Math.random() * 255.);
		color.b = Math.floor(Math.random() * 255.);

		document.body.style.backgroundColor = "rgb(" + color.r +","+ color.g +","+ color.b +")";
		$("#touch").css("backgroundColor", "rgb(" + (255-color.r) +","+ (255-color.g) +","+ (255-color.b) +")");

		setupEvents( document.body );
		// all done
	}

	function setupEvents( el ){
		el.addEventListener("mousedown", touchStart, false);
		el.addEventListener("touchstart", touchStart, false);
		el.addEventListener("touchmove", touchMove, false);
		el.addEventListener("mousemove", touchMove, false);
		el.addEventListener("touchend", touchEnd, false);
		el.addEventListener("mouseup", touchEnd, false);
		el.addEventListener("touchcancel", touchCancel, false);
		el.addEventListener("touchleave", touchEnd, false);
	}

	function draw(){
		window.requestAnimationFrame(draw.bind(this));

		// don't do anything ATM
	}

	/**
	 * [sendTouch description]
	 * @param  {[type]} index [description]
	 * @param  {[type]} x     [description]
	 * @param  {[type]} y     [description]
	 */
	function sendTouch( index, x, y ){
		console.log(x, window.innerWidth, (x - window.innerWidth/2.0) / (window.innerWidth/2.0)) ;
		x = (x - window.innerWidth/2.0) / (window.innerWidth/2.0);
		y = (y - window.innerHeight/2.0) / (window.innerHeight/2.0);

		var dir = map(x, -1, 1, 0, 2);

		if ( dir == 1 ){
			if ( y > 0 ){
				// dir = 3;
			}
		}

		sender.send(dir);
	}
	/**
	 * [touchStart description]
	 * @param  {[type]} e [description]
	 * @return {[type]}   [description]
	 */
	function touchStart( e ){
		// e.changedTouches
		// e.touches
		if ( e.touches.length == 1 ){
			$("#touch").css("opacity", 1);
			$("#touch").css("left", e.touches[0].clientX - touchWidth/2. + "px");
			$("#touch").css("top", e.touches[0].clientY - touchWidth/2. + "px");
			sendTouch(0, e.touches[0].clientX, e.touches[0].clientY)
		}
	}

	/**
	 * [touchStart description]
	 * @param  {[type]} e [description]
	 * @return {[type]}   [description]
	 */
	function touchMove( e ){
		// to-do only send moved touch
		if ( e.touches && e.touches.length > 0 ){
			$("#touch").css("opacity", 1);
			$("#touch").css("left", e.touches[0].clientX - touchWidth/2. + "px");
			$("#touch").css("top", e.touches[0].clientY - touchWidth/2. + "px");
			sendTouch(0, e.touches[0].clientX, e.touches[0].clientY)
			log(0, "sending tuch");
		} else {

			$("#touch").css("opacity", 1);
			$("#touch").css("left", e.clientX - touchWidth/2. + "px");
			$("#touch").css("top", e.clientY - touchWidth/2. + "px");
			sendTouch(0, e.clientX, e.clientY)
			log(0, "sending tuch");
		}
	}

	/**
	 * [touchStart description]
	 * @param  {[type]} e [description]
	 * @return {[type]}   [description]
	 */
	function touchEnd( e ){
		$("#touch").css("opacity", 0);
	}

	/**
	 * [touchStart description]
	 * @param  {[type]} e [description]
	 * @return {[type]}   [description]
	 */
	function touchCancel( e ){
		
	}

	// @begin 	Spacebrew events
	function onCustomMessage( name, type, value ){
		if ( name == "color" ){
			// do somethng
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
	// draw(); // kickoff animation loop
};