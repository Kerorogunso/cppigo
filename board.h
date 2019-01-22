#pragma once
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <queue>

using namespace boost::numeric::ublas;

enum stones {
	EMPTY,
	BLACK,
	WHITE
};

class Goban
{
    public:
        int boardSize;
        matrix<int> board;

		Goban();
		Goban(int boardSize);
		Goban(Goban& goban);

		bool operator=(const Goban& goban);

		void placeStone(int stone, int row, int column);
		void placeStone(int stone, tuple<int, int> index);
		int getBoardSize();
		void getNeighbors(std::vector<tuple<int,int>> *neighbors, int row, int col);
		std::vector<tuple<int,int>> returnNeighbors(int row, int col);
		void displayBoard();
	
	private:
		const std::vector<int> validBoardSizes = { 9, 13, 19 };
		bool isNotInRange(int row, int column);
};

class Group
{
    public:
        std::vector<int> stones;
        int liberties;

    int getLiberties();
    bool isCaptured();
};

bool isInvalidStone(int stone);
bool notInGroup(std::vector<std::tuple<int, int>>& groupElements, std::tuple<int, int> stoneIndex);
