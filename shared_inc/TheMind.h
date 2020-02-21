/************************************************/
/********************* DECK *********************/
void create_deck();
void shuffle_deck(int *deck, int nb_shuffle);
void distribute_cards(int nb_shuffles);
void initialize_played_cards();
void pack_hand(int *dst, int size);
void play_card(int *hand, int card);
void discard_card(int *hand, int card);
void move_card(int *dst, int dst_index, int *src, int src_index);
void copy_card(int *dst, int dst_index, int *src, int src_index);
void exchange_card(int *dst, int dst_index, int *src, int src_index);;
int cards_per_level(void);
/************************************************/

/************************************************/
/******************** PLAYER ********************/
void set_nb_players(int nb_players);
int  get_nb_players(void);
int  add_player(char *name);
struct player_info get_player(int i);
/************************************************/


/************************************************/
/********************* LIFE *********************/
void       set_lives(int lives);
int        get_lives(void);
void decrement_life(void);
void increment_life(void);
bool is_alive(void);
/************************************************/

/************************************************/
/******************** LEVEL *********************/
void set_level(int level);
int  get_level(void);
void decrement_level(void);
void increment_level(void);
/************************************************/

/************************************************/
/******************* SHURIKEN *******************/
void       set_shurikens(int shuriken);
int        get_shurikens(void);
void decrement_shuriken(void);
void increment_shuriken(void);
bool is_shuriken_available(void);
/************************************************/


/************************************************/
/********************* TURN *********************/
void initialize_round();
void next_round();
/************************************************/


/************************************************/
/******************* DISCARD ********************/
void discard_lowest();
void discard_lower(int card);
/************************************************/


/************************************************/
/****************** GAME PLAY *******************/
int  init_game(int nb_shuffle);
void start_new_round();
int  end_game();
bool check_answer();
int  sort(int *cards, int size);
/************************************************/

/************************************************/
/******************** DEBUG *********************/
void print_current_status();
/************************************************/

/************************************************/
/**************** MQTT FUNCTIONS ****************/
void set_mosquitto(struct mosquitto *);
void publish_hands();
/************************************************/

/************************************************/
/*************** MESSAGE CALLBACK ***************/
void message_from_player(char *message);
/************************************************/

/************************************************/
/******************* PARSING ********************/
int  parse_player_id    (char *message);
int  parse_message_type (char *message);
int  parse_card         (char *message);
int  parse_response     (char *message);
/************************************************/