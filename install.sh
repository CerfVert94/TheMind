#!/bin/sh
git clone https://github.com/eclipse/mosquitto
mkdir mqtt_lib 
cd mqtt_lib
cmake ../mosquitto 
make 
cd ..
cp mosquitto/lib/mosquitto.h        Host/inc/
cp mosquitto/client/client_shared.h Host/inc/
cd Host
make
cd ..
cd MqttBroker_Server
./install.sh
cd ..