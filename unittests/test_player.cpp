#include "pch.h"

#include "cppigo_lib/Player.h"
#include "cppigo_lib/GoGame.h"
#include "cppigo_lib/Board.h"

TEST(Player, CheckKomi)
{
    Player black("Player 1", "black");
    Player white("Player 2", "white");

    float blackKomi = black.getKomi();
    float whiteKomi = white.getKomi(); 

    // Test color parameter works.
    EXPECT_EQ(blackKomi, 0);
    EXPECT_EQ(whiteKomi, 7.5);
}