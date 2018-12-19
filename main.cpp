#include <iostream>

#include "player.h"
#include "board.h"
#include "game.h"

using namespace std;

int main()
{
	Goban board;
	board.placeStone(BLACK, 18, 18);
	board.displayBoard();
	system("pause");
}