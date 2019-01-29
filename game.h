#include "player.h"
#include "board.h"

using namespace std;
class GoGame
{
public:
	Goban goban;
	Player black;
	Player white;
	Player currentPlayer;

	GoGame();
	GoGame(Goban board, Player black, Player white);
	
	void play(int row, int col);
	void startGame();
	
private:
	void switchActivePlayer();
	void checkForCapturedStones(int row, int col);
};

tuple<int, int> parseMove(string move);