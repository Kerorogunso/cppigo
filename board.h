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

		void placeStone(int stone, int row, int column);
		int getBoardSize();
		void operator=(const Goban& goban);
		std::vector<std::tuple<int, int>> getGroup(int row, int column);
	
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
void getAdjacentNeighborsAndPush(int row, int column, std::queue<std::tuple<int, int>>& queue, std::vector<std::tuple<int, int>> group);
bool notInGroup(std::vector<std::tuple<int, int>>& groupElements, std::tuple<int, int> stoneIndex);
std::vector<std::tuple<int, int>> getNeighbors(int row, int column);
