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
}
void GoGame::play(int row, int col)
{
	try
	{
		this->board->placeStone(row, col);
	}
	catch(logic_error)
	{
		cout << "Invalid Move." << endl;
	}
	this->switchActivePlayer();

}

void GoGame::switchActivePlayer()
{
	if (this->currentPlayer == this->black)
	{
		this->currentPlayer = this->white;
	}
	else
	{
		this->currentPlayer = this->black;
	}
}

void GoGame::checkForCapturedStones(int row, int col)
{
	Group stoneGroup = this->board->getNeighbors(row, col);

	if (this->board->getLiberties(stoneGroup) == 0)
	{
		for (auto it = stoneGroup.begin(); it != stoneGroup.end(); ++it)
		{
			this->board.placeStone(EMPTY, *it);
		}
	}
}
