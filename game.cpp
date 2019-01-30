#include <sstream>
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
		stone = WHITE;
	}

	goban.placeStone(stone, row, col);
}

void GoGame::startGame()
{
	string move;
	do
	{
		cout << "What's your move: ";
		getline(cin, move);
		tuple<int, int> parsedMove = parseMove(move);

		try 
		{
			int row = get<0>(parsedMove);
			int col = get<1>(parsedMove);

			this->play(row, col);

			std::vector<tuple<int,int>> adjacentStones = {
				{max(row - 1, 0), col},
				{row, max(col - 1, 0)},
				{min(row + 1, this->goban.getBoardSize() - 1), col},
				{row, min(col + 1, this->goban.getBoardSize() - 1)}
			};

			for (auto adjacent : adjacentStones)
			{
				cout << get<0>(adjacent) << get<1>(adjacent) << endl;
				this->checkForCapturedStones(get<0>(adjacent), get<1>(adjacent));
			}
		}
		catch(logic_error)
		{
			cout << "Invalid Move." << endl;
			continue;
		}
		cout << endl;
		this->switchActivePlayer();
		cout << "Current player:" << this->currentPlayer.color << endl;
		this->goban.displayBoard();
	} while (move != "quit");

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

tuple<int, int> parseMove(string move)
{
	stringstream stream(move);
	std::vector<int> coordinates;
	int n;
	while (stream >> n)
	{
		coordinates.push_back(n);
	}

	return make_tuple(coordinates[0], coordinates[1]);
}
