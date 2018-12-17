#include "game.h"

GoGame::GoGame()
{
    this->black = Player("Player 1", "black");
    this->white = Player("Player 2", "white");
	this->currentPlayer = black;
};

GoGame::GoGame(Goban goban, Player player1, Player player2)
{
    board = goban;
    black = player1;
    white = player2;
	currentPlayer = player1;
};