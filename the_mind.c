#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "MQTTClient.h"
#define ADDRESS          "tcp://localhost:1883"
#define CLIENTID         "TheMind"
#define TOPIC_MIND       "TheMind"
#define TOPIC_JOUEUR     "Joueur"
#define QOS              1
#define TIMEOUT         500L
#define TX_BUFFER_LEN   128
#define MAX_JOUEUR      4

MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;

volatile MQTTClient_deliveryToken deliveredtoken;
int compteur=0;
int compteur_joueur = 0;
int cartes[101];
void parseJoueur(char * tx_buffer, MQTTClient_message *message, int compteur_joueur);
void parseJoueur(char * tx_buffer, MQTTClient_message *message, int compteur_joueur) 
{
    snprintf(tx_buffer, TX_BUFFER_LEN, "%s %d", (char*)message->payload, compteur_joueur);
}
void melangerPaquet()
{
	int i;

	srand(time(NULL));
	for (i=1;i<101;i++)
		cartes[i]=i;

	for (i=0;i<10000;i++)
	{
		int r1=rand()%100+1;
		int r2=rand()%100+1;

		int tmp;

		tmp=cartes[r1];
		cartes[r1]=cartes[r2];
		cartes[r2]=tmp;
	}
}

void afficherPaquet()
{
	int i;

	for (i=1;i<101;i++)
		printf("%d ",cartes[i]);
	printf("\n");
}

int sendMessage()
{
    int rc;

    //pubmsg.payload = PAYLOAD;
    //pubmsg.payloadlen = strlen(PAYLOAD);
	//
		pubmsg.payload = "testenvoi\n";
		pubmsg.payloadlen = strlen("testenvoi\n");
	//
	
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
	///
		 //MQTTClient_publishMessage(client, TOPICout, &pubmsg, &token);
	///
    //MQTTClient_publishMessage(client, TOPICout, &pubmsg, &token);
/*
    printf("Waiting for up to %d seconds for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            (int)(TIMEOUT/1000), PAYLOAD, TOPICout, CLIENTID);
*/
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);
}

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}


void print_payload(MQTTClient_message *message){
    int i = 0;
    char* payloadptr;
    payloadptr = message->payload;
    for(i=0; i<message->payloadlen; i++)
        putchar(*payloadptr++);
    putchar('\n');
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char tx_buffer[128];
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    print_payload(message);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    if (compteur_joueur < MAX_JOUEUR) {
        parseJoueur(tx_buffer, message, compteur_joueur++);

    }
    compteur++;
	if (compteur==5)
	{
		sendMessage();
		puts("compteur=5");
		compteur=0;
	}

    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[])
{
    int rc;
    int ch;

    melangerPaquet();
    afficherPaquet();

    compteur=0;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 200;
    conn_opts.cleansession = 1;
    //conn_opts.username="username";
    //conn_opts.password="password";
    //MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, NULL);
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
	
/*
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
*/
	
    MQTTClient_subscribe(client, "Mind", QOS);
	do
    {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
