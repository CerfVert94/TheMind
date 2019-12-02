all:
	gcc -o Mind Mind.c -L/home/sasl/eleves/ei-se/3520402/IoT/MQTT/paho.mqtt.c-master/build/src/ -lpaho-mqtt3c-static -I /home/sasl/eleves/ei-se/3520402/IoT/MQTT/paho.mqtt.c-master/src/ -lpthread
