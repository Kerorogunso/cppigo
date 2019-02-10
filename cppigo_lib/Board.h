#pragma once
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include "cppigo_exports.h"
#include <queue>

using Group = std::vector<std::tuple<int, int>>;

enum stones {
    EMPTY,
    BLACK,
    WHITE
};

class CPPIGO_DLL Goban
{
public:
    int boardSize;

    Goban();
    Goban(int boardSize);

    void operator=(const Goban& goban);
    const bool operator==(const Goban& goban);

    void placeStone(int stone, int row, int column);
    void placeStone(int stone, std::tuple<int, int> index);
    int getBoardSize();
    void getNeighbors(Group *neighbors, int row, int col);
    void checkSelfAtari(int row, int col);
    Group returnNeighbors(int row, int col);
    void displayBoard();
    int getLiberties(Group neighbors);
    bool isEmpty(std::tuple<int, int> coordinates);
    const boost::numeric::ublas::matrix<int> &board() const;
    boost::numeric::ublas::matrix<int> *board_mutable();

private:
    const std::vector<int> validBoardSizes = { 9, 13, 19 };
    bool isNotInRange(int row, int column);
    boost::numeric::ublas::matrix<int> m_board;
};

bool isInvalidStone(int stone);
Group getAdjacentSquares(int row, int col, int boardSize);