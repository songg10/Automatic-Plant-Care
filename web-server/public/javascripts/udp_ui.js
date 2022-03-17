"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
var socket = io.connect();

var errorTime;

// Emulate enum in C
const NO_ERROR     = 0
const NODEJS_ERROR = 1;
const UDP_ERROR    = 2
var error_origin   = NO_ERROR;

$(document).ready(function() {
	sendCommand("pump off\n")
	// Hide the error box
	$("#error-box").hide();

	// Adjust pump
	$("#confirmEmail").click(function(){
		var value = $("#userEmail").val()
		sendCommand("email " + value + "\n");
	});
	$("#sampleEmail").click(function(){
		var value = $("#userEmail").val()
		sendCommand("email " + value + "\n" + "sample\n");
	});

	// Adjust email
	$("#pumpOn").click(function(){
		sendCommand("pump on\n");
	});
	$("#pumpOff").click(function(){
		sendCommand("pump off\n");
	});

	// Adjust Min
	$("range-slider-moist-min").on("slidechange", function(event, ui) {
		$("#input-slider-moist-min").val(ui.value)
		sendCommand("moisture_min " + ui.value + "\n")
	});
	$("range-slider-light-min").on("slidechange", function(event, ui) {
		$("#input-slider-light-min").val(ui.value)
		sendCommand("light_min " + ui.value + "\n")
	});

	// Adjust Max
	$("range-slider-moist-max").on("slidechange", function(event, ui) {
		$("#input-slider-moist-max").val(ui.value)
		sendCommand("moisture_max " + ui.value + "\n")
	});
	$("range-slider-light-max").on("slidechange", function(event, ui) {
		$("#input-slider-light-max").val(ui.value)
		sendCommand("light_max " + ui.value + "\n")
	});

	// Setup a repeating function (every 1s)
	window.setInterval(function() {getUpdate()}, 1000);
	handleReplies();
	
});

function sendCommand(message) {
	socket.emit('beat_box', message);
};

function getUpdate() {
	// Get the current volume, tempo, mode
	sendCommand("get light\n");
	sendCommand("get moist\n");
}

function handleReplies() {
	// Display the received data
	socket.on("commandReply", function(result) {
		if (error_origin == UDP_ERROR) {
			// setTimeout(function() {
			// 	console.log("UDP ERROR CLEAR");
			// 	$("#error-box").hide();
			// }, 10000);
			clearTimeout(errorTime);
			$("#error-box").hide();
		}

		console.log(result);
		var reply = result.trim().split(":")
		switch (reply[0]) {
			case "email":
				$("#userEmail").text(reply[1]);
				break;
			case "pump":
				$("#pumpStatus").text(reply[1]);
				break;
			case "light":
				$("#lightStatus").val(reply[1]);
				break;
			case "moist":
				$("#moistStatus").val(reply[1]);
				break;

			default:
				break;
		}
	});

	socket.on("DaError", function(result) {
		$("#error-box").show();
		$("#error-text").text(result.contents);
		error_origin = UDP_ERROR;
	});
}