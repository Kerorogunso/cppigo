#pragma once

#include "GLFWUIExports.h"
#include "board.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/math/constants/constants.hpp>
#include "Utilities/VectorT.h"
#include <future>
#include <memory>
#include <thread>

struct GLFWwindow;

constexpr double kPi = boost::math::constants::pi<double>();

class GLFWUI_DLL GLFWUI
{
public:

    struct UIOptions
    {
        bool fullscreen = false;
        int windowWidth = 400;
        int windowHeight = 300;

        float boardBorderFraction = 0.01f;
        
        ColourRGBf boardColour{ 219.f / 255.f, 158.f / 255.f, 46.f / 255.f };
        ColourRGBf gridColour{ 0.f, 0.f, 0.f };
    };

    GLFWUI();
    ~GLFWUI();
    
    // creates window and starts rendering in a separate thread (non-blocking)
    bool setupUI(const UIOptions &options);

    // shutdown the UI
    bool quitUI();

    bool updateBoard(const boost::numeric::ublas::matrix<int> &board);

private:
    void handleGlobalMouseEvents();
    void renderLoop();
    void drawBoard();
    void drawHorizontalGridlines();
    void drawVerticalGridlines();
    void setupDrawing();
    void drawStone(const Vector2f &centre, float radius, const ColourRGBf &colour);
    ColourRGBf stoneToColour(stones stone);

    Vector2f screenCoordsToBoard(const Vector2f &screenCoords);
    Vector2f boardCoordsToScreen(const Vector2f &boardCoords);

    Vector2f m_boardStart;
    float m_boardSide = 0.f;
    float m_squareSide = 0.f;

    struct BoardSize
    {
        int width = 1;
        int height = 1;
    } m_boardSize;

    GLFWwindow* m_window = nullptr;
    std::unique_ptr<boost::numeric::ublas::matrix<int>> m_board = nullptr;
    UIOptions m_options;
    bool m_setup = false;
    bool m_killRenderThread;
    std::unique_ptr<std::future<bool>> m_renderThread = nullptr;
};