"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
var socket = io.connect();

var errorTime;

$(document).ready(function() {
	sendCommand("pump off\n");
	sendCommand("get moist_min\n");
	sendCommand("get moist_max\n");
	sendCommand("get light_max\n");
	sendCommand("get light_min\n");

	// Hide the error box
	$("#error-box").hide();

	// Adjust email
	$("#confirmEmail").click(function(){
		var value = $("#userEmail").val()
		sendCommand("email " + value + "\n");
	});
	$("#sampleEmail").click(function(){
		var value = $("#userEmail").val()
		sendCommand("email " + value + "\n");
		sendCommand("sample\n");
	});

  	// Adjust DOB
  	$("#setDOB").click(function(){
		var value = $("plantDOB").val()
		sendCommand("DOB " + value + "\n");
	});

	// Adjust pump
	$("#pumpOn").click(function(){
		sendCommand("pump on\n");
	});
	$("#pumpOff").click(function(){
		sendCommand("pump off\n");
	});

	// Adjust Min
	$("#range-slider-moist-min").on("change", function() {
		var value = $(this).val();
		console.log("change");
		$("#input-slider-moist-min").val(value);
		sendCommand("moist_min " + value + "\n");
	});

	$("#range-slider-light-min").on("change", function() {
		var value = $(this).val();
		$("#input-slider-light-min").val(value);
		sendCommand("light_min " + value + "\n");
	});

	// Adjust Max
	$("#range-slider-moist-max").on("change", function() {
		var value = $(this).val();
		$("#input-slider-moist-max").val(value);
		sendCommand("moist_max " + value + "\n");
	});
	$("#range-slider-light-max").on("change", function() {
		var value = $(this).val();
		$("#input-slider-light-max").val(value);
		sendCommand("light_max " + value + "\n");
	});

	// Setup a repeating function (every 1s)
	window.setInterval(function() {getUpdate()}, 1000);
	handleReplies();
	
});

function sendCommand(message) {
	socket.emit('plant-care', message);
};

function getUpdate() {
	// Get the current volume, tempo, mode
	sendCommand("get light\n");
	sendCommand("get moist\n");
}

function handleReplies() {
	// Display the received data
	socket.on("commandReply", function(result) {
		$("#error-box").hide();

		// console.log(result);
		var reply = result.trim().split(":")
		switch (reply[0]) {
			case "email":
				$("#userEmail").text(reply[1]);
				break;
			case "pump":
				$("#pumpStatus").text(reply[1]);
				break;
			case "moist":
				$("#moistStatus").html(reply[1]);
				break;
      case "DOB":
        $("#DOB").html(reply[1]);
				break;
      case "age":
        $("#plantAge").html(reply[1]);
				break;
			case "moist_min":
				$("#range-slider-moist-min").val(reply[1]);
				$("#input-slider-moist-min").val(reply[1]);
				break;
			case "moist_max":
				$("#range-slider-moist-max").val(reply[1]);
				$("#input-slider-moist-max").val(reply[1]);
				break;
			case "light":
				$("#lightStatus").html(reply[1]);
				break;
			case "light_min":
				$("#range-slider-light-min").val(reply[1]);
				$("#input-slider-light-min").val(reply[1]);
				break;
			case "light_max":
				$("#range-slider-light-max").val(reply[1]);
				$("#input-slider-light-max").val(reply[1]);
				break;
			default:
				break;
		}
	});

	socket.on("DaError", function(result) {
		$("#error-box").show();
		$("#error-text").text(result.contents);
	});
}