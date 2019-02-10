#include "player.h"
#include "board.h"
#include "Utilities/VectorT.h"

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
    void play(const Vector2i &move);
    
private:
    void switchActivePlayer();
    bool checkForCapturedStones(int row, int col);
    void koCheck();
};

Vector2i parseMove(const string &move);