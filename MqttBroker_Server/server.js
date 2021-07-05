/**************************************************
** NODE.JS REQUIREMENTS
**************************************************/
var util = require("util"),
    express = require("express"),
    app = express(),
    http = require("http"),
    // sq = require('sqlite3'),
    mosca = require('mosca'),
    io = require("socket.io")(http);                              // Socket.IO

/**************************************************
** GAME VARIABLES
**************************************************/
var socket;             // Socket controller
// var db = new sq.Database(__dirname + '/messages.db3');

var mess = "+";
var sclient;
var prot = 0;
var mqtt_server_init = false;

function retrieveDatabase() {
    // console.log("------------------------");
    // db.each("SELECT * FROM message", function(err, row) {
    // if (err) {
    //         console.log(err);
    // } else {
    //         console.log(row.nom + " a écrit '" + row.content + "'");
    // }
    // });
    // mess+="+";
    // if (prot==1)
    // 	//sclient.emit('message',mess);
    // 	//sclient.broadcast.emit('message',mess);
    // 	io.sockets.emit('message',mess);
}


/**************************************************
** MQTT INITIALISATION
**************************************************/
function init_mqtt_server() {
    let server = new mosca.Server(
        {
            http: {
                port: 9000,
                bundle: true,
                static: './public'
            },
            port: 1883
        });
    server.on('ready', function () {
        console.log("[MQTT Broker] Server ready");
        let mqtt = require('mqtt');
        let client = mqtt.connect('mqtt://localhost');

        mqtt_server_init = true;
        client.on('connect', function () {
            client.subscribe("#");
        });
        client.on('message', function (topic, message) {
            context = message.toString();
            console.log(topic.toString() + " : " + context);
        });
        init_themind();
    });
}

/**************************************************
** GAME INITIALISATION
**************************************************/
function init_themind() {
    const { exec } = require('child_process');

    let options = {
        timeout: 0,
        stdio: 'inherit',
        shell: true,
    }

    exec('LD_LIBRARY_PATH=../mqtt_lib/lib ../Host/exe/Host -p 1883 -h localhost -n 4', options, (error, stdout, stderr) => {
        // console.log('stdout:', stdout);
        // console.log('stderr:', stderr);
        if (error) {
            console.error('exec error:', error);
            return;
        }
    });
}

function init() {
    // Create an empty array to store players

    server = http.createServer(app);
    // Set up Socket.IO to listen on port 8000
    socket = io.listen(server);
    server.listen(8000);
    app.use(express.static(__dirname + "/public"));

    // Start listening for events
    io.on("connection", onSocketConnection);
    console.log("server.js started");
    console.log("Open the following links. (Name query can be modifiable but don't delete it.)");
    console.log("http://localhost:8000/index.html?name=User1");
    console.log("http://localhost:8000/index.html?name=User2");
    console.log("http://localhost:8000/index.html?name=User3");
    console.log("http://localhost:8000/index.html?name=User4");
};

// New socket connection
function onSocketConnection(client) {

    console.log("New player has connected: " + client.constructor.name);
    console.log("New player has connected: " + client.id);

    //client.emit('message', mess);
    sclient = client;
    prot = 1;
};

init_mqtt_server();
init();