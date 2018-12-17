#pragma once
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>

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

		void placeStone(int stone, int row, int column);
		int getBoardSize();
		void operator=(const Goban& goban);
	
	private:
		const std::vector<int> validBoardSizes = { 9, 13, 19 };
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
