#!/bin/sh
git clone https://github.com/eclipse/mosquitto
mkdir mqtt_lib 
cd mqtt_lib
cmake ../mosquitto 
make 
cd ..
cp mosquitto/lib/mosquitto.h        Host/inc/
cp mosquitto/client/client_shared.h Host/inc/
