const int VOTE_YES  = 2;
const int VOTE_NO   = 1;
const int VOTE_WAIT = 0;
void    init_vote(int nb_players);
void         vote(int id, int response);
int  ballot_count();