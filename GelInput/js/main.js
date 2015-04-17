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

	var colors = [
		"rgb(255,255,0)","rgb(0,255,255)","rgb(255,0,255)"
	];

	var colors_alpha = [
		"rgba(255,255,0,","rgba(0,255,255,","rgba(255,0,255,"
	];

	/** @type {Object} Fill color */
	var color = {r:0, g:0, b:0};

	/** @type {Number} 0 == portait, 1 == horizontal */
	var orientationState = 0;

	/**********************************************************************/
	// ACCEL STATE STUFF
	// 
	var lastStates = {alpha:0, beta:0, gamma:0, x:0, y:0, z:0, max:0};
	var vel = {alpha:0, beta:0, gamma:0, x:0, y:0, z:0, max:0};

	var targetNorth = 300;
	var moveThreshold = 0;
	var power = 0;

	/**********************************************************************/
	function setup(){
		// setup Sender, which connects to Spacebrew + handles in/out
		sender.setup("spacebrew.robotconscience.com", app_name);// get_local_url());

		sender.getSpacebrew().addSubscribe("gameevent", "event");
		sender.getSpacebrew().addSubscribe("statusupdate", "event");

		sender.getSpacebrew().onCustomMessage = onCustomMessage;
			
		gestureHandler.setup();

		// orientation stuff
		window.promptTimeout = null;
		window.addEventListener('orientationchange', orientationChange, true);

		orientationChange();

		// all done
	}

	// @ START PROMPT FUNCTIONS

	function showLandscapePrompt(){
		orientationState = 0;
		window.clearTimeout(window.promptTimeout);
		$("#landscape").css("visibility", "visible");
		$("#landscape").css("display", "block");
		$("#landscape").css("opacity", "1");
		$("#getready").css("visibility", "hidden");
		$("#getready").css("display", "none");
	}

	function hideLandscapePrompt(){
		orientationState = 1;

		if ( current_level == 0 ){
			$("#getready").css("visibility", "visible");
			$("#getready").css("display", "block");
		}
		$("#landscape").css("opacity", "0");
		window.clearTimeout(window.promptTimeout);
		window.promptTimeout = setTimeout(function(){
			$("#landscape").css("visibility", "hidden");
			$("#landscape").css("display", "none");
		}, 500);
	}

	function orientationChange() {
        var ua = navigator.userAgent.toLowerCase();
        var isAndroid = ua.indexOf("android") > -1; // Detect Android devices
        if (isAndroid) {
            //window.orientation is different for iOS and Android
            if (window.orientation == 0 || window.orientation == 180) { //Landscape Mode
            	hideLandscapePrompt();
            }
            else if (window.orientation == 90 || window.orientation == -90) { //Portrait Mode
            	showLandscapePrompt();
            }
        }
        else {
            if (window.orientation == 90 || window.orientation == -90) { //Landscape Mode
            	hideLandscapePrompt();
            }
            else if (window.orientation == 0 || window.orientation == 180) { //Portrait Mode
            	showLandscapePrompt();
            }
        }
    }

	// @ END PROMPT FUNCTIONS

	// @begin Level functions 
	
	/**********************************************************************/
	function setupGetReady(){
		$("#getready").css("display", "block");
		$("#getready").css("visibility", "visible");
		$("#getready").css("-webkit-animation-iteration-count", "infinite");
		$("#getready").css("-moz-animation-iteration-count", "infinite");
		$("#getready").css("animation-iteration-count", "infinite");
	}

	function teardownGetReady(){
		$("#getready").css("display", "none");
		$("#getready").css("visibility", "hidden");
		$("#getready").css("-webkit-animation-iteration-count", "0");
		$("#getready").css("-moz-animation-iteration-count", "0");
		$("#getready").css("animation-iteration-count", "0");
	}

	/**********************************************************************/
	function setupLevelOne(){
		teardownPartyMode();
		teardownLevelTwo();
		teardownLevelThree();
		teardownGetReady();

		setupArrow("left", "255,255,0", 0);
		setupArrow("up", "0,255,255", 1);
		setupArrow("right", "255,0,255", 2);
		$("#level_one").css("opacity", 1);
	}

	function setupArrow(arrow, color, dir){
		$("#" + arrow).on('touchstart', function(event){
			event.stopPropagation();
			$("#" + arrow + "_arrow").css("fill", "rgba(" + color + ",1)");
			sender.send(dir, 0)

		});

		$("#" + arrow).on('touchend', function(event){
			event.stopPropagation();
			$("#" + arrow + "_arrow").css("fill", "rgba("+ color + ",0)");
		});
	}

	function teardownLevelOne(){
		$("#left").unbind();
		$("#up").unbind();
		$("#right").unbind();
		$("#level_one").css("opacity", 0);
	}

	/**********************************************************************/
	function setupLevelTwo(){
		teardownPartyMode();
		teardownLevelOne();
		teardownLevelThree();
		teardownGetReady();

		$("#level_two").css("opacity",1);
		$("#level_two").css("display","block");
		$("#level_two").css("visibility","visible");
		// listen to events
	}

	function teardownLevelTwo(){
		$("#level_two").css("opacity",0);
		$("#level_two").css("display","none");
		$("#level_two").css("visibility","hidden");
	}

	/**********************************************************************/
	function setupLevelThree(){
		teardownPartyMode();
		teardownLevelOne();
		teardownLevelTwo();
		teardownGetReady();

		$("#level_three").css("display","block");
		$("#level_three").css("visibility","visible");

		$("#hot").on('touchstart', function(event){
			event.stopPropagation();
			$("#hotbg").css("background-color", "magenta");
			$(".hot_text").css("stroke", "white");
			$(".hot_sun").css("stroke", "white");
			sender.send(0,0);
		});

		$("#hot").on('touchend', function(event){
			event.stopPropagation();
			$("#hotbg").css("background-color", "black");
			$(".hot_text").css("stroke", "magenta");
			$(".hot_sun").css("stroke", "yellow");
		});

		$("#cold").on('touchstart', function(event){
			event.stopPropagation();
			$("#coldbg").css("background-color", "cyan");
			$(".cold_text").css("stroke", "white");
			$(".cold_flake").css("stroke", "white");
			sender.send(2,0);
		});

		$("#cold").on('touchend', function(event){
			event.stopPropagation();
			$("#coldbg").css("background-color", "black");
			$(".cold_text").css("stroke", "cyan");
			$(".cold_flake").css("stroke", "white");
		});
	}

	function teardownLevelThree(){
		$("#level_three").css("display","none");
		$("#level_three").css("visibility","hidden");

		$("#hot").unbind();
		$("#cold").unbind();
	}

	/**********************************************************************/
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

	/**********************************************************************/

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

	/**********************************************************************/

	function setupEvents( el ){
	}

	/**********************************************************************/
	function draw(){
		window.requestAnimationFrame(draw.bind(this));

		var shaking = false;
		if ( current_level == 2 ){
			vel.max = 0;

			// Check if shaking
			for ( var state in gestureHandler.getState().accelStates ){
				vel[state] = Math.abs(lastStates[state] -  gestureHandler.getState().accel[state]);
				if ( vel[state] > vel.max ) vel.max = vel[state];

				if ( vel[state] > moveThreshold/10. ){
					shaking = true;
				}
				lastStates[state] = gestureHandler.getState().accel[state];
			}

			// offset by "target"
			var compass = gestureHandler.getState().gyro.alpha - targetNorth;
			if ( compass < 0 ){
				// wrap around
				compass = 1024 + compass;
			}

			var dir = Math.round(map(compass, 0, 1024, 0, 2));
			power = power * .5 + Math.round(map(vel.max, 0, 10, 0, 3)) * .5;

			if ( lastStates.alpha != compass){
				lastStates.alpha = compass;
				$("#l2_border").css("background-color", colors[dir]);
				$("#l2arrow_u").css("stroke", colors[dir]);
				$("#l2arrow_m").css("stroke", colors[dir]);
				$("#l2arrow_d").css("stroke", colors[dir]);
			}

			if ( vel.max != lastStates.max ){
				switch (Math.round(power)){
					case 0:
						$("#l2arrow_u").css("fill", colors_alpha[dir] + "0)");
						$("#l2arrow_m").css("fill", colors_alpha[dir] + "0)");
						$("#l2arrow_d").css("fill", colors_alpha[dir] + "0)");
						break;
					case 1:
						$("#l2arrow_u").css("fill", colors_alpha[dir] + "0)");
						$("#l2arrow_m").css("fill", colors_alpha[dir] + "0)");
						$("#l2arrow_d").css("fill", colors_alpha[dir] + "1)");
						break;

					case 2:
						$("#l2arrow_u").css("fill", colors_alpha[dir] + "0)");
						$("#l2arrow_m").css("fill", colors_alpha[dir] + "1)");
						$("#l2arrow_d").css("fill", colors_alpha[dir] + "1)");
						break;

					case 3:
						$("#l2arrow_u").css("fill", colors_alpha[dir] + "1)");
						$("#l2arrow_m").css("fill", colors_alpha[dir] + "1)");
						$("#l2arrow_d").css("fill", colors_alpha[dir] + "1)");
						break;
				}
			}
			power *= .9;

			lastStates.max = vel.max;

			// only send if moving!
			if ( shaking && Math.round(power) > 0){
				sender.send( dir, dir == 1 ? Math.max(power-1,0) : 0 );
			}

			// gestureHandler.getState().gamma
		} else if ( current_level == 3 ){
		}
	}

	/**********************************************************************/
	// @begin 	Spacebrew events
	function onCustomMessage( name, data, type ){
		if ( name == "gameevent" || name == "statusupdate" ){
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