#include <iostream>

#include "player.h"
#include "board.h"
#include "game.h"

using namespace std;

int main()
{
	Goban board;
	board.placeStone(BLACK, 18, 18);
	system("pause");
	
	std::vector<std::tuple<int, int>> group = board.getGroup(5, 5);

	for (auto it = group.begin(); it != group.end(); ++it)
	{
		std::cout << std::get<0>(*it) << std::endl;
	}
	system("pause");
}