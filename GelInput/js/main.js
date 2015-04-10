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

		setupArrow("left", "255,255,0", 0);
		setupArrow("up", "0,255,255", 1);
		setupArrow("right", "255,0,255", 2);
		$("#container").css("opacity", 1);

		teardownPartyMode();
	}

	function setupArrow(arrow, color, dir){
		// $("#" + arrow + "_svg").mousedown(function(){
		// 	$("#" + arrow + "_arrow").css("fill", "rgba(" + color + ",1)");
		// 	sender.send(dir)
		// });

		$("#" + arrow).on('touchstart', function(event){
			event.stopPropagation();
			$("#" + arrow + "_arrow").css("fill", "rgba(" + color + ",1)");
			sender.send(dir)
		});

		// $("#container").mouseup(function(){
		// 	$("#" + arrow + "_arrow").css("fill", "rgba("+ color + ",0)");
		// });

		$("#" + arrow).on('touchend', function(event){
			event.stopPropagation();
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
		$("#container").css("opacity", 1);
		teardownLevelOne();
		$("#ball").css("opacity", 1);
		$("#"+ getDirName(0) +"_border").css("background-color", "white");
		$("#"+ getDirName(1) +"_border").css("background-color", "white");
		$("#"+ getDirName(2) +"_border").css("background-color", "white");
		// listen to events
	}

	function teardownLevelTwo(){
		teardownPartyMode();
		$("#ball").css("opacity", 0);
	}

	function setupLevelThree(){
		teardownPartyMode();
		$("#container").css("opacity", 1);
		teardownLevelOne();
		$("#compass").css("opacity", 1);

		// THIS SHOULD BE CSS DUMMY
		$("#compass").css("left", window.innerWidth/2.0 - $("#compass").width()/2 + "px");
		$("#compass").css("top", window.innerHeight/2.0 - $("#compass").width()/2 + "px");
		// already listening to events!
	}

	function teardownLevelThree(){
		$("#compass").css("opacity", 0);
		teardownPartyMode();
	}

	// PARTY MODE
	
	var partyTimeout = null;

	function triggerPartyMode( level ){
		$("#party").css("visibility", "visible");
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
		partyTimeout = setTimeout(function(){
			$("#party").css("visibility", "hidden");
		}, 500);
	}

	var colors = [
		"rgb(255,255,0)","rgb(0,255,255)","rgb(255,0,255)"
	];

	function getDirName( dir ){
		switch(dir){
			case 0:
			return "left";
			case 1:
			return "up";
			case 2:
			return "right";
		}
	}

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

	var lastStates = {alpha:0, beta:0, gamma:0, x:0, y:0, z:0};

	var targetNorth = 180;

	var moveThreshold = 50;

	function draw(){
		window.requestAnimationFrame(draw.bind(this));

		var shaking = false;
		if ( current_level > 1 ){
			for ( var state in gestureHandler.getState().accelStates ){
				if ( Math.abs(lastStates[state] -  gestureHandler.getState().accel[state]) > moveThreshold/10. ){
					shaking = true;
				}
				lastStates[state] = gestureHandler.getState().accel[state];
			}
		}

		if ( current_level == 2 ){

			if ( gestureHandler.getState().gyroStates.beta ){
				$("#ball").css("top", map(gestureHandler.getState().gyro.beta, 0, 1024, $("ball").width(), window.innerHeight - $("ball").width()) + "px");
				$("#ball").css("left", map(gestureHandler.getState().gyro.gamma, 0, 1024, $("ball").width(), window.innerWidth - $("ball").width()) + "px");
				
				var dir = Math.floor(map(gestureHandler.getState().gyro.gamma, 200, 700, 0, 2));
				
				// only send if moving!
				if ( shaking ){
					$("#"+ getDirName(dir) +"_border").css("background-color", colors[dir]);
					sender.send( dir );
				}
			} else {
				$("#"+ getDirName(0) +"_border").css("background-color", "white");
				$("#"+ getDirName(1) +"_border").css("background-color", "white");
				$("#"+ getDirName(2) +"_border").css("background-color", "white");
			}

			// gestureHandler.getState().gamma

			// orientationState == 1
		} else if ( current_level == 3 ){
			var angle = Math.floor(map(gestureHandler.getState().gyro.alpha, 0, 1024, 0, 360));

			$("#compass").css("-ms-transform", "rotate(" + (-(angle-targetNorth) )+ "deg)");
			$("#compass").css("-webkit-transform", "rotate(" + (-(angle-targetNorth) ) + "deg)");
			$("#compass").css("transform", "rotate(" + (-(angle-targetNorth) ) + "deg)");

			// only send if moving!
			if ( shaking){
				var dir = 1;
				if ( Math.abs(targetNorth-angle) > 50 && targetNorth-angle < 0  ){
					dir = 2;
				} else if ( Math.abs(targetNorth-angle) > 50 && targetNorth-angle > 0  ){
					dir = 0;
				}
				$("#"+ getDirName(dir) +"_border").css("background-color", colors[dir]);
				sender.send( dir );
			} else {
				$("#"+ getDirName(0) +"_border").css("background-color", "white");
				$("#"+ getDirName(1) +"_border").css("background-color", "white");
				$("#"+ getDirName(2) +"_border").css("background-color", "white");
			}
		}
	}

	// @begin 	Spacebrew events
	function onCustomMessage( name, data, type ){
		if ( name == "gameevent" ){
			// console.log( value );
			// value = JSON.parse( value );

			if ( data.name == "level" ){
				if ( data.value == "level one"){
					if ( current_level != 1){
						setupLevelOne();
						current_level = 1;
					}
				} else if (data.value == "level two"){
					if ( current_level != 2){
						teardownLevelOne();
						setupLevelTwo();
						current_level = 2;
					}
				} else if (data.value == "level three"){
					if ( current_level != 3){
						teardownLevelTwo();
						setupLevelThree();
						current_level = 3;
					}
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