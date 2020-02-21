#include <mosquitto.h>
#include <mqtt_callback.h>
#include <client_shared.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <mymacro.h>
#include <TheMind.h>

#define STATUS_CONNECTING 0
#define STATUS_CONNACK_RECVD 1
#define STATUS_WAITING 2
#define MAX_SUB_TOPIC 20
#define MAX_PUB_TOPIC 20
#define _LOG(str) "[MQTT] "str"\n"

static int mode = MSGMODE_NONE;
static int last_mid = -1;
static int last_mid_sent = -1;
static bool disconnect_sent = false;
static int  g_nb_sub_topics = 0;
static int  g_nb_pub_topics = 0;

struct subscription{
	char topic[TOPIC_LEN];
	subscription_func on_subscribe;
}subscription;

struct publication{
	char topic[TOPIC_LEN];
	char message[MESSAGE_LEN];
}publication;

struct subscription sub[MAX_SUB_TOPIC];
struct publication  pub[MAX_PUB_TOPIC];

int mid = 100;
void subscribe(struct mosquitto *mosq, char *topic, subscription_func on_subscribe, int qos){
	if (g_nb_sub_topics > MAX_SUB_TOPIC - 1) {
		EXIT_WITH_ERROR_MESSAGE("The number of subscribe topic cannot exceed %d", MAX_SUB_TOPIC);
	}
	strncpy(sub[g_nb_sub_topics].topic, topic, strlen(topic));
	sub[g_nb_sub_topics].on_subscribe = on_subscribe;
	mosquitto_subscribe(mosq, NULL, topic, qos);
	g_nb_sub_topics++;
}
void publish(struct mosquitto *mosq, char * pub_topic   , char *message, int qos)
{
	mosquitto_publish(mosq, NULL, pub_topic, strlen(message), message, qos, false);
}

void add_publish_topic(char *topic){
	if (g_nb_pub_topics > MAX_PUB_TOPIC - 1) {
		EXIT_WITH_ERROR_MESSAGE("The number of publish topic cannot exceed %d", MAX_SUB_TOPIC);
	}
	strncpy(pub[g_nb_pub_topics++].topic, topic, strlen(topic));
}
void print_publish_topic()
{
	for (int i = 0 ; i < g_nb_pub_topics; i++) {
		printf("Pub Topic %d : %s\n",i , pub[i].topic);
	}
}
void print_subscribe_topic()
{
	for (int i = 0 ; i < g_nb_sub_topics; i++) {
		printf("Sub Topic %d : %s\n",i , sub[i].topic);
	}
}

void publish_from_topic_id(struct mosquitto *mosq, int    pub_topic_id, char *message, int qos)
{
	mosquitto_publish(mosq, NULL, pub[pub_topic_id].topic, strlen(message), message, qos, false);
}
void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
	int i = 0;
	if(message->payloadlen) {

		printf("Topic   : %s\n", message->topic);
		printf("Message : %s\n", message->payload);
		for (i = 0; i < g_nb_sub_topics; i++) {
			if (!strncmp(message->topic, sub[i].topic, strlen(message->topic))) {

				// printf("[%d]Topic   : %s\n", i, sub[i].topic);
				sub[i].on_subscribe(message->payload);
			}
		}
				
	}else{
	}
}

void my_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
	int i;
	if(!result){
		fprintf(stdout, _LOG("Connected to MQTT broker"));
		for (i = 0; i < g_nb_sub_topics; i++)
			mosquitto_subscribe(mosq, NULL, sub[i].topic, 0);
	}
	else{
		fprintf(stderr, _LOG("Connect to MQTT broker failed"));
	}
}

void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
}

void my_publish_callback(struct mosquitto *mosq, void *obj, int mid)
{
	last_mid_sent = mid;
	// fprintf(stderr, _LOG("Published : %d"), mid);
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
