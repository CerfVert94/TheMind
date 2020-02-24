var mosca = require('mosca');
var settings = {
		port:1883
		}

var server = new mosca.Server(settings);


server.on('ready', function(){
    console.log("ready");

    var mqtt = require('mqtt');
    var client  = mqtt.connect('mqtt://localhost');
    client.on('connect', function () {
	client.subscribe("#");
        /*client.subscribe(sub_topic1);
        client.subscribe(sub_topic2);
        client.subscribe(sub_topic3);
        client.subscribe(pub_topic);*/
    });
    client.on('message', function (topic, message) {
	    context = message.toString();
	    console.log(topic.toString() + " : " + context);
    });
 /*
    client.on('message', function (topic, message) {
        context = message.toString();
        
        if (topic == sub_topic1 ||
            topic == sub_topic2 ||
            topic == sub_topic3 ) {
            let value = parseFloat(message);
            let json_payload = "{\"" + topic + "\" : " + value + "}";

            if (topic == sub_topic3) {
                if (value > 2000) 
                    client.publish(pub_topic, "gyro_on");
                else
                    client.publish(pub_topic, "gyro_off");
            }
        
            // Temporary solution to bypass http response 302 problem 
            send_payload2ubidot(json_payload);
            
        }
        console.log(topic.toString() + " : " +context)
    });*/
});
