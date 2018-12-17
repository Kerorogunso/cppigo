#include "pch.h"

#include "../player.cpp"

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