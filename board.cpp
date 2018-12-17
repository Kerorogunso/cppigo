#include <map>
#include <stdexcept>

#include "board.h"

using namespace std;
using namespace boost::numeric::ublas;

Goban::Goban()
{
	boardSize = 19;
	board = matrix<int>(boardSize, boardSize);
}

Goban::Goban(int boardSize)
{	
	if (find(validBoardSizes.begin(), validBoardSizes.end(), boardSize) == validBoardSizes.end())
	{
		throw invalid_argument("Board size must be 9, 13 or 19.");
	}
	this->boardSize = boardSize;
}

void Goban::placeStone(int stone, int row, int column)
{	
	if (!isInvalidStone(stone))
	{
		throw std::invalid_argument("Stone is not valid.");
	}
	
	board (row, column) = stone;
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

bool isInvalidStone(int stone)
{
	return stone != BLACK && stone != WHITE && stone == EMPTY;
}
