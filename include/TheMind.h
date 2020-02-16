

/************************************************/
/********************* DECK *********************/
int  *create_deck();
void  shuffle_deck(int *deck, int nb_shuffle);
void  free_deck(int *deck);
void distribute_cards(int nb_shuffles);
/************************************************/

/************************************************/
/******************** PLAYER ********************/
void set_nb_players(int nb_players);
int  get_nb_players(void);
void add_player(char *name);
struct player_info get_player(int i);
/************************************************/


/************************************************/
/********************* LIFE *********************/
void       set_lives(int lives);
int        get_lives(void);
void decrement_life(void);
void increment_life(void);
/************************************************/

/************************************************/
/******************** LEVEL *********************/
void set_level(int level);
int  get_level(void);
void decrement_level(void);
void increment_level(void);
/************************************************/

int init_game();
void print_current_status();


void shuriken();