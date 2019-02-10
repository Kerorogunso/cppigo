#include "pch.h"
#include <random>

#include "cppigo_lib/Board.h"

TEST(Board, BoardSize)
{    
    Goban goban9x9(9);
    Goban goban13x13(13);
    Goban goban19x19(19);

    int boardSize9x9 = goban9x9.getBoardSize();
    int boardSize13x13 = goban13x13.getBoardSize();
    int boardSize19x19 = goban19x19.getBoardSize();

    EXPECT_EQ(boardSize9x9, 9);
    EXPECT_EQ(boardSize13x13, 13);
    EXPECT_EQ(boardSize19x19, 19);

}

TEST(Board, PlaceStone)
{
    Goban testGoban(9);
    testGoban.placeStone(BLACK, { 5, 4 });
    EXPECT_EQ(testGoban.board()(5,4), BLACK);

    testGoban.placeStone(WHITE, 4, 4);
    EXPECT_EQ(testGoban.board()(4,4), WHITE);
    
    EXPECT_THROW(testGoban.placeStone(WHITE, { 10, 5 }), std::logic_error);
    EXPECT_THROW(testGoban.placeStone(BLACK, 5, 10), std::logic_error);

}

TEST(Board, BoardAssignment)
{
    Goban oldBoard(9);
    Goban newBoard(13);
    
    int randomStone;
    for (int i = 0; i < 13; ++i)
    {
        for (int j = 0; j < 13; ++j)
        {
            randomStone = (i * j) % 2 + 1;
            newBoard.placeStone(randomStone, i, j);
        }
    }
    oldBoard = newBoard;

    EXPECT_EQ(oldBoard.getBoardSize(), 13);


    for (int i = 0; i < 13; ++i)
    {
        for (int j = 0; j < 13; ++j)
        {
            EXPECT_EQ(oldBoard.board()(i, j), newBoard.board()(i, j));
        }
    }

}