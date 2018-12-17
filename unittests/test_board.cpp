#include "pch.h"

#include "../board.cpp"
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