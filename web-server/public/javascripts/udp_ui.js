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
	// Hide the error box
	$("#error-box").hide();

	// Adjust volume
	$("#volumeUp").click(function(){
		sendBeatCommand("volume up\n");
	});
	$("#volumeDown").click(function(){
		sendBeatCommand("volume down\n");
	});

	// Adjust tempo
	$("#tempoUp").click(function(){
		sendBeatCommand("tempo up\n");
	});
	$("#tempoDown").click(function(){
		sendBeatCommand("tempo down\n");
	});

	// Change between beat modes
	$("#modeNone").click(function(){
		sendBeatCommand("beat none\n");
	});
	$("#modeRock1").click(function(){
		sendBeatCommand("beat rock_1\n");
	});
	$("#modeRock2").click(function(){
		sendBeatCommand("beat rock_2\n");
	});
	$("#modeRUNNN").click(function(){
		sendBeatCommand("beat runnn\n");
	});

	// Select the sound to play
	$("#hi-hat").click(function(){
		sendBeatCommand("sound hi_hat\n");
	});
	$("#snare").click(function(){
		sendBeatCommand("sound snare\n");
	});
	$("#base").click(function(){
		sendBeatCommand("sound base\n");
	});

	// Setup a repeating function (every 1s)
	window.setInterval(function() {getUpdate()}, 1000);
	handleReplies();
	
});

function sendBeatCommand(message) {
	socket.emit('beat_box', message);
};

function sendRequest(file) {
	console.log("Requesting '" + file + "'");
	socket.emit('proc', file);
}

function getUpdate() {
	// Get the current volume, tempo, mode
	sendBeatCommand("get volume\n");
	sendBeatCommand("get tempo\n");
	sendBeatCommand("get mode\n");

	// Get the uptime
	sendRequest('uptime');
	errorTime = setTimeout(function() {
		$("#error-box").show();
		$("#error-text").text("Server error: No response from the NodeJs server. Is it running?");
		error_origin = NODEJS_ERROR;
	}, 1000);	
}

function replaceAll(str, find, replace) {
	return str.replace(new RegExp(find, 'g'), replace);
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
			case "volume":
				$("#volumeid").val(reply[1]);
				break;
			case "tempo":
				$("#tempoid").val(reply[1]);
				break;
			case "mode":
				var mode_name = convertModeToText(reply[1]);
				$("#modeid").text(mode_name);
				break;
			default:
				break;
		}
	});

	// Handle data coming back from the server
	socket.on('fileContents', function(result) {
		clearTimeout(errorTime);
		if (error_origin == NODEJS_ERROR) {
			console.log("NODEJS ERROR CLEAR");
			// setTimeout(function() {
			// 	$("#error-box").hide();
			// }, 10000);
			$("#error-box").hide();
		}

		var fileName = result.fileName;
		var contents = result.contents;
		console.log("fileContenst callback: fileName " + fileName 
				+ ", contents: " + contents);
		
		// /proc/uptime contains 2 pieces of information: 
		// https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/6/html/deployment_guide/s2-proc-uptime
		// We only need the 1st one so split them up to use
		// the 1st one
		var time = contents.split(" ");

		// How to conver seconds to hours, minutes, seconds: https://www.inchcalculator.com/seconds-to-time-calculator/
		var time_string = "";
		var hour = time[0] / 3600;
		if (hour < 10) {
			time_string += "0";
		}
		time_string += Math.floor(hour) + ":";

		var min = (hour % 1) * 60;
		if (min < 10) {
			time_string += "0";
		}
		time_string += Math.floor(min) + ":";

		var sec = (min % 1) * 60;
		if (sec < 10) {
			time_string += "0";
		}
		time_string += Math.floor(sec);

		// Make linefeeds into <br> tag.		
		// time_string = replaceAll(time_string, "\n", "<br/>");
		$('#uptimeId').html(time_string);
	});

	socket.on("DaError", function(result) {
		$("#error-box").show();
		$("#error-text").text(result.contents);
		error_origin = UDP_ERROR;
	});
}

function convertModeToText(mode_num) {
	switch (mode_num) {
		case "0":
			return "None";
		case "1":
			return "Rock #1";
		case "2":
			return "Rock #2";
		case "3":
			return "RUNNN";
		default:
			mode_num = "1";
	}
}