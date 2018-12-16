#include "player.h"
#include "board.h"

class GoGame
{
    public:
        Goban board;
        Player black;
        Player white;

    GoGame();
    GoGame(Goban board, Player black, Player white);
};