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

GobanError GoGame::makeMove(int row, int col)
{
    int stone = BLACK;
    if (currentPlayer.color == kWhite)
    {
        stone = WHITE;
    }

    return goban.placeStone(stone, row, col);
}

GobanError GoGame::play(const Vector2i &move)
{
    int row = move.y;
    int col = move.x;
    return GoGame::play(row, col);

}

GobanError GoGame::play(int row, int col)
{
    GobanError moveCheck = makeMove(row, col);
    if (moveCheck != GobanError::kSuccess)
    {
        return moveCheck;
    }

    Group adjacentStones = getAdjacentSquares(row, col, goban.getBoardSize());

    for (auto adjacent : adjacentStones)
    {
        checkForCapturedStones(std::get<0>(adjacent), std::get<1>(adjacent));
    }
    
    GobanError selfAtariCheck = checkSelfAtari(row, col);
    if (selfAtariCheck != GobanError::kSuccess)
    {
        return selfAtariCheck;
    }
    
    GobanError koRuleCheck = koCheck();
    if (koRuleCheck != GobanError::kSuccess)
    {
        return koRuleCheck;
    }
    boardHistory.push_back(this->goban);
    switchActivePlayer();
    goban.displayBoard();
    return GobanError::kSuccess;
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

GobanError GoGame::checkSelfAtari(int row, int col)
{
    Group stoneGroup = goban.returnNeighbors(row, col);
    int liberties = goban.getLiberties(stoneGroup);

    if (liberties == 0)
    {
        goban = this->boardHistory[boardHistory.size() - 1];
        return GobanError::kSelfAtari;
    }
    return GobanError::kSuccess;
}

GobanError GoGame::koCheck()
{
    Goban currentState = goban;

    if (find(boardHistory.begin(), boardHistory.end(), currentState) != boardHistory.end())
    {
        this->goban = boardHistory[boardHistory.size() - 1];
        return GobanError::kKoRule;
    }
    return GobanError::kSuccess;
}
