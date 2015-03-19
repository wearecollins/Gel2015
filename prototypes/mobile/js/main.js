var app;

window.onload = function(){
	app = new App();
}

var App = function(){
	/** @type {Spacebrew.Client} */
	var sb;

	/** @type {String} Name in Spacebrew */
	var app_name = "gel_client_";

	/** @type {Object} Fill color */
	var color = {r:0, g:0, b:0};

	var touchWidth = 100;

	var logLevel = 0; // 0 = debug, 1 = error, 2 = none

	function setup(){
		var random_id = "0000" + Math.floor(Math.random() * 10000);

		app_name = app_name + ' ' + random_id.substring(random_id.length-4);

		log(0, "Setting up spacebrew connection");
		sb = new Spacebrew.Client("192.168.88.161");

		sb.name(app_name);

		// configure the publication and subscription feeds
		sb.addPublish("touch", "point2d", "{\"x\":0,\"y\":0}");
		sb.addPublish("announce", "color", "{\"r\":0,\"g\":0,\"b\":0,}");

		// maybe color will be a publish?
		sb.addSubscribe("color", "color");

		// override Spacebrew events - this is how you catch events coming from Spacebrew
		sb.onCustomMessage = onCustomMessage;
		sb.onOpen = onOpen;
		sb.onClose = onClose;

		color.r = Math.floor(Math.random() * 255.);
		color.g = Math.floor(Math.random() * 255.);
		color.b = Math.floor(Math.random() * 255.);

		document.body.style.backgroundColor = "rgb(" + color.r +","+ color.g +","+ color.b +")";
		$("#touch").css("backgroundColor", "rgb(" + (255-color.r) +","+ (255-color.g) +","+ (255-color.b) +")");


		// connect to spacbrew
		sb.connect();
		setupEvents( document.body );
		// all done
	}

	function setupEvents( el ){
		el.addEventListener("touchstart", touchStart, false);
		el.addEventListener("touchmove", touchMove, false);
		el.addEventListener("touchend", touchEnd, false);
		el.addEventListener("touchcancel", touchCancel, false);
		el.addEventListener("touchleave", touchEnd, false);
	}

	function draw(){
		window.requestAnimationFrame(draw.bind(this));

	}

	/**
	 * [sendTouch description]
	 * @param  {[type]} index [description]
	 * @param  {[type]} x     [description]
	 * @param  {[type]} y     [description]
	 */
	function sendTouch( index, x, y ){
		x = x / window.innerWidth;
		y = y / window.innerHeight;
		sb.send("touch", "point2d", JSON.stringify({x:x, y:y, index:index, id:app_name}));
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
		if ( e.touches.length > 0 ){
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
	function onOpen(){
		sb.send("announce", "color", JSON.stringify(color));
	}

	function onClose(){}

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
	draw(); // kickoff animation loop
};