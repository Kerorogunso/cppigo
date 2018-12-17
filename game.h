#include "player.h"
#include "board.h"

class GoGame
{
    public:
        Goban board;
        Player black;
        Player white;
		Player currentPlayer;

		GoGame();
		GoGame(Goban board, Player black, Player white);
};