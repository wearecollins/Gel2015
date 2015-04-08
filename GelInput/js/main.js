var app;

window.onload = function(){
	app = new App();
}

var App = function(){
	var sender = new Sender();

	/** @type {String} Name in Spacebrew */
	var app_name = "gel_client_";

	/** @type {GestureHandler} */
	var gestureHandler = new GestureHandler();

	var current_level = 0;

	var bPartyMode = false;

	/** @type {Object} Fill color */
	var color = {r:0, g:0, b:0};

	/** @type {Number} 0 == portait, 1 == horizontal */
	var orientationState = 0;

	function setup(){
		// setup Sender, which connects to Spacebrew + handles in/out
		sender.setup("spacebrew.robotconscience.com", app_name);// get_local_url());

		sender.getSpacebrew().addSubscribe("gameevent", "event");
		sender.getSpacebrew().onCustomMessage = onCustomMessage;
			
		gestureHandler.setup();

		// orientation stuff
		window.addEventListener('orientationchange', function () {
			switch (window.orientation){
				// horiz
				case -90:
				case 90:
					orientationState = 1;
					break;

				// vertical
				case 0:
					orientationState = 0;
					break;
			}
		}, true);

		// all done
	}

	// @begin Level functions 
	
	function setupLevelOne(){
		teardownPartyMode();

		setupArrow("left", "255,255,0", 0);
		setupArrow("up", "0,255,255", 1);
		setupArrow("right", "255,0,255", 2);
		$("#container").css("opacity", 1);
	}

	function setupArrow(arrow, color, dir){
		// $("#" + arrow + "_svg").mousedown(function(){
		// 	$("#" + arrow + "_arrow").css("fill", "rgba(" + color + ",1)");
		// 	sender.send(dir)
		// });

		$("#" + arrow).on('touchstart', function(){
			console.log("YES");
			$("#" + arrow + "_arrow").css("fill", "rgba(" + color + ",1)");
			sender.send(dir)
		});

		// $("#container").mouseup(function(){
		// 	$("#" + arrow + "_arrow").css("fill", "rgba("+ color + ",0)");
		// });

		$("#" + arrow).on('touchend', function(){
			console.log("YES");
			$("#" + arrow + "_arrow").css("fill", "rgba("+ color + ",0)");
		});
	}

	function teardownLevelOne(){
		teardownPartyMode();

		$("#left_svg").css("opacity", 0);
		$("#right_svg").css("opacity", 0);
		$("#up_svg").css("opacity", 0);
		$("#left").unbind();
		$("#up").unbind();
		$("#right").unbind();
	}

	function setupLevelTwo(){
		teardownPartyMode();
		$("#ball").css("opacity", 1);
		// listen to events
	}

	function teardownLevelTwo(){
		teardownPartyMode();
		$("#ball").css("opacity", 0);
	}

	function setupLevelThree(){
		teardownPartyMode();
		$("#compass").css("opacity", 1);
		// already listening to events!
	}

	function teardownLevelThree(){
		$("#compass").css("opacity", 0);
		teardownPartyMode();
	}

	// PARTY MODE
	
	var partyTimeout = null;

	function triggerPartyMode( level ){
		if (partyTimeout != null) clearInterval(partyTimeout);

		bPartyMode = true;
		switch( level ){

		}

		partyTimeout = setInterval(setRandomBackground, 250);//Math.random() * 750 + 250 );

		setRandomBackground();
	}

	function teardownPartyMode(){
		if (partyTimeout != null) clearInterval(partyTimeout);
		bPartyMode = false;
		$("#party").css("opacity", "0");
	}

	var colors = [
		"rgb(255,255,0)","rgb(255,0,255)","rgb(0,255,255)"
	]

	var lastIndex = -1;

	function setRandomBackground(){
		$("#party").css("opacity", "1");

		var bg = $("#party").css("background-color");

		var index = Math.floor(Math.random() * colors.length );
		if ( index == lastIndex ){
			index++;
			if ( index >= colors.length ) index = 0;
		}
		lastIndex = index;

		$("#party").css("background-color", colors[index]);
		// partyTimeout = setTimeout(setRandomBackground, 250);//Math.random() * 750 + 250 );
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

		if ( current_level == 2 ){

			$("#ball").css("left", map(gestureHandler.getState().beta, 0, 1024, $("ball").width(), window.innerWidth - $("ball").width()) + "px");
			$("#ball").css("top", map(gestureHandler.getState().gamma, 0, 1024, $("ball").width(), window.innerWidth - $("ball").width()) + "px");
			
			// only send if tilted forward!

			// gestureHandler.getState().gamma

			// orientationState == 1
		} else if ( current_level == 3 ){
			gestureHandler.getState()
		}
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
	function onCustomMessage( name, data, type ){
		console.log("?");
		if ( name == "gameevent" ){
			// console.log( value );
			// value = JSON.parse( value );

			if ( data.name == "level" ){
				if ( data.value == "level one"){
					setupLevelOne();
					current_level = 1;
				} else if (data.value == "level two"){
					teardownLevelOne();
					setupLevelTwo();
					current_level = 2;
				} else if (data.value == "level three"){
					teardownLevelTwo();
					setupLevelThree();
					current_level = 3;
				}
			} else if ( data.name == "trigger"){
				// this should be a partial match, duh
				if ( data.value == "level one complete!"){
					triggerPartyMode(1);
				} else if (data.value == "level two complete!"){
					triggerPartyMode(2);
				} else if (data.value == "level three complete!"){
					triggerPartyMode(3);
				}
			}
		}
	}

	// @end 	Spacebrew events

	setup();
	draw(); // kickoff animation loop
};