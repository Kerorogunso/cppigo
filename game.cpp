#include "game.h"

GoGame::GoGame()
{
    this->black = Player("Player 1", "black");
    this->white = Player("Player 2", "white");
	this->currentPlayer = black;
};

GoGame::GoGame(Goban board, Player player1, Player player2)
{
    goban = board;
    black = player1;
    white = player2;
	currentPlayer = player1;
}
void GoGame::play(int row, int col)
{
	int stone = BLACK;
	if (this->currentPlayer == this->white)
	{
		int stone = WHITE;
	}

	try
	{
		goban.placeStone(stone, row, col);
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
	Group stoneGroup = this->goban.returnNeighbors(row, col);
	Player currentPlayer = this->currentPlayer;
	
	int color = BLACK;
	if (currentPlayer.color == "black")
	{
		color = WHITE;
	}
	
	if (this->goban.getLiberties(stoneGroup) == 0)
	{
		for (auto coordinates : stoneGroup)
		{
			if (this->goban.board(get<0>(coordinates), get<1>(coordinates)) != color) 
			{
				this->goban.placeStone(EMPTY, coordinates);
			}
		}
	}
}
