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

	function setup(){
		// setup Sender, which connects to Spacebrew + handles in/out
		sender.setup("spacebrew.robotconscience.com", app_name);// get_local_url());
		// setupEvents( document.body );
		
		setupLevelOne();

		// all done
	}

	// @begin Level functions 
	
	function setupLevelOne(){
		setupArrow("left", "255,255,0", 0);
		setupArrow("up", "0,255,255", 1);
		setupArrow("right", "255,0,255", 2);
	}

	function setupArrow(arrow, color, dir){
		$("#" + arrow + "_svg").mousedown(function(){
			$("#" + arrow + "_arrow").css("fill", "rgba(" + color + ",1)");
			sender.send(dir)
		});
		$("#container").mouseup(function(){
			$("#" + arrow + "_arrow").css("fill", "rgba("+ color + ",0)");
		});
	}

	function teardownLevelOne(){

	}

	function setupLevelTwo(){

	}

	function teardownLevelTwo(){
		
	}

	function setupLevelThree(){

	}

	function teardownLevelThree(){
		
	}

	// @end Level functions 

	function setupEvents( el ){
		// el.addEventListener("mousedown", touchStart, false);
		// el.addEventListener("touchstart", touchStart, false);
		// el.addEventListener("touchmove", touchMove, false);
		// el.addEventListener("mousemove", touchMove, false);
		// el.addEventListener("touchend", touchEnd, false);
		// el.addEventListener("mouseup", touchEnd, false);
		// el.addEventListener("touchcancel", touchCancel, false);
		// el.addEventListener("touchleave", touchEnd, false);
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

		var dir = map(x, -1, 1, 0, 3);

		if ( dir == 1 ){
			if ( y > 0 ){
				// dir = 3;
			}
		}

		sender.send(dir);
	}

	// @begin 	Spacebrew events
	function onCustomMessage( name, type, value ){
		if ( name == "color" ){
			// do somethng
		}
	}

	// @end 	Spacebrew events

	setup();
	// draw(); // kickoff animation loop
};