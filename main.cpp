#include <iostream>

#include "player.h"
#include "board.h"
#include "game.h"

using namespace std;

int main()
{
	Goban board(19);
	board.placeStone(BLACK, 8, 8);
	board.placeStone(BLACK, 7, 8);
	board.placeStone(BLACK, 7, 7);
	board.displayBoard();
	
	std::vector<tuple<int, int>> result = board.returnNeighbors(7, 8);
	for (auto it = result.begin(); it != result.end(); ++it)
	{
		std::cout << get<0>(*it) << " " << get<1>(*it) << std::endl;
	}
	system("pause");
}