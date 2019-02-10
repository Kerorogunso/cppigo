#pragma once

#include "GLFWUIExports.h"
#include "board.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <future>
#include <memory>
#include <thread>

struct GLFWwindow;

struct Vector3f
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

struct ColourRGBf
{
    float red = 0.f;
    float green = 0.f;
    float blue = 0.f;
};

struct Vector2f
{
    float x = 0.f;
    float y = 0.f;
};

class GLFWUI_DLL GLFWUI
{
public:

    struct UIOptions
    {
        bool fullscreen = false;
        int windowWidth = 400;
        int windowHeight = 300;

        float boardBorderFraction = 0.01f;
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
    void setupDrawing();
    void drawStone(const Vector2f &centre, float radius, const ColourRGBf &colour);

    GLFWwindow* m_window = nullptr;
    std::unique_ptr<boost::numeric::ublas::matrix<int>> m_board = nullptr;
    UIOptions m_options;
    bool m_setup = false;
    bool m_killRenderThread;
    std::unique_ptr<std::future<bool>> m_renderThread = nullptr;
};