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

	if (board(row, column) != EMPTY && stone != EMPTY)
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

const bool Goban::operator==(const Goban& goban)
{
	for (int i = 0; i < getBoardSize(); ++i)
	{
		for (int j = 0; j < getBoardSize(); ++j)
		{
			if (this->board(i, j) != goban.board(i, j))
			{
				return false;
			}
		}
	}
	return true;
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
	return stone != BLACK && stone != WHITE && stone != EMPTY;
}

bool Goban::isNotInRange(int row, int column)
{
	bool withinRow = (row >= 0) && (row < getBoardSize());
	bool withinColumn = (column >= 0) && (row < getBoardSize());

	if (withinRow && withinColumn)
	{
		return false;
	}
	return true;
}

Group Goban::returnNeighbors(int row, int col)
{
	Group neighbors = {};
	neighbors.push_back({ row, col });
	getNeighbors(&neighbors, row, col);

	return neighbors;
}

void Goban::displayBoard()
{
	//const string BLACK_UNICODE = "\u25CF";
	//const string WHITE_UNICODE = "\u25CB";

	//const string BOX_TOP_LEFT = "\u250E";
	//const string BOX_TOP_RIGHT = "\u2513";
	//const string BOX_TOP = "\u2530";
	//const string BOX_LEFT = "\u2523";
	//const string BOX_RIGHT = "\u252B";
	//const string BOX_BOTTOM_LEFT = "\u2516";
	//const string BOX_BOTTOM_RIGHT = "\u251B";
	//const string BOX_BOTTOM = "\u253A";
	//const string BOX_CENTRE = "\u254B";

	const string BLACK_UNICODE = "B";
	const string WHITE_UNICODE = "W";

	const string BOX_TOP_LEFT = "+";
	const string BOX_TOP_RIGHT = "+";
	const string BOX_TOP = "+";
	const string BOX_LEFT = "+";
	const string BOX_RIGHT = "+";
	const string BOX_BOTTOM_LEFT = "+";
	const string BOX_BOTTOM_RIGHT = "+";
	const string BOX_BOTTOM = "+";
	const string BOX_CENTRE = "+";
	
	string gridChar;

	for (unsigned int i = 0; i < board.size1(); ++i)
	{
		for (unsigned int j = 0; j < board.size2(); ++j)
		{
			if (board(i, j) == BLACK)
			{
				gridChar = BLACK_UNICODE;
			}
			else if (board(i, j) == WHITE)
			{
				gridChar = WHITE_UNICODE;
			}
			else
			{
				if (i == 0)
				{
					if (j == 0)
						gridChar = BOX_TOP_LEFT;
					else if (j == board.size2() - 1)
						gridChar = BOX_TOP_RIGHT;
					else
						gridChar = BOX_TOP;
				}

				else if (j == 0)
				{
					if (i == board.size1() - 1)
						gridChar = BOX_BOTTOM_LEFT;
					else
						gridChar = BOX_LEFT;
				}

				else if (i == board.size1() - 1)
				{
					if (j == board.size2() - 1)
						gridChar = BOX_BOTTOM_RIGHT;
					else
						gridChar = BOX_BOTTOM;
				}
				else if (j == board.size2() - 1)
					gridChar = BOX_RIGHT;
				else
					gridChar = BOX_CENTRE;
				
			}
			cout << gridChar;
		}
		cout << endl;
	}
}

int Goban::getLiberties(Group neighbors)
{
	
	int numLiberties = 0;
	Group libertyCoords = {};
	tuple<int, int> stoneCoords = neighbors[0];

	if (board(get<0>(stoneCoords), get<1>(stoneCoords)) == EMPTY)
	{
		return 1;
	}
	
	for (auto coordinates : neighbors)
	{
		int neighborRow = get<0>(coordinates);
		int neighborColumn = get<1>(coordinates);

		Group adjacentSquares = getAdjacentSquares(neighborRow, neighborColumn, getBoardSize());

		for (auto adjacent: adjacentSquares)
		{	
			int row = get<0>(adjacent);
			int col = get<1>(adjacent);

			if (isEmpty(adjacent) && find(libertyCoords.begin(), libertyCoords.end(), adjacent) == libertyCoords.end())
			{
				numLiberties++;
				libertyCoords.push_back(adjacent);
			}

		}
	}
	return numLiberties;
}

bool Goban::isEmpty(tuple<int, int> coordinates)
{
	int row = get<0>(coordinates);
	int col = get<1>(coordinates);
	return board(row, col) == EMPTY;
}

void Goban::getNeighbors(Group *neighbors, int row, int col)
{	
	
	Group potentialNeighbors = getAdjacentSquares(row, col, getBoardSize());
	
	for (auto neighbor : potentialNeighbors)
	{	
		int neighborRow = get<0>(neighbor);
		int neighborCol = get<1>(neighbor);

		if (board(neighborRow, neighborCol) == board(row, col))
		{
			if (find(neighbors->begin(), neighbors->end(), neighbor) == neighbors->end())
			{
				neighbors->push_back(neighbor);
				getNeighbors(neighbors, neighborRow, neighborCol);
			}
		}
	}
}

Group getAdjacentSquares(int row, int col, int boardSize)
{
	Group adjacentSquares = {
		{max(row - 1, 0), col},
		{min(row + 1, boardSize - 1), col},
		{row, max(col - 1, 0)},
		{row, min(col + 1, boardSize - 1)}
	};
	return adjacentSquares;
}

void Goban::checkSelfAtari(int row, int col)
{
	Group stoneGroup = returnNeighbors(row, col);
	int liberties = getLiberties(stoneGroup);
	if (liberties == 0)
	{
		placeStone(EMPTY, row, col);
		throw logic_error("Invalid Move: Self Atari");
	}
}