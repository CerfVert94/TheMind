all:
	gcc -o the_mind the_mind.c -L./paho.mqtt.c-master/build/src/ -lpaho-mqtt3c-static -I./paho.mqtt.c-master/src/ -lpthread
