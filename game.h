#include "player.h"
#include "board.h"

using namespace std;
class GoGame
{
public:
    Goban goban;
    Player black;
    Player white;
    Player currentPlayer;
    std::vector<Goban> boardHistory;

    GoGame();
    GoGame(Goban board, Player black, Player white);
    GoGame(Goban board, Player black, Player white, std::vector<Goban> boardHistory);
    
    void makeMove(int row, int col);
    void play(int row, int col);
    void play(tuple<int, int> move);
    
private:
    void switchActivePlayer();
    bool checkForCapturedStones(int row, int col);
    void koCheck();
};

tuple<int, int> parseMove(string move);