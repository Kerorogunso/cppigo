#include "pch.h"
#include "cppigo_lib/GoGame.h"
#include "cppigo_lib/Board.h"

TEST(GoGame, MakeMove)
{
    GoGame testGame;
    GobanError code = testGame.makeMove(1, 1);
    EXPECT_EQ(code, GobanError::kSuccess);
}

TEST(GoGame, SelfAtari)
{
    Goban testBoard(9);
    Player black("Player 1", kBlack);
    Player white("Player 2", kWhite);
    
    // Illegal white position on centre
    //   0 1 2
    // 0   B
    // 1 B . B
    // 2   B
    testBoard.placeStone(BLACK, 1, 0);
    testBoard.placeStone(BLACK, 0, 1);
    testBoard.placeStone(BLACK, 1, 2);
    testBoard.placeStone(BLACK, 2, 1);
    
    GoGame testGame(testBoard, black, white);
    testGame.currentPlayer = testGame.white;
    GobanError selfAtariCheck = testGame.play(1, 1);
    EXPECT_EQ(selfAtariCheck, GobanError::kSelfAtari);
    
    // Illegal black position on corner
    //   7 8
    // 7   W
    // 8 W . 
    testBoard.placeStone(WHITE, 7, 8);
    testBoard.placeStone(WHITE, 8, 7);
    GoGame testGame2(testBoard, black, white);
    GobanError selfAtariCheck2 = testGame2.play(8, 8);
    EXPECT_EQ(selfAtariCheck2, GobanError::kSelfAtari);
}

TEST(GoGame, KoRule)
{
    Goban testBoard(9);
    Player black("Player 1", kBlack);
    Player white("Player 2", kWhite);
    
    // Board setup
    //   0 1 2 3
    // 0   B W 
    // 1 B . B W
    // 2   B W 
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
    
    // Replay on captured position is illegal as it makes a previous board state
    //   0 1 2 3
    // 0   B W 
    // 1 B W . W
    // 2   B W 
    GobanError invalidKo = testGame.play(1, 2);
    EXPECT_EQ(invalidKo, GobanError::kKoRule);
    
}

TEST(GoGame, CaptureCentre)
{
    Goban testBoard(9);
    Player black("Player 1", kBlack);
    Player white("Player 2", kWhite);
    
    // Black plays below white and captures successfully 
    //   0 1 2
    // 0   B
    // 1 B W B
    // 2   .
    testBoard.placeStone(BLACK, 1, 0);
    testBoard.placeStone(BLACK, 0, 1);
    testBoard.placeStone(BLACK, 1, 2);
    testBoard.placeStone(WHITE, 1, 1);

    GoGame testGame(testBoard, black, white);
    testGame.currentPlayer = testGame.white;

    GobanError capture = testGame.play(2, 1);
    EXPECT_EQ(capture, GobanError::kSuccess);
    
}

TEST(GoGame, CaptureCorner)
{
    Goban testBoard(13);
    Player black("Player 1", kBlack);
    Player white("Player 2", kWhite);
    
    // Black plays left of white in bottom corner and captures
    //    11 12
    // 11     B
    // 12  .  W
    testBoard.placeStone(BLACK, 11, 12);
    testBoard.placeStone(WHITE, 12, 12);
    GoGame testGame(testBoard, black, white);
    GobanError captureCorner = testGame.play(12, 11);
    EXPECT_EQ(testGame.goban.board()(12, 12), EMPTY);
    EXPECT_EQ(captureCorner, GobanError::kSuccess);
}

TEST(GoGame, CaptureSide)
{
    Goban testBoard(19);
    Player black("Player 1", kBlack);
    Player white("Player 2", kWhite);
    
    // Board setup on right hand side
    //   17 18
    // 5     W  
    // 6  W  B
    // 7  W  B
    // 8     .
    testBoard.placeStone(BLACK, 5, 18);
    testBoard.placeStone(BLACK, 6, 18);
    testBoard.placeStone(WHITE, 5, 17);
    testBoard.placeStone(WHITE, 6, 17);
    testBoard.placeStone(WHITE, 4, 18);
       
    GoGame testGame(testBoard, black, white);
    testGame.currentPlayer = testGame.white;

    // Play capturing white stone
    GobanError capture = testGame.play(7, 18);

    EXPECT_EQ(testGame.goban.board()(5, 18), EMPTY);
    EXPECT_EQ(testGame.goban.board()(6, 18), EMPTY);
    EXPECT_EQ(capture, GobanError::kSuccess);
}

