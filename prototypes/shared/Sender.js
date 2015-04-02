var Sender = function(){

	var sb;

	/** @type {String} Name in Spacebrew */
	var app_name = "gel_client_";

	/** @type {Object} Fill color */
	var color = {r:0, g:0, b:0};
	
	// function sendTouch( index, x, y ){
	// 	console.log(x, window.innerWidth, (x - window.innerWidth/2.0) / (window.innerWidth/2.0)) ;
	// 	x = (x - window.innerWidth/2.0) / (window.innerWidth/2.0);
	// 	y = (y - window.innerHeight/2.0) / (window.innerHeight/2.0);
	// 	var t = {x:""+x, y:""+y, index:index, id:app_name, color: color};
	// 	sb.send("touch", "gelpoint", JSON.stringify(t));
	// }

	this.setup = function( host, name ){
		app_name = name;

		var random_id = "0000" + Math.floor(Math.random() * 10000);

		app_name = app_name + ' ' + random_id.substring(random_id.length-4);

		sb = new Spacebrew.Client(host);

		sb.name(app_name);

		// configure the publication and subscription feeds
		sb.addPublish("touch", "gelpoint", "{\"x\":0,\"y\":0}");
		sb.addPublish("direction", "pad", "0"); // 0-3, URDL
		sb.addPublish("announce", "announce", "{\"id\":\"\",\"r\":0,\"g\":0,\"b\":0,}");

		sb.onOpen = onOpen;
		sb.onClose = onClose;

		color.r = Math.floor(Math.random() * 255.);
		color.g = Math.floor(Math.random() * 255.);
		color.b = Math.floor(Math.random() * 255.);

		document.body.style.backgroundColor = "rgb(" + color.r +","+ color.g +","+ color.b +")";
		$("#touch").css("backgroundColor", "rgb(" + (255-color.r) +","+ (255-color.g) +","+ (255-color.b) +")");


		// connect to spacbrew
		sb.connect();
		// all done
	}

	this.getSpacebrew = function(){
		return sb;
	}

	/**
	 * Send URDL 0-3
	 * @param  {Integer} direction 0-3
	 * @param  {Object} data      Note in use, but maybe later?
	 */
	this.send = function( direction, data ){
		if ( direction > 3 ){
			console.error("Data must be 0-3! Sending 0 (up)");
			direction = 0;
		}
		sb.send("direction", "pad", direction + ":" + app_name );
	}

	// private
	function onOpen(){
		var o = {r:color.r, g:color.g, b:color.b, id:app_name};
		sb.send("announce", "announce", JSON.stringify(o));
	}

	function onClose(){

	}
}