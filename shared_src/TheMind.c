#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <vote.h>
#include <mosquitto.h>
#include <mqtt_callback.h>
#include <TheMind.h>
#include <mymacro.h>
#include <host.h>

struct player_info{
    int  id;
    char name[NAME_LEN];
    int  hand[MAX_LEVEL];
}player_info;


static struct player_info g_players[MAX_PLAYER];
static int  g_round       = 0;
static int  g_level      = 0;
static int  g_lives      = 0;
static int  g_shuriken   = 0;
static int  g_nb_players = 0;

static int  g_deck[DECK_SIZE];
static int  g_deck_top   = 0; // Points to the last index of g_deck (the beyond become irrelevant)

static int  g_played[DECK_SIZE];
static int  g_played_top = 0; // Points to the last index of g_played

static int  g_answer[DECK_SIZE];
static struct mosquitto *g_mosq;

static int g_nb_shuffle = 100;
/************************************************/
/****************** GAME PLAY *******************/
int init_game(int nb_shuffle) {
    int i, j;
    // Seed random
    srand((unsigned)time(NULL));
    
    // Create a new deck / initialize hands
    create_deck();

    //Set intial level to 1
    set_level(4);
    //Set intial initial lives
    // set_lives(get_nb_players());
    set_lives(100);

    
    // Set global number of shuffle.
    g_nb_shuffle = nb_shuffle;
    start_new_round();

    print_current_status();	
}

int  end_game()
{
    printf("end game\n");
    exit(EXIT_SUCCESS);
}
void start_new_round() {
    int i = 0, j = 0;
    for (i = 0; i < g_nb_players; i++) 
        for (j = 0; j < cards_per_level(); j++)
            g_players[i].hand[j] = 0;
        
    for (i = 0; i < g_played_top; i++) {
        g_played[i] = 0;
    }
    g_played_top = 0;
    //Set round to 0
    initialize_round();
    
    // Distribute cards
    distribute_cards(g_nb_shuffle);

    // Publish hands to clients
    publish_hands();
}

bool check_answer() {
    for (int i = 0; i < g_played_top; i++) {
        if (g_played[i] == 0)
            continue;
        if (g_played[i] != g_answer[i])
            return false;
    }
    return true;
}
void play_card(int *hand, int card)
{
    for (int i = 0; i < g_played_top + 1; i++) {
        if (g_played[i] == 0) {
            move_card(g_played, i, hand, card);
            g_played_top++;
            break;
        }
    }
}
void discard_card(int *hand, int card) {
    for (int j = 0; j < g_deck_top; j++) {
        if (hand[card] == g_answer[j]) {
            move_card(g_played, j, hand, card);
            g_played_top++;
            return;
        }
    }
}
/************************************************/

/************************************************/
/*************** MESSAGE CALLBACK ***************/
void message_from_player(char *message) {
    int id       = parse_player_id(message);
    int msg_type = parse_message_type(message);
    int card, response, final_count, wrong_card;

    if (!is_alive()) {
        return;
    }

    printf("Message from Player (id : %d / type : %d)\n", id, msg_type);
    

    switch (msg_type) {
        case MSG_TYPE_CARD: 
        if (is_vote_called() || g_played_top == g_deck_top)
            return;
        card = parse_card(message);
        printf("Received card %d from Player %d\n", g_players[id].hand[card], id);
        
        play_card(g_players[id].hand, card);
        break;

        case MSG_TYPE_VOTE: 
        
        if (!is_vote_called())
            return;

        response = parse_response(message);
        vote(id, response);
        printf("Received ballot from Player %d (%d)\n", id, response);

        final_count = ballot_count();
        if      (final_count == VOTE_NO) 
            printf("Motion denied.\n");
        else if (final_count == VOTE_YES) {
            printf("Agreed upon shuriken.\n");
            printf("Discard the lowest cards from each hand\n");
            discard_lowest();
        }
        break;

        case MSG_TYPE_SHURIKEN: 
        if (is_vote_called())
            return;
        printf("Call a vote for a use of shuriken\n");
        init_vote(g_nb_players);
        
        break;
    }
    next_round();
    if (g_played_top < g_deck_top){
        if (!check_answer()){
            decrement_life();
            move_card(g_players[id].hand, card, g_played, --g_played_top);
            printf("Wrong card.\n");
            printf("Life(s) : %d\n", get_lives());
            printf("Discard all cards lower than %d.\n", g_players[id].hand[card]);
            discard_lower(g_players[id].hand[card]);
        }
    }
    if (check_answer() && get_level() <= MAX_LEVEL && g_played_top == g_deck_top){
        printf("Level up !\n");
        printf("Start the new round!\n");
        increment_level();
        if(get_level() > MAX_LEVEL){ 
            end_game();
        }
        else {
            start_new_round();
        }
    }

    for (int i = 0; i < g_nb_players; i++) 
        pack_hand(g_players[i].hand, cards_per_level());
    print_current_status();
}
/************************************************/

/************************************************/
/********************* DECK *********************/
void create_deck() 
{
    for (int i = 1; i <= DECK_SIZE; i++) 
        g_deck[i - 1] = i;
}

void shuffle_deck(int *deck, int nb_shuffle)
{
    int i, j, cnt = 0, tmp;
    g_deck_top = 0;

    while (cnt < nb_shuffle) {
        do {
            i = rand() % DECK_SIZE;
            j = rand() % DECK_SIZE;
        }while (i == j);
        tmp = deck[i];
        deck[i] = deck[j];
        deck[j] = tmp;
        cnt++;
    }
}
int sort(int *cards, int size){
    int tmp; 
    // printf ("size : %d\n", size);
    for (int j = 0; j < size; j++) {
        for (int i = 1; i < size; i++) {
            if (cards[i - 1] > cards[i]) {
                tmp = cards[i - 1];
                cards[i - 1] = cards[i + 0];
                cards[i + 0] = tmp;
                // printf("[%d] %d <=> %d \n", i, cards[i - 1], cards[i + 0]);

            }
        }
    }
}
void distribute_cards(int nb_shuffles)
{
    int i = 0, j = 0, nb_cards = cards_per_level();
    create_deck();
    shuffle_deck(g_deck, nb_shuffles);

    for (i = 0; i < nb_cards; i++) {
        for (j = 0; j < g_nb_players; j++) {
            move_card(g_players[j].hand,          i, g_deck           , g_deck_top);
            copy_card(g_answer         , g_deck_top, g_players[j].hand, i         );
            g_deck_top++;
        }
    }
    for (j = 0; j < g_nb_players; j++) {
        sort(g_players[j].hand, nb_cards);
    }
    sort(g_answer, g_deck_top);
}

void initialize_played_cards()
{
    for (int i = 0; i < DECK_SIZE; i++) 
        g_played[i] = 0;
    g_played_top = 0;

}
void pack_hand(int *hand, int size) {
    int i = 0;
    for (i = 0; i < size - 1; i++) {
        if (hand[i] == 0) 
            memcpy(&hand[i], &hand[i + 1], sizeof(int) * (size - i));
    }
}
void move_card(int *dst, int dst_index, int *src, int src_index) {
    dst[dst_index] = src[src_index];
    src[src_index] = 0;
}
void copy_card(int *dst, int dst_index, int *src, int src_index) {
    dst[dst_index] = src[src_index];
}
void exchange_card(int *dst, int dst_index, int *src, int src_index) {
    int tmp;
    tmp            = dst[dst_index];
    dst[dst_index] = src[src_index];
    src[src_index] = tmp;
}
/************************************************/


/************************************************/
/******************** PLAYER ********************/
void set_nb_players(int nb_players) {
    g_nb_players = nb_players;
}
int get_nb_players(void) {
    return g_nb_players;
}

struct player_info get_player(int i) {
    if (i > MAX_PLAYER - 1) {
        EXIT_WITH_ERROR_MESSAGE("The number of player does not exceed %d", MAX_PLAYER);
    }
    struct player_info player = {.id = g_players[i].id};
    memcpy(g_players[i].name, g_players[i].name, NAME_LEN * sizeof(char));
                                       
    return player;
}
int add_player(char *name) {
    if (g_nb_players > MAX_PLAYER - 1) {
        EXIT_WITH_ERROR_MESSAGE("The number of player cannot exceed %d", MAX_PLAYER);
    }
    g_players[g_nb_players].id = g_nb_players;
    memcpy(g_players[g_nb_players].name, name, NAME_LEN * sizeof(char));
    return g_players[g_nb_players++].id;
}
/************************************************/

/************************************************/
/********************* LIFE *********************/
int get_lives(void) {
    return g_lives;
}
void set_lives(int lives) {
    g_lives = lives;
}
void decrement_life(void) {
    g_lives--;
}
void increment_life(void) {
    g_lives++;
}
bool is_alive(void) {
    return g_lives >= 0;
} 
/************************************************/

/************************************************/
/******************** LEVEL *********************/
void decrement_level(void) {
    g_level--;
}
void increment_level(void) {
    g_level++;
}
void set_level(int level) {
    g_level = level;

    for (int i = 0; i < MAX_LEVEL; i++) {
        for (int j = 0; j < MAX_PLAYER; j++) {
            g_players[j].hand[i] = 0;
        }
    }

    g_played_top = 0;
}

int get_level(void) {
    return g_level;
}
/************************************************/


/************************************************/
/******************* SHURIKEN *******************/
void set_shurikens(int shuriken) {
    g_shuriken = shuriken;
}
int  get_shurikens(void) {
    return g_shuriken;
}
void decrement_shuriken(void) {
    g_shuriken--;
}
void increment_shuriken(void) {
    g_shuriken++;
}
bool is_shuriken_available(void) {
    return g_shuriken > 0;
}
/************************************************/

/************************************************/
/********************* TURN *********************/
void initialize_round() {
    g_round = 0;
}
void next_round() {
    g_round++;
}
/************************************************/


/************************************************/
/******************** DEBUG *********************/
void print_current_status() {
    int i, j, k, start, end;
    printf("Current number of players : %d\n", g_nb_players);
    printf("Current level             : %d\n", g_level);
    printf("Current lives             : %d\n", g_lives);
    printf("Entire deck (top %d) : \n", g_deck_top);
    for (j = 0; j < 4; j++) {
        start = (DECK_SIZE / 4) * (j + 0);
        end   = (DECK_SIZE / 4) * (j + 1);
        printf("Index : ", i);
        for (i = start; i < end; i++) {
            printf("%4d", i);
        }
        printf("\n");
        printf("Deck  : ", i);
        for (i = start; i < end; i++) {
            printf("%4d", g_deck[i]);
        }
        printf("\n\n");
    }

    for (i = 0; i < MAX_PLAYER; i++) {
        printf("ID : %d / NAME : %s\n", g_players[i].id, g_players[i].name);
    }
    
    for (k = 0; k < MAX_PLAYER; k++) {
        printf("The hand of player %d : \n", g_players[k].id);
        for (i = 0; i < MAX_LEVEL; i++) {
            printf("%4d", g_players[k].hand[i]);
        }
        printf("\n");
    }
    
    printf("Played  : ");
    for (i = 0; i < DECK_SIZE; i++) {
        printf("%4d", g_played[i]);
    }
    printf("\n\n");

    for (j = 0; j < 4; j++) {
        start = (DECK_SIZE / 4) * (j + 0);
        end   = (DECK_SIZE / 4) * (j + 1);
        printf("Index  : ", i);
        for (i = start; i < end; i++) {
            printf("%4d", i);
        }
        printf("\n");
        printf("Answer : ", i);
        for (i = start; i < end; i++) {
            printf("%4d", g_answer[i]);
        }
        printf("\n\n");
    }
    printf("\n\n");
    
}
/************************************************/

/************************************************/
/**************** MQTT FUNCTIONS ****************/
void set_mosquitto(struct mosquitto *mosq)
{
    g_mosq = mosq;
}
void publish_hands()
{
    int *hand; 
    char topic[TOPIC_LEN], message[MESSAGE_LEN];
    printf("publish_hands\n");
    for (int i = 0; i < g_nb_players; i++) {
        hand = g_players[i].hand;
        snprintf(topic  , TOPIC_LEN  , "Player");
        snprintf(message, MESSAGE_LEN, "H%4d%4d%4d%4d%4d%4d%4d%4d%4d%4d%4d%4d", hand[0], hand[1], hand[ 2], hand[ 3], hand[4], hand[5], hand[ 6], hand[ 7], hand[8], hand[9], hand[10], hand[11]);
        printf("%s : %s\n", topic, message);
        publish(g_mosq, topic, message, 2);
    }
}
/************************************************/


/************************************************/
/******************* PARSING ********************/
int  parse_player_id(char *message)
{
    int i = 0, j = 0;
    char number[4] = {0,0,0,0};
    i = 1;
    while(message[i] != ' ' && message[i] != '\0'){
        number[j++] = message[i++];
    }
    return atoi(number);
}
int  parse_message_type(char *message)
{
    int i = 0;
    char message_type;
    
    while(message[i] != ' ' && message[i] != '\0') i++;
    
    return message[i + 1];
}
int  parse_card         (char *message)
{
    int i = 0, j = 0;
    char number[4] = {0,0,0,0};
    i = 0;
    
    while(message[i] != ' ' && message[i] != '\0') i++;
    i++;
    while(message[i] != ' ' && message[i] != '\0') i++;
    i++; 
    while(message[i] != ' ' && message[i] != '\0') {
        number[j++] = message[i++];
    }
        
    return atoi(number);
}
int  parse_response     (char *message)
{
    int i = 0, j = 0;
    char number[4] = {0,0,0,0};
    i = 0;
    
    while(message[i] != ' ' && message[i] != '\0') i++;
    i++;
    while(message[i] != ' ' && message[i] != '\0') i++;
    i++; 
    while(message[i] != ' ' && message[i] != '\0') {
        number[j++] = message[i++];
    }
        
    return atoi(number);
}

/************************************************/




int cards_per_level(void) {
    return g_level;
}





/************************************************/
/******************* DISCARD ********************/
void discard_lowest() {
    int i = 0, j = 0, min_index = 0, *hand;

    for (i = 0; i < g_nb_players; i++) {
        min_index = 0;
        hand = g_players[i].hand;
        for (j = 1; j < cards_per_level(); j++) {
            if (hand[j] < hand[min_index] && hand[j] > 0)  
                min_index = j;
        }

        for (j = 0; j < g_deck_top; j++) {
            if (hand[min_index] == g_answer[j]) {
                move_card(g_played, j, hand, min_index);
                g_played_top++;
                break;
            }
        }
        //move_card(g_played, g_played_top++, hand, min_index);
        // sort(g_played, g_played_top);
    }
}
void discard_lower(int card) {
    int i = 0, j = 0, k = 0, *hand, nb_cards =  cards_per_level();

    for (i = 0; i < g_nb_players; i++) {
        // printf("%d[%d]:\n", i, nb_cards);
        hand = g_players[i].hand;
        for (j = 0; j < nb_cards; j++) {
            if (hand[j] <= card && hand[j] > 0)  {
                // printf("[%d] %d <= %d\n", j, hand[j], card);
                discard_card(hand, j);
                // printf("\thand[%d] = %d\n", j, hand[j], card);
            }
        }
        pack_hand(hand, nb_cards);
    }

}
/************************************************/
