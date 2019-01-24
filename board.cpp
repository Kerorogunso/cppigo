#include <map>
#include <stdexcept>
#include <queue>
#include <iostream>

#include "board.h"

using namespace std;
using namespace boost::numeric::ublas;

Goban::Goban()
{
	boardSize = 19;
	board = matrix<int>(boardSize, boardSize, 0);
}

Goban::Goban(int boardSize)
{	
	if (find(validBoardSizes.begin(), validBoardSizes.end(), boardSize) == validBoardSizes.end())
	{
		throw invalid_argument("Board size must be 9, 13 or 19.");
	}
	this->boardSize = boardSize;
	board = matrix<int>(boardSize, boardSize, 0);
}

Goban::Goban(Goban& goban)
{
	board = goban.board;
	boardSize = goban.getBoardSize();
}

void Goban::placeStone(int stone, int row, int column)
{	
	if (isInvalidStone(stone))
	{
		throw invalid_argument("Stone is not valid.");
	}

	if (isNotInRange(row, column))
	{
		throw logic_error("Stone coordinates is not in range.");
	}

	if (board(row, column) != EMPTY)
	{
		throw logic_error("There is a stone already there.");
	}
	
	board (row, column) = stone;
}

void Goban::placeStone(int stone, tuple<int, int> index)
{
	int row = get<0>(index);
	int column = get<1>(index);
	Goban::placeStone(stone, row, column);
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

bool notInGroup(Group &groupElements, tuple<int, int> stoneIndex)
{
	const auto position = find(groupElements.begin(), groupElements.end(), stoneIndex);
	if (position == groupElements.end())
	{
		return true;
	}
	return false;
}

bool isInvalidStone(int stone)
{
	return stone != BLACK && stone != WHITE && stone == EMPTY;
}

bool Goban::isNotInRange(int row, int column)
{
	bool withinRow = (row >= 0 && row < getBoardSize());
	bool withinColumn = (column >= 0 && row < getBoardSize());

	if (withinRow && withinColumn)
	{
		return false;
	}
	return true;
}

Group Goban::returnNeighbors(int row, int col)
{
	Group neighbors = {};
	getNeighbors(&neighbors, row, col);

	return neighbors;
}

void Goban::displayBoard()
{
	for (int i = 0; i < board.size1(); ++i)
	{
		for (int j = 0; j < board.size2(); ++j)
		{
			cout << board(i, j);
		}
		cout << endl;
	}
}

int Goban::getLiberties(Group neighbors)
{
	
	int numLiberties = 0;
	Group libertyCoords = {};
	
	for (auto coordinates : neighbors)
	{
		int neighborRow = get<0>(coordinates);
		int neighborColumn = get<1>(coordinates);

		Group adjacentSquares = {
			make_pair(neighborRow + 1, neighborColumn),
			make_pair(neighborRow - 1, neighborColumn),
			make_pair(neighborRow, neighborColumn + 1),
			make_pair(neighborRow, neighborColumn - 1)
		};

		for (auto adjacent: adjacentSquares)
		{
			if (this->isEmpty(get<0>(adjacent), get<1>(adjacent)) && find(libertyCoords.begin(), libertyCoords.end(), (adjacent)) == libertyCoords.end())
			{
				numLiberties++;
				neighbors.push_back(adjacent);
			}

		}
	}
	return numLiberties;
}

bool Goban::isEmpty(int row, int col)
{
	return this->board(row, col) == EMPTY;
}

void Goban::getNeighbors(Group *neighbors, int row, int col)
{	
	
	Group potentialNeighbors = {
		make_tuple(row, col + 1),
		make_tuple(row, col - 1),
		make_tuple(row + 1, col),
		make_tuple(row - 1, col)
	 };
	
	for (auto neighbor : potentialNeighbors)
	{	
		int neighborRow = get<0>(neighbor);
		int neighborCol = get<1>(neighbor);

		if (isNotInRange(neighborRow, neighborCol))
		{
			continue;
		}
		
		if (this->board(neighborRow, neighborCol) == this->board(row, col))
		{
			if (find(neighbors->begin(), neighbors->end(), neighbor) == neighbors->end())
			{
				neighbors->push_back(neighbor);
				getNeighbors(neighbors, neighborRow, neighborCol);
			}
		}
	}
}