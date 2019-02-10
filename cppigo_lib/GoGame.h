#include "player.h"
#include "Board.h"
#include "Utilities/VectorT.h"
#include "cppigo_exports.h"

class CPPIGO_DLL GoGame
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

Vector2i CPPIGO_DLL parseMove(const std::string &move);