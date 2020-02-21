#include <stdlib.h>
#include <stdbool.h>
#include <vote.h>
#include <mymacro.h>

static int *g_votes = NULL;
static int  g_nb_players = 0;
void init_vote(int nb_players){
    if (g_votes) 
        return;
    g_nb_players = nb_players;
    g_votes      = malloc(sizeof(int) * g_nb_players);
    for (int i = 0; i < g_nb_players; i++) {
        g_votes[i] = VOTE_WAIT;
    }

}

bool is_vote_called(void) {
    return g_votes != NULL;
}

void vote(int id, int response){
    if (g_votes[id] == VOTE_WAIT)
        g_votes[id] = response;
}

int ballot_count()
{
    int count[3] = {0,0,0};
    for (int i = 0; i < g_nb_players; i++) 
        count[g_votes[i]]++;

    if (count[VOTE_WAIT] > 0) 
        return VOTE_WAIT;
    free(g_votes);
    g_votes = NULL;
    return count[VOTE_NO] < count[VOTE_YES] ? VOTE_YES : VOTE_NO;
}