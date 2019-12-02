#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#define ADDRESS     "tcp://localhost:1883"
#define JEUID    "ExampleJeuPub"
#define JOUEURID  "ExampleJoueurPub"
#define TOPICJEU       "TheMind"
#define TOPICJOUEUR "Joueur"
#define QOS         0
#define TIMEOUT     10000L


volatile MQTTClient_deliveryToken deliveredtoken;
int compteur=0;
MQTTClient Joueur;
MQTTClient TheMind;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;




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
    rc = MQTTClient_waitForCompletion(Joueur, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);
}


int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = message->payload;
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    putchar('\n');
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
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

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}


int main(int argc, char* argv[])
{

    if(argc < 2){
      fprintf(stderr,"Type your name\n");
      exit(0);
    }


    int rc;
    int ch;

    compteur=0;

    MQTTClient_create(&Joueur, ADDRESS, JOUEURID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 200;
    conn_opts.cleansession = 1;
    //conn_opts.username="username";
    //conn_opts.password="password";
    //MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, NULL);
    MQTTClient_setCallbacks(Joueur, NULL, connlost, msgarrvd, delivered);
    if ((rc = MQTTClient_connect(Joueur, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    printf("\nSubscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPICJOUEUR, JOUEURID, QOS);

    MQTTClient_subscribe(Joueur, TOPICJOUEUR, QOS);





    MQTTClient_create(&TheMind, ADDRESS, JEUID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTClient_connect(TheMind, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    pubmsg.payload = argv[1];
    pubmsg.payloadlen = strlen(argv[1]);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(TheMind, TOPICJEU, &pubmsg, &token);
    printf("Waiting for up to %d seconds for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            (int)(TIMEOUT/1000), argv[1], TOPICJEU, JEUID);
    rc = MQTTClient_waitForCompletion(TheMind, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);
    do
    {
          ch = getchar();
    } while(ch!='Q' && ch != 'q');
    MQTTClient_disconnect(TheMind, 10000);
    MQTTClient_destroy(&TheMind);
    MQTTClient_disconnect(Joueur, 10000);
    MQTTClient_destroy(&Joueur);
    return rc;
}
