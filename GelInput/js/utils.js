var map = function(value, _oldMin, _oldMax, _min, _max){    
   return _min + ((value-_oldMin)/(_oldMax-_oldMin)) * (_max-_min);
}

/**
 * mapVal Maps a value in a similar way to the Arduino and Processing map method. It takes a value, along
 *   with source min and max values, and target min and max values. Then it converts the value from the 
 *   source to the target range.
 * @param  {Integer} value      The value that will be mapped.
 * @param  {Integer} source_min The minimum value for the source range.
 * @param  {Integer} source_max The maximum value for the source range.
 * @param  {Integer} target_min The minimum value for the target/outgoing range.
 * @param  {Integer} target_max The maximum value for the target/outgoing range.
 * @return {Integer}            The mapped value.
 */
var mapRounded = function(value, source_min, source_max, target_min, target_max) {
  if (!(value && source_min && source_max && target_min  && target_max)) "missing parameter";
  if (isNaN(value) || isNaN(source_min) || isNaN(source_max) || isNaN(target_min) || isNaN(target_max)) "not a number";
  if (value > source_max) value = source_max;
  if (value < source_min) value = source_min;
  var source_delta = source_max - source_min;
  var target_delta = target_max - target_min;
  var value_abs = value - source_min;
  var mapped_abs = value_abs * (target_delta / source_delta);
  var mapped_final = target_min + mapped_abs;

  return Math.round(mapped_final);
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