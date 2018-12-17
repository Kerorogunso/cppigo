#include <vector>
#include <map>
#include <stdexcept>

#include "board.h"

using namespace std;

Goban::Goban()
{
	boardSize = 19;
	board = {};
}

Goban::Goban(int boardSize)
{	
	if (find(validBoardSizes.begin(), validBoardSizes.end(), boardSize) == validBoardSizes.end())
	{
		throw invalid_argument("Board size must be 9, 13 or 19.");
	}
	this->boardSize = boardSize;
}

void Goban::placeStone(vector<int> position)
{
    vector<int> a;
}

int Goban::getBoardSize()
{
    return boardSize;
}

void Goban::operator=(const Goban & goban)
{
	boardSize = goban.boardSize;
	board = goban.board;
}

int Group::getLiberties()
{
    return liberties;
}

bool Group::isCaptured()
{
    if (liberties == 0)
    {
        return true;
    }
    return false;
}