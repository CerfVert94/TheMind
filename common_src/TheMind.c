#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <TheMind.h>

#define EXIT_WITH_ERROR_MESSAGE(msg, ...) fprintf(stderr, "Error : "msg" (%s)\n", __VA_ARGS__, __func__);\
                                          exit(EXIT_FAILURE)

#define NAME_LEN    128
#define DECK_SIZE   100
#define MAX_PLAYER  4
#define MAX_LEVEL   12
struct player_info{
    int  id;
    char name[NAME_LEN];
}player_info;


static struct player_info g_players[MAX_PLAYER];

static int  g_level      = 0;
static int  g_lives      = 0;
static int  g_nb_players = 0;
static int *g_deck      = NULL;
static int  g_hands[MAX_PLAYER][MAX_LEVEL];
static int  g_deck_top   = 0;

int *create_deck() {
    int *deck = malloc(DECK_SIZE * sizeof(int));
    
    for (int i = 1; i <= DECK_SIZE; i++) 
        deck[i - 1] = i;
    return deck;
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

void free_deck(int *deck){
    free(deck);
}

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
void add_player(char *name) {
    if (g_nb_players > MAX_PLAYER - 1) {
        EXIT_WITH_ERROR_MESSAGE("The number of player cannot exceed %d", MAX_PLAYER);
    }
    g_players[g_nb_players].id = g_nb_players;
    memcpy(g_players[g_nb_players].name, name, NAME_LEN * sizeof(char));
    g_nb_players++;
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
void decrement_level(void) {
    g_level--;
}
void increment_level(void) {
    g_level++;
}

int get_lives(void) {
    return g_lives;
}

void set_level(int level) {
    g_level = level;
}

int get_level(void) {
    return g_level;
}

int cards_per_level(void) {
    return g_level;
}

void distribute_cards(int nb_shuffles)
{
    int i = 0, j = 0, nb_cards = cards_per_level();
    shuffle_deck(g_deck, nb_shuffles);

    for (i = 0; i < nb_cards; i++) {
        for (j = 0; j < g_nb_players; j++) {
            g_hands[j][i] = g_deck[g_deck_top++];
        }
    }
}

int init_game() {
    int i, j;
    // Seed random
    srand((unsigned)time(NULL));
    
    // Create a new deck / initialize hands
    g_deck = create_deck();
    for (i = 0; i < MAX_LEVEL; i++) {
        for (j = 0; j < MAX_PLAYER; j++) {
            g_hands[j][i] = 0;
        }
    }

    //Set intial level to 1
    set_level(1);
    //Set intial initial lives
    set_lives(get_nb_players());
    
}
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
            printf("%4d", g_hands[k][i]);
        }
        printf("\n");
    }
    
}

void shuriken() {
    
}