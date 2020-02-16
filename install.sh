#!/bin/sh
git clone https://github.com/eclipse/mosquitto
mkdir mqtt_lib 
cd mqtt_lib
cmake ../mosquitto
make
cd ..
cp mosquitto/lib/mosquitto.h include/
cp mosquitto/client/client_shared.h include/
