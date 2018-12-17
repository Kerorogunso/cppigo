#include <iostream>

#include "player.h"
#include "board.h"
#include "game.h"

using namespace std;

int main()
{
	Player albert("Albert");
	Player rohit("Rohit");

	Goban board;
	GoGame myGame(board, albert, rohit);
	myGame.white.printName();
	cout << myGame.black.name << endl;
	cout << myGame.currentPlayer.name << endl;
	system("pause");
}