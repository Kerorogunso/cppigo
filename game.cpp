#include "game.h"

GoGame::GoGame()
{
    this->board= Goban();
    this->black = Player("Player 1", "black");
    this->white = Player("Player 2", "white");
};

GoGame::GoGame(Goban goban, Player black, Player white)
{
    this->board= goban;
    this->black = black;
    this->white = white;
};