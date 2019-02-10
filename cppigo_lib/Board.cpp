#include "stdafx.h"

#include <map>
#include <stdexcept>
#include <queue>
#include <iostream>

#include "Board.h"

using namespace boost::numeric::ublas;

Goban::Goban()
{
    boardSize = 19;
    m_board = matrix<int>(boardSize, boardSize, 0);
}

Goban::Goban(int boardSize)
{
    if (find(validBoardSizes.begin(), validBoardSizes.end(), boardSize) == validBoardSizes.end())
    {
        throw std::invalid_argument("Board size must be 9, 13 or 19.");
    }
    this->boardSize = boardSize;
    m_board = matrix<int>(boardSize, boardSize, 0);
}

void Goban::placeStone(int stone, int row, int column)
{
    if (isInvalidStone(stone))
    {
        throw std::invalid_argument("Stone is not valid.");
    }

    if (isNotInRange(row, column))
    {
        throw std::logic_error("Stone coordinates is not in range.");
    }

    if (m_board(row, column) != EMPTY && stone != EMPTY)
    {
        throw std::logic_error("There is a stone already there.");
    }

    m_board(row, column) = stone;
}

void Goban::placeStone(int stone, std::tuple<int, int> index)
{
    int row = std::get<0>(index);
    int column = std::get<1>(index);
    Goban::placeStone(stone, row, column);
}

int Goban::getBoardSize()
{
    return boardSize;
}

void Goban::operator=(const Goban & goban)
{
    boardSize = goban.boardSize;
    m_board = goban.board();
}

const bool Goban::operator==(const Goban& goban)
{
    for (int i = 0; i < getBoardSize(); ++i)
    {
        for (int j = 0; j < getBoardSize(); ++j)
        {
            if (this->m_board(i, j) != goban.board()(i, j))
            {
                return false;
            }
        }
    }
    return true;
}

bool notInGroup(Group &groupElements, std::tuple<int, int> stoneIndex)
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
    bool withinColumn = (column >= 0) && (column < getBoardSize());

    if (withinRow && withinColumn)
    {
        return false;
    }
    return true;
}

const matrix<int> &Goban::board() const
{
    return m_board;
}

matrix<int> *Goban::board_mutable()
{
    return &m_board;
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
    //const std::string BLACK_UNICODE = "\u25CF";
    //const std::string WHITE_UNICODE = "\u25CB";

    //const std::string BOX_TOP_LEFT = "\u250E";
    //const std::string BOX_TOP_RIGHT = "\u2513";
    //const std::string BOX_TOP = "\u2530";
    //const std::string BOX_LEFT = "\u2523";
    //const std::string BOX_RIGHT = "\u252B";
    //const std::string BOX_BOTTOM_LEFT = "\u2516";
    //const std::string BOX_BOTTOM_RIGHT = "\u251B";
    //const std::string BOX_BOTTOM = "\u253A";
    //const std::string BOX_CENTRE = "\u254B";

    const std::string BLACK_UNICODE = "B";
    const std::string WHITE_UNICODE = "W";

    const std::string BOX_TOP_LEFT = "+";
    const std::string BOX_TOP_RIGHT = "+";
    const std::string BOX_TOP = "+";
    const std::string BOX_LEFT = "+";
    const std::string BOX_RIGHT = "+";
    const std::string BOX_BOTTOM_LEFT = "+";
    const std::string BOX_BOTTOM_RIGHT = "+";
    const std::string BOX_BOTTOM = "+";
    const std::string BOX_CENTRE = "+";

    std::string gridChar;

    for (unsigned int i = 0; i < m_board.size1(); ++i)
    {
        for (unsigned int j = 0; j < m_board.size2(); ++j)
        {
            if (m_board(i, j) == BLACK)
            {
                gridChar = BLACK_UNICODE;
            }
            else if (m_board(i, j) == WHITE)
            {
                gridChar = WHITE_UNICODE;
            }
            else
            {
                if (i == 0)
                {
                    if (j == 0)
                        gridChar = BOX_TOP_LEFT;
                    else if (j == m_board.size2() - 1)
                        gridChar = BOX_TOP_RIGHT;
                    else
                        gridChar = BOX_TOP;
                }

                else if (j == 0)
                {
                    if (i == m_board.size1() - 1)
                        gridChar = BOX_BOTTOM_LEFT;
                    else
                        gridChar = BOX_LEFT;
                }

                else if (i == m_board.size1() - 1)
                {
                    if (j == m_board.size2() - 1)
                        gridChar = BOX_BOTTOM_RIGHT;
                    else
                        gridChar = BOX_BOTTOM;
                }
                else if (j == m_board.size2() - 1)
                    gridChar = BOX_RIGHT;
                else
                    gridChar = BOX_CENTRE;

            }
            std::cout << gridChar;
        }
        std::cout << std::endl;
    }
}

int Goban::getLiberties(Group neighbors)
{

    int numLiberties = 0;
    Group libertyCoords = {};
    std::tuple<int, int> stoneCoords = neighbors[0];

    if (m_board(std::get<0>(stoneCoords), std::get<1>(stoneCoords)) == EMPTY)
    {
        return 1;
    }

    for (auto coordinates : neighbors)
    {
        int neighborRow = std::get<0>(coordinates);
        int neighborColumn = std::get<1>(coordinates);

        Group adjacentSquares = getAdjacentSquares(neighborRow, neighborColumn, getBoardSize());

        for (auto adjacent : adjacentSquares)
        {
            int row = std::get<0>(adjacent);
            int col = std::get<1>(adjacent);

            if (isEmpty(adjacent) && find(libertyCoords.begin(), libertyCoords.end(), adjacent) == libertyCoords.end())
            {
                numLiberties++;
                libertyCoords.push_back(adjacent);
            }

        }
    }
    return numLiberties;
}

bool Goban::isEmpty(std::tuple<int, int> coordinates)
{
    int row = std::get<0>(coordinates);
    int col = std::get<1>(coordinates);
    return m_board(row, col) == EMPTY;
}

void Goban::getNeighbors(Group *neighbors, int row, int col)
{

    Group potentialNeighbors = getAdjacentSquares(row, col, getBoardSize());

    for (auto neighbor : potentialNeighbors)
    {
        int neighborRow = std::get<0>(neighbor);
        int neighborCol = std::get<1>(neighbor);

        if (m_board(neighborRow, neighborCol) == m_board(row, col))
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
        {std::max(row - 1, 0), col},
        {std::min(row + 1, boardSize - 1), col},
        {row, std::max(col - 1, 0)},
        {row, std::min(col + 1, boardSize - 1)}
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
        throw std::logic_error("Invalid Move: Self Atari");
    }
}