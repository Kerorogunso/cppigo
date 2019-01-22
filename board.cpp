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

std::vector<tuple<int, int>> Goban::getGroup(int row, int column)
{
	queue<tuple<int, int>> neighborQueue;
	std::vector<tuple<int, int>> stoneGroup;
	//getAdjacentNeighborsAndPush(row, column, neighborQueue, stoneGroup);

	while (!neighborQueue.empty())
	{	
		tuple<int, int> neighbor = neighborQueue.front();
		stoneGroup.push_back(neighbor);

		int neighborRow = get<0>(neighbor);
		int neighborColumn = get<1>(neighbor);

		//getAdjacentNeighborsAndPush(neighborRow, neighborColumn, neighborQueue, stoneGroup);
		neighborQueue.pop();
	}
	return stoneGroup;
}

bool notInGroup(std::vector<tuple<int, int>>& groupElements, tuple<int, int> stoneIndex)
{
	const auto position = find(groupElements.begin(), groupElements.end(), stoneIndex);
	if (position == groupElements.end())
	{
		return true;
	}
	return false;
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

std::vector<tuple<int, int>> Goban::returnNeighbors(int row, int col)
{
	std::vector<tuple<int, int>> neighbors = {};
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

void Goban::getNeighbors(std::vector<tuple<int, int>> *neighbors, int row, int col)
{	
	
	std::vector<tuple<int, int>> potentialNeighbors = {
		make_tuple(row, col + 1),
		make_tuple(row, col - 1),
		make_tuple(row + 1, col),
		make_tuple(row - 1, col)
	 };
	
	for (auto it = potentialNeighbors.begin(); it != potentialNeighbors.end(); ++it)
	{	
		int neighborRow = get<0>(*it);
		int neighborCol = get<1>(*it);

		if (isNotInRange(neighborRow, neighborCol))
		{
			continue;
		}
		
		if (this->board(neighborRow, neighborCol) == this->board(row, col))
		{
			if (find(neighbors->begin(), neighbors->end(), *it) == neighbors->end())
			{
				neighbors->push_back(*it);
				getNeighbors(neighbors, neighborRow, neighborCol);
			}
		}
	}
}