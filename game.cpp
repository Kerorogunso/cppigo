#include <sstream>
#include "game.h"

GoGame::GoGame()
{
    black = Player("Player 1", "black");
    white = Player("Player 2", "white");
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

void GoGame::play(int row, int col)
{
    int stone = BLACK;
    if (currentPlayer == white)
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

        try
        {
            tuple<int, int> parsedMove = parseMove(move);
            int row = get<0>(parsedMove);
            int col = get<1>(parsedMove);

            play(row, col);
            cout << this->goban.getLiberties({ make_tuple(1,1) });

            Group adjacentStones = getAdjacentSquares(row, col, goban.getBoardSize());
            
            bool captured = false;
            for (auto adjacent : adjacentStones)
            {
                cout << get<0>(adjacent) << "," << get<1>(adjacent) << endl;
                captured = captured || checkForCapturedStones(get<0>(adjacent), get<1>(adjacent));
            }

            if (!captured)
            {
                goban.checkSelfAtari(row, col);
            }
            koCheck();
        }
        catch (const logic_error &error)
        {
            cout << error.what() << endl;
            continue;
        }
        cout << endl;
        boardHistory.push_back(this->goban);
        switchActivePlayer();
        cout << "Current player:" << currentPlayer.color << endl;
        goban.displayBoard();
    } while (move != "quit");

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
    if (currentPlayer.color == "black")
    {
        color = WHITE;
    }
    
    if (goban.getLiberties(stoneGroup) == 0)
    {
        for (auto coordinates : stoneGroup)
        {
            if (goban.board()(get<0>(coordinates), get<1>(coordinates)) == color) 
            {
                goban.placeStone(EMPTY, coordinates);
            }
        }
        return true;
    }

    return false;
}

tuple<int, int> parseMove(string move)
{
    stringstream stream(move);
    std::vector<int> coordinates;
    int n;
    try {
        while (stream >> n)
        {
            coordinates.push_back(n);
        }

        return make_tuple(coordinates[0], coordinates[1]);
    }
    catch(const out_of_range& cor) {
        cout << "Could not parse move into tuple of ints" << endl;
    }
}

void GoGame::koCheck()
{
    Goban currentState = goban;
    
    if (find(boardHistory.begin(), boardHistory.end(), currentState) != boardHistory.end())
    {
        this->goban = boardHistory[boardHistory.size() - 1];
        throw logic_error("Ko rule.");
    }
}    
