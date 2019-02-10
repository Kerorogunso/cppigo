#include <iostream>

#include "player.h"
#include "board.h"
#include "game.h"
#include "GLFWUI/GLFWUI.h"

using namespace std;

int main()
{
    GoGame go;
    GLFWUI ui;
    GLFWUI::UIOptions uiOptions;
    uiOptions.fullscreen = false;
    uiOptions.windowHeight = 300;
    uiOptions.windowWidth = 400;

    ui.setupUI(uiOptions);

    while (true)
    {
        ui.updateBoard(go.goban.board());

        string move;
        cout << "What's your move:" << endl;
        cin >> move;
        tuple<int, int> parsedMove = parseMove(move);
        go.play(parsedMove);
    }
}