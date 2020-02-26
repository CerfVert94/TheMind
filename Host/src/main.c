
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <mosquitto.h>
#include <mqtt_callback.h>
#include <TheMind.h>
#include <pthread.h>
#include <mymacro.h>

static struct mosquitto *mosq = NULL;
static int g_max_nb_players = 4;
static int g_nb_players = 0;
static bool initialize_game = false;
void registration(char *message);


void registration(char *message)
{
	int id;
	char *player_name, pub_message[MESSAGE_LEN], pub_topic[TOPIC_LEN];
	if (g_nb_players < g_max_nb_players) {
		printf("Max     number of players : %d\n", g_max_nb_players);
		printf("Current number of players : %d\n", ++g_nb_players);
		printf("              Player name : %s\n", message);
		player_name = message;
		id = add_player(player_name);
		snprintf(pub_message, MESSAGE_LEN,"%d", id);
		snprintf(pub_topic, TOPIC_LEN, PUB_PLAYER_TOPIC("%d"), id);
		
		// Send player id to the joined client
		publish(mosq, player_name, pub_message, 2);
		
		if (g_nb_players >= g_max_nb_players) 
			initialize_game = true;
		
	}

}
struct com_info{
	char *address;
	int port;
}com_info;
void *start_mqtt(void *parameter) 
{
    char *host = ((struct com_info*)parameter)->address;
	int port = ((struct com_info*)parameter)->port;
	int keepalive = 60;
	bool clean_session = true;

	mosquitto_lib_init();

	mosq = mosquitto_new(NULL, clean_session, NULL);
	if(!mosq){
		fprintf(stderr, "Error: Out of memory.\n");
		exit(EXIT_FAILURE);
		// return 1;
	}
	subscribe(mosq, "TheMind", registration, 0);
	mosquitto_log_callback_set(      mosq, my_log_callback);
	mosquitto_connect_callback_set(  mosq, my_connect_callback);
	mosquitto_message_callback_set(  mosq, my_message_callback);
	mosquitto_subscribe_callback_set(mosq, my_subscribe_callback);
	mosquitto_publish_callback_set(  mosq, my_publish_callback);


	if(mosquitto_connect(mosq, host, port, keepalive)){
		fprintf(stderr, "Unable to connect.\n");
		exit(EXIT_FAILURE);
	}
	

	mosquitto_loop_forever(mosq, -1, 1);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
}
// extern int opterr, optopt;
// extern char *optarg;
bool start_game(struct mosquitto *mosq) {

	int *hand;
	char message[MESSAGE_LEN], topic[TOPIC_LEN];
	if (g_nb_players < g_max_nb_players) {
		printf("Waiting for player %d\n", g_nb_players);
		sleep(1);
		return true;
	}
	else if (initialize_game) {
		initialize_game = false;
		printf("All player joined\n");
		set_mosquitto(mosq);
		init_game(100);
		subscribe(mosq, SUB_HOST_TOPIC, message_from_player, 2);
	}
	return false;

}
int main(int argc, char *argv[])
{
	struct com_info param = {.address = "localhost", .port = 1883};
	int nb_players = 4;
	int c;

	opterr = 0;
	while ((c = getopt (argc, argv, "p:n:h:")) != -1) {
		switch (c)
		{
		case 'p':
		param.port = atoi(optarg);
		break;

		case 'h':
		param.address = optarg;
		break;

		case 'n':
		nb_players = atoi(optarg);
		break;

		default:
		printf("Usage : %s -h [HOST ADDRESS] -p [HOST PORT] -n [NUMBER OF PLAYERS]\n", argv[0]);
		return 1;
		}
	}

	pthread_t mqtt_thread;
	g_max_nb_players = nb_players;
	pthread_create(&mqtt_thread, NULL, start_mqtt, &param);
	// printf("%s %d / %d\n", address, port, nb_players);
	while(start_game(mosq));
	printf("Start : TheMind\n");

	while(1);
	return 0;
}