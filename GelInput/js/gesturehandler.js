var GestureHandler = function(){
	var state = {};

    state.services = { "accel": false, "gyro": false };
    state.sensors = [ "accel", "gyro" ];
    state.accel = { x: 0, y: 0, z: 0 };
    state.gyro = { alpha: 0, beta: 0, gamma: 0 };
    state.txtVisible = true;

    state.ids = {};
    state.ids.accel = ["x", "y", "z"];
    state.ids.gyro = ["alpha", "beta", "gamma", "webkitCompassHeading"];
    state.ids.color = ["hue", "sat", "light"];

    /** @type {Object} acce/gyro output to {min:Number, max:Number } */
    var outputBounds = {min:0, max: 1024}

    // holds the max and min values to map spacebrew range to range of the sensors and outputs
    state.bounds = {};
    state.bounds.accel = {              // values to map the accelerometer values to range values (from iphone 4)
      low: { x: -10, y: -10, z: -10 },
      high: { x: 10, y: 10, z: 10 }
    };
    state.bounds.gyro = {               // values to map the gyrometer values to range values (from iphone 4)
      low: { alpha: 0, beta: -90, gamma: -180 },
      high: { alpha: 360, beta: 90, gamma: 180 }
    };
    state.bounds.color = {              // value to map range to set the background color
      low: { hue: 0, sat: 0, light: 0 },
      high: { hue: 360, sat: 100, light: 100 }
    };

	var debug = false;

	this.setup = function( onsuccess, onerror ){
		registerMotionEventListeners( onsuccess, onerror );
	}

	this.getState = function(){
		return state;
	}

	this.setBounds = function(min,max){
		outputBounds.min = min;
		outputBounds.max = max;
	}

	/**
	 * registerMotionEventListeners Register callback methods for accelerometer and gyrometer events.
	 *   If connected to Spacebrew, messages are sent when a new event is received.
	 * @return {none} 
	 */
	var registerMotionEventListeners = function ( onsuccess, onerror) {
		// check if device has an gyrometer, and if so, then register an event handler
		if (window.DeviceOrientationEvent) {
			// if (debug) console.log("[registerMotionEventListeners] gyro device orientation event available " );
			
			window.addEventListener("deviceorientation", function() {
		    	processEvent("gyro", event);
		    }, true);    
		    state.services["gyro"] = true;

			if ( onsuccess ){
				onsuccess("gyro");
			}
		} else {
			// $("#deviceMsggyro h1").html(htmlForTextWithEmbeddedNewlines("gyro"));    
			// $("#deviceMsggyro p").html(htmlForTextWithEmbeddedNewlines("Data not available."));    
		    state.services["gyro"] = false;

			if ( onerror ){
				onerror("gyro");
			}
		}

		if (window.DeviceMotionEvent) {
		    // if (debug) console.log("[registerIphoneEventListeners] accel motion event available " );
		    window.addEventListener("devicemotion", function() {
		    	processEvent("accel", event.acceleration);
		    }, true);  
		    state.services["accel"] = true;
		    if ( onsuccess ){
				onsuccess("accel");
			}
		  } else {
			$("#deviceMsgaccel h1").html(htmlForTextWithEmbeddedNewlines("accel"));    
			$("#deviceMsgaccel p").html(htmlForTextWithEmbeddedNewlines("Data not available."));    
			state.services["accel"] = false;

			if ( onerror ){
				onerror("accel");
			}
		  }
	}

	/**
	 * processEvent Processes new movement events and in response updates the data on the UI, and sends
	 *   the appropriate Spacebrew messages.
	 * @param  {String} name The name of the sensor associated to this event
	 * @param  {Object} data An object that includes data about all sources associated to the sensor
	 *                       named in the previous paramter. This object needs to feature the appropriate
	 *                       attribute names in order for the method to process them.
	 * @return {none}      
	 */
	var processEvent = function (name, data) {
		// console.log( data );
		var debug = true;
		if (state.ids[name]) {
			var sensor = name;
			var parts = state.ids[sensor];
			var new_data = false;

			// loop through each source associated to the current sensor
			for (var p in parts) {
				var part = parts[p];
				// console.log( name, part, data[part] );

				if (!data[part]) continue; // if data[part] doesn't exist then skip to next part

				var sensor_data = data[part];

				// hack to overwrite alpha with compass
				if ( part == "webkitCompassHeading" ){
					window.useCompassAlpha = true;
					part = "alpha";
					// console.log("Using webkit!");
				} else if ( part == "alpha" && window.useCompassAlpha === true ){
					// console.log("Throwing away alpha!");
					continue;
				}

				var new_state = mapRounded( sensor_data, state.bounds[sensor]["low"][part], 
			                          state.bounds[sensor]["high"][part], outputBounds.min, outputBounds.max );

				if ( name == "accel") new_state = data[part];

			  // if the new state is different from state[sensor][part] then update state[sensor][part]
				if (state[sensor][part] != new_state) {
					state[sensor][part] = new_state;
					// if (debug) console.log("[processEvent] new value for " + sensor + " part " + part + " val " + state[sensor][part] );

					// if connected to spacebrew then send messages
					// if (sb.connected) {
					// 	var outlet_name = sensor + "_" + part;
					// 	sb.connection.send(outlet_name, "range", state[sensor][part]); 
					// 	new_data = true;
					// }
				}
			}

			// if any of the sensor sources were updated then send a summary message
			// if (new_data && sb.connected) sb.connection.send(sensor, "string", JSON.stringify(state[sensor]));      
		}
	}

}