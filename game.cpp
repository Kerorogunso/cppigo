#include "game.h"

GoGame::GoGame()
{
    this->black = Player("Player 1", "black");
    this->white = Player("Player 2", "white");
};

GoGame::GoGame(Goban goban, Player black, Player white)
{
    board = goban;
    black = black;
    white = white;
};