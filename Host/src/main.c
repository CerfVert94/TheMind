
#include <stdio.h>
#include <string.h>
#include <mosquitto.h>
#include <TheMind.h>
#include <client_shared.h>

#define STATUS_CONNECTING 0
#define STATUS_CONNACK_RECVD 1
#define STATUS_WAITING 2
static int mode = MSGMODE_NONE;
static int last_mid = -1;
static int last_mid_sent = -1;
static bool disconnect_sent = false;

#define _LOG(str) "[MQTT] "str"\n"

char payload[128];
char text[128];
char sessionId[128];

int mid = 100;
void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
	if(message->payloadlen) {
	    fprintf(stderr, "%s\n",(char*) message->payload);		
	}else{
	}
}

void my_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
	if(!result){
		fprintf(stdout, _LOG("Connected to MQTT broker"));
		mosquitto_subscribe(mosq, NULL, "#", 0);
	}
	else{
		fprintf(stderr, _LOG("Connect to MQTT broker failed"));
	}
}

void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
	int i;

	//printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
	for(i=1; i<qos_count; i++){
		//printf(", %d", granted_qos[i]);
	}
//	printf("\n");
}

void my_publish_callback(struct mosquitto *mosq, void *obj, int mid)
{
	last_mid_sent = mid;
	fprintf(stderr, _LOG("Published : %d"), mid);
	if(mode == MSGMODE_STDIN_LINE){
		if(mid == last_mid){
			mosquitto_disconnect(mosq);
			disconnect_sent = true;
		}
	}else if(disconnect_sent == false){
		mosquitto_disconnect(mosq);
		disconnect_sent = true;
	}
}

void my_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
	/* Pring all log messages regardless of level. */
	//printf("%s\n", str);
}

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