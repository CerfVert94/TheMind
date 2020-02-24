var mqtt;
var reconnectTimeout = 2000;
var host="127.0.0.1"; //change this
var port=9000;
var pub_message;
var href;
// let myName = null;
function onConnect() {
    // Once a connection has been made, print to the console.
    console.log("Connected ");
    // 
    connect2TheMind(myName);
    // connect2TheMind(href);
}

function MQTTconnect() {
    console.log("connecting to "+ host +" "+ port);
    myCID = Math.floor(Math.random() * 65535);
    mqtt = new Paho.MQTT.Client(host, port, myCID.toString());
    //document.write("connecting to "+ host);
    var options = {
        timeout: 3,
        onSuccess: onConnect,
        
        };
    mqtt.connect(options); //connect
}

let notice_topic = "Player/Notice";
function connect2TheMind(name) {
    let pub_message;
    
    mqtt.subscribe(name);
    mqtt.subscribe(notice_topic);
    pub_message                 = new Paho.MQTT.Message(name);
    pub_message.destinationName = "TheMind";
    mqtt.send(pub_message);
    mqtt.onMessageArrived = onMessageArrived;
    header.innerHTML = myName + " (" + myID.toString() + " : " + myCID.toString() + ")";
};

function parseMessage(destinationName, payloadString) {
    if (destinationName == notice_topic) {
        alert(payloadString);
        return;
    }
    
    let id       = parseID(destinationName),
        msg_type, data;

    if (id != myID) return;



    payloadString = parseSubMessage(payloadString)
    console.log("ID : " + id.toString());
    msg_type = payloadString['msg_type'];
    data     = payloadString['data'];
    // console.log(msg_type, data);

    if (msg_type == 'L') {
        nb_lives = data[0].toString();
    }
    else if (msg_type == 'S') {
        nb_shrks = data[0].toString();
    }
    else if (msg_type == 'N') {
        played_cards    = []
        discarded_cards = []
    }
    else if (msg_type == 'H') {
        let cnt = 0, i = 0;
        cnt = 0;
        hand = [];
        for (i = 0; i < data.length; i++) {
            if (data[i] != 0) {
                hand[cnt] = parseInt(data[i]);
                cnt += 1;
            }
        }
    }
    else if (msg_type == 'P') {
        // voting = true;

        if (data[0] == -1) {
            played_cards.pop();
        }
        else {
            played_cards[played_cards.length] = data[0];
        }
        // console.log("Played :", played_cards);
    }
    else if (msg_type == 'D') {
        // voting = true;
        discarded_cards[discarded_cards.length] = data[0];
        // console.log("Played :", played_cards);
    }
    else if (msg_type == 'V') {
        voting = true;
    }
    
}

function onMessageArrived(msg){
    out_msg=
    console.log("Topic   received : " + msg.destinationName);
    console.log("Message received : " + msg.payloadString);
    if (!connected) {
        if (msg.destinationName == myName) {
            let stopic;
            myID = parseInt(msg.payloadString);
            console.log("Received myID : ", myID);
            connected = true;
            stopic = "Player/" + myID.toString();
            console.log("Subscribe : ", stopic);
            mqtt.subscribe(stopic);

            header.innerHTML = myName + " (" + myID.toString() + " : " + myCID.toString() + ")";
        }
    }
    else {
        parseMessage(msg.destinationName, msg.payloadString);
    }
}
function playLowestCard() {
    let pub_message             = new Paho.MQTT.Message("P" + myID.toString() + " C 0");
    pub_message.destinationName = "Host";
    mqtt.send(pub_message);
}

function voteNo() {
    let pub_message             = new Paho.MQTT.Message("P" + myID.toString() + " V 1");
    pub_message.destinationName = "Host";
    mqtt.send(pub_message);
    voting = false;

}

function voteYes() {
    let pub_message             = new Paho.MQTT.Message("P" + myID.toString() + " V 2");
    pub_message.destinationName = "Host";
    mqtt.send(pub_message);
    voting = false;

}

function useShuriken() {
    let pub_message             = new Paho.MQTT.Message("P" + myID.toString() + " S");
    pub_message.destinationName = "Host";
    mqtt.send(pub_message);

}


