"use strict";
/*
 * Respond to commands over a websocket to relay UDP commands to a local program
 */

var socketio = require('socket.io');
var io;

// Adopted from: https://stackoverflow.com/a/51084163
// const exec = require('child_process').exec;
// const isRunning = (query, cb) => {
//     let platform = process.platform;
//     let cmd = '';
//     switch (platform) {
//         case 'win32' : cmd = `tasklist`; break;
//         case 'darwin' : cmd = `ps -ax | grep ${query}`; break;
//         case 'linux' : cmd = `ps -A`; break;
//         default: break;
//     }
//     exec(cmd, (err, stdout, stderr) => {
//         cb(stdout.toLowerCase().indexOf(query.toLowerCase()) > -1);
//     });
// }

var dgram = require('dgram');

// Include the module to work with filesystem
// var fs   = require('fs');

exports.listen = function(server) {
	io = socketio.listen(server);
	io.set('log level 1');

	io.sockets.on('connection', function(socket) {
		handleCommand(socket);
	});
};

function handleCommand(socket) {
	// Pased string of comamnd to relay
	socket.on('plant-care', function(data) {
		console.log('plant-care: ' + data);

		// Adopted from: https://stackoverflow.com/a/51084163
		// isRunning('beat-box', (status) => {
		// 	if (!status) {
		// 		emitDaError(socket, "UDP", 
		// 			"Server Error: No response from the C beat-box applicaiton. Is it running?");
		// 	}
		// })

		// Info for connecting to the local process via UDP
		var PORT = 12345;
		var HOST = '192.168.7.2';
		var buffer = new Buffer(data);

		var client = dgram.createSocket('udp4');
		client.send(buffer, 0, buffer.length, PORT, HOST, function(err, bytes) {
			if (err) 
				throw err;
			
			console.log('UDP message sent to ' + HOST +':'+ PORT);
		});

		client.on('listening', function () {
			var address = client.address();
			console.log('UDP Client: listening on ' + address.address + ":" + address.port);
		});
		// Handle an incoming message over the UDP from the local application.
		client.on('message', function (message, remote) {
			console.log("UDP Client: message Rx" + remote.address + ':' + remote.port +' - ' + message);

			var reply = message.toString('utf8')
			var replyChunk = reply.split(" ");
			if (reply[0] === "Invalid") {
				emitDaError(socket, "UDP", 
					"Invalid reply from the application!");
			} else {
				socket.emit('commandReply', reply);
			}

			client.close();

		});
		client.on("UDP Client: close", function() {
			console.log("closed");
		});
		client.on("UDP Client: error", function(err) {
			console.log("error: ",err);
		});
	});
};

function emitSocketData(socket, fileName, contents) {
	var result = {
			fileName: fileName,
			contents: contents
	}
	socket.emit('fileContents', result);	
}

function emitDaError(socket, fileName, contents) {
	var result = {
		fileName: fileName,
		contents: contents
	}
	socket.emit('DaError', result);	
}
