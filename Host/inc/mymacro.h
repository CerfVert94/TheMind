#define EXIT_WITH_ERROR_MESSAGE(msg, ...) fprintf(stderr, "Error : "msg" (%s)\n", __VA_ARGS__, __func__);\
                                          exit(EXIT_FAILURE)

#define NAME_LEN         128
#define DECK_SIZE        100
#define PLAYABLE_SIZE    48
#define MAX_PLAYER       4
#define MAX_LEVEL        12
#define WINNING_LEVEL    8
#define CARDS_PER_HAND   MAX_LEVEL
#define TOPIC_LEN     128
#define MESSAGE_LEN   256
#define VOTE_YES  2
#define VOTE_NO   1
#define VOTE_WAIT 0


#define PUB_PLAYER_TOPIC(id) "Player/"id
#define SUB_HOST_TOPIC       "Host"

#define MSG_TYPE_CARD     'C'
#define MSG_TYPE_DISCARD  'D'
#define MSG_TYPE_SHURIKEN 'S'
#define MSG_TYPE_VOTE     'V'