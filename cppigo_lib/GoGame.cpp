#include "stdafx.h"

#include <sstream>
#include "GoGame.h"

GoGame::GoGame()
{
    black = Player("Player 1", kBlack);
    white = Player("Player 2", kWhite);
    currentPlayer = black;
    boardHistory.push_back(goban);
};

GoGame::GoGame(Goban board, Player player1, Player player2)
{
    goban = board;
    black = player1;
    white = player2;
    currentPlayer = player1;
    boardHistory.push_back(board);
}

GoGame::GoGame(Goban board, Player black, Player white, std::vector<Goban> boardHistory)
{
    this->goban = board;
    this->black = black;
    this->white = white;
    this->boardHistory = boardHistory;
    this->currentPlayer = black;
}

void GoGame::makeMove(int row, int col)
{
    int stone = BLACK;
    if (currentPlayer == white)
    {
        stone = WHITE;
    }

    goban.placeStone(stone, row, col);
}

void GoGame::play(const Vector2i &move)
{
    int row = move.y;
    int col = move.x;
    GoGame::play(row, col);

}

void GoGame::play(int row, int col)
{
    try
    {
        makeMove(row, col);

        Group adjacentStones = getAdjacentSquares(row, col, goban.getBoardSize());

        bool captured = false;
        for (auto adjacent : adjacentStones)
        {
            captured = captured || checkForCapturedStones(std::get<0>(adjacent), std::get<1>(adjacent));
        }

        if (!captured)
        {
            goban.checkSelfAtari(row, col);
        }
        koCheck();
        std::cout << std::endl;
        boardHistory.push_back(this->goban);
        switchActivePlayer();
        std::cout << "Current player:" << currentPlayer.name << std::endl;
        goban.displayBoard();
    }
    catch (const std::logic_error &error)
    {
        std::cout << error.what() << std::endl;
    }
}

void GoGame::switchActivePlayer()
{
    if (currentPlayer == black)
    {
        currentPlayer = white;
    }
    else
    {
        currentPlayer = black;
    }
}

bool GoGame::checkForCapturedStones(int row, int col)
{
    Group stoneGroup = goban.returnNeighbors(row, col);

    int color = BLACK;
    if (currentPlayer.color == kBlack)
    {
        color = WHITE;
    }

    if (goban.getLiberties(stoneGroup) == 0)
    {
        for (auto coordinates : stoneGroup)
        {
            if (goban.board()(std::get<0>(coordinates), std::get<1>(coordinates)) == color)
            {
                goban.placeStone(EMPTY, coordinates);
            }
        }
        return true;
    }

    return false;
}

Vector2i parseMove(const std::string &move)
{
    std::stringstream stream(move);
    std::vector<int> coordinates;
    int n;
    try {
        while (stream >> n)
        {
            coordinates.push_back(n);
        }

        return Vector2i{ coordinates[0], coordinates[1] };
    }
    catch (const std::out_of_range& cor) {
        std::cout << "Could not parse move into tuple of ints" << std::endl;
    }
}

void GoGame::koCheck()
{
    Goban currentState = goban;

    if (find(boardHistory.begin(), boardHistory.end(), currentState) != boardHistory.end())
    {
        this->goban = boardHistory[boardHistory.size() - 1];
        throw std::logic_error("Ko rule.");
    }
}
