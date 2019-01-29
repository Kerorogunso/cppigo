#pragma once
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <queue>

using namespace boost::numeric::ublas;
using namespace std;

using Group = std::vector<tuple<int, int>>;

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

		void operator=(const Goban& goban);

		void placeStone(int stone, int row, int column);
		void placeStone(int stone, tuple<int, int> index);
		int getBoardSize();
		void getNeighbors(Group *neighbors, int row, int col);
		Group returnNeighbors(int row, int col);
		void displayBoard();
		int getLiberties(Group neighbors);
		bool isEmpty(tuple<int, int> coordinates);
	
	private:
		const std::vector<int> validBoardSizes = { 9, 13, 19 };
		bool isNotInRange(int row, int column);
};

bool isInvalidStone(int stone);
