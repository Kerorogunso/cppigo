#include "pch.h"
#include "cppigo_lib/GoGame.h"
#include "cppigo_lib/Board.h"

TEST(GoGame, SelfAtari)
{
    Goban testBoard(9);
    Player black("Player 1", kBlack);
    Player white("Player 2", kWhite);

    testBoard.placeStone(BLACK, 1, 0);
    testBoard.placeStone(BLACK, 0, 1);
    testBoard.placeStone(BLACK, 1, 2);
    testBoard.placeStone(BLACK, 2, 1);
    
    testBoard.placeStone(WHITE, 1, 3);
    testBoard.placeStone(WHITE, 2, 2);
    testBoard.placeStone(WHITE, 0, 2);

    GoGame testGame(testBoard, black, white);
    testGame.play(1, 1);
 
}

TEST(GoGame, KoRule)
{
    Goban testBoard(9);
    Player black("Player 1", kBlack);
    Player white("Player 2", kWhite);

    testBoard.placeStone(BLACK, 1, 0);
    testBoard.placeStone(BLACK, 0, 1);
    testBoard.placeStone(BLACK, 1, 2);
    testBoard.placeStone(BLACK, 2, 1);
    
    testBoard.placeStone(WHITE, 1, 3);
    testBoard.placeStone(WHITE, 2, 2);
    testBoard.placeStone(WHITE, 0, 2);

    GoGame testGame(testBoard, black, white);
    testGame.currentPlayer = testGame.white;
    testGame.play(1, 1);
    GobanError invalidKo = testGame.play(1, 2);
    EXPECT_EQ(invalidKo, GobanError::kKoRule);
    
}

TEST(GoGame, CaptureCentre)
{
    Goban testBoard(9);
    Player black("Player 1", kBlack);
    Player white("Player 2", kWhite);

    testBoard.placeStone(BLACK, 1, 0);
    testBoard.placeStone(BLACK, 0, 1);
    testBoard.placeStone(BLACK, 1, 2);
    testBoard.placeStone(BLACK, 2, 1);

    GoGame testGame(testBoard, black, white);
    testGame.currentPlayer = testGame.white;

    GobanError atari = testGame.play(1, 1);
    EXPECT_EQ(atari, GobanError::kSelfAtari);
    
}

TEST(GoGame, CaptureCorner)
{
    Goban testBoard(13);
    Player black("Player 1", kBlack);
    Player white("Player 2", kWhite);

    testBoard.placeStone(BLACK, 11, 12);
    testBoard.placeStone(WHITE, 12, 12);
    GoGame testGame(testBoard, black, white);
    GobanError captureCorner = testGame.play(12, 11);
    EXPECT_EQ(testGame.goban.board()(12, 12), EMPTY);
}

TEST(GoGame, CaptureSide)
{
    Goban testBoard(19);
    Player black("Player 1", kBlack);
    Player white("Player 2", kWhite);
    
    testBoard.placeStone(BLACK, 5, 18);
    testBoard.placeStone(BLACK, 6, 18);
    testBoard.placeStone(WHITE, 5, 17);
    testBoard.placeStone(WHITE, 6, 17);
    testBoard.placeStone(WHITE, 4, 18);
       
    GoGame testGame(testBoard, black, white);
    testGame.currentPlayer = testGame.white;

    // Play capturing white stone
    testGame.play(7, 18);

    EXPECT_EQ(testGame.goban.board()(5, 18), EMPTY);
    EXPECT_EQ(testGame.goban.board()(6, 18), EMPTY);
}