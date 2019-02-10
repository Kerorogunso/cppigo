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

        GameState state;
        state.board = &go.goban.board();
        state.nextColour = go.currentPlayer.color;
        state.nextPlayer = go.currentPlayer.name;
        ui.updateBoard(state);
    });

    while (true)
    {
        GameState state;
        state.board = &go.goban.board();
        state.nextColour = go.currentPlayer.color;
        state.nextPlayer = go.currentPlayer.name;
        ui.updateBoard(state);

        std::string move;
        cout << "What's your move:" << endl;
        while (cin >> move) {};
        Vector2i parsedMove = parseMove(move);
        go.play(parsedMove);
    }
}