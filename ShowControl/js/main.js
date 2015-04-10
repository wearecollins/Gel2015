window.onload = function(){
	var sb = new Spacebrew.Client("spacebrew.robotconscience.com", "ShowControl");
	sb.addPublish("windowEvent", "string");
	sb.connect();

	$("#level_prev").click(function(){
		sb.send("windowEvent", "string", "level_prev");
	})

	$("#level_next").click(function(){
		sb.send("windowEvent", "string", "level_next");
	})

	$("#slide_prev").click(function(){
		sb.send("windowEvent", "string", "slide_prev");
	})

	$("#slide_next").click(function(){
		sb.send("windowEvent", "string", "slide_next");
	})

	$("#start_level").click(function(){
		sb.send("windowEvent", "string", "start_level");
	})

	$("#finish_level").click(function(){
		sb.send("windowEvent", "string", "finish_level");
	})

	$("#mouse_on").click(function(){
		sb.send("windowEvent", "string", "mouse_on");
	})

	$("#mouse_off").click(function(){
		sb.send("windowEvent", "string", "mouse_off");
	})
};





