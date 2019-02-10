#pragma once

#include "GLFWUIExports.h"
#include "Utilities/Colours.h"
#include "cppigo_lib/Board.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/math/constants/constants.hpp>
#include "Utilities/VectorT.h"
#include <future>
#include <memory>
#include <thread>

struct GLFWwindow;

constexpr double kPi = boost::math::constants::pi<double>();

struct GameState
{
    const boost::numeric::ublas::matrix<int> *board = nullptr;
    ColourRGBf nextColour;
    std::string nextPlayer;
};

class GLFWUI_DLL GLFWUI
{
public:

    struct UIOptions
    {
        bool fullscreen = false;
        int windowWidth = 400;
        int windowHeight = 300;

        double boardBorderFraction = 0.01f;
        double stoneDiameterAsFractionOfGridCell = 0.8f;
        
        ColourRGBf boardColour{ 219.f / 255.f, 158.f / 255.f, 46.f / 255.f };
        ColourRGBf gridColour{ 0.f, 0.f, 0.f };
    };

    GLFWUI();
    ~GLFWUI();

    void addBoardPositionClickedCallback(std::function<void(Vector2i)> callback);
    
    // creates window and starts rendering in a separate thread (non-blocking)
    bool setupUI(const UIOptions &options);

    // shutdown the UI
    bool quitUI();

    bool updateBoard(const GameState &newState);

private:
    void handleGlobalMouseEvents();
    void renderLoop();
    void drawBoard();
    void drawHorizontalGridlines();
    void drawVerticalGridlines();
    void setupDrawing();
    void updateForNewScreenSize();
    void drawStone(const Vector2d &centre, double radius, const ColourRGBf &colour);
    ColourRGBf stoneToColour(stones stone);

    Vector2d screenCoordsToBoard(const Vector2d &screenCoords);
    Vector2d boardCoordsToScreen(const Vector2d &boardCoords);

    Vector2d m_boardStart;
    double m_boardSide = 0.f;
    double m_squareSide = 0.f;

    struct BoardSize
    {
        int width = 1;
        int height = 1;
    } m_boardSize;

    std::vector<std::function<void(Vector2i)>> m_boardPositionClickedCallbacks;

    GLFWwindow* m_window = nullptr;

    GameState m_gameState;
    UIOptions m_options;
    bool m_setup = false;
    bool m_killRenderThread;
    std::unique_ptr<std::future<bool>> m_renderThread = nullptr;
};