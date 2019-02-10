#include <iostream>

#include "cppigo_lib/Player.h"
#include "cppigo_lib/GoGame.h"
#include "cppigo_lib/Board.h"
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
    ui.addBoardPositionClickedCallback([&go, &ui](Vector2i position)
    {
        go.play(position);
        ui.updateBoard(go.goban.board());
    });

    while (true)
    {
        ui.updateBoard(go.goban.board());

        std::string move;
        cout << "What's your move:" << endl;
        while (cin >> move) {};
        Vector2i parsedMove = parseMove(move);
        go.play(parsedMove);
    }
}