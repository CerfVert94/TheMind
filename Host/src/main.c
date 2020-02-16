
#include <stdio.h>
#include <string.h>
#include <mosquitto.h>
#include <mqtt_callback.h>
#include <TheMind.h>


int mqtt() 
{
    char *host = "localhost";
	int port = 1883;
	int keepalive = 60;
	bool clean_session = true;
	struct mosquitto *mosq = NULL;

	mosquitto_lib_init();

	mosq = mosquitto_new(NULL, clean_session, NULL);
	if(!mosq){
		fprintf(stderr, "Error: Out of memory.\n");
		return 1;
	}
	mosquitto_log_callback_set(      mosq, my_log_callback);
	mosquitto_connect_callback_set(  mosq, my_connect_callback);
	mosquitto_message_callback_set(  mosq, my_message_callback);
	mosquitto_subscribe_callback_set(mosq, my_subscribe_callback);
	mosquitto_publish_callback_set(  mosq, my_publish_callback);


	if(mosquitto_connect(mosq, host, port, keepalive)){
		fprintf(stderr, "Unable to connect.\n");
		return 1;
	}

	mosquitto_loop_forever(mosq, -1, 1);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
}
int main(int argc, char *argv[])
{
  
	return 0;
}