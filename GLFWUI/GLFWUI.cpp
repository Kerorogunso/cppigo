// GLFWUI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "glfw3.h"
#include "GLFWUI.h"
#include "glfw3native.h"
#include <iostream>

using namespace boost::numeric::ublas;

constexpr ColourRGBf boardColour{ 219.f / 255.f, 158.f / 255.f, 46.f / 255.f };
constexpr ColourRGBf gridColour{ 0.f, 0.f, 0.f };

enum class MouseButton
{
    kLeft = GLFW_MOUSE_BUTTON_LEFT,
    kRight = GLFW_MOUSE_BUTTON_RIGHT
};

enum class MouseAction
{
    kPress = GLFW_PRESS,
    kRelease = GLFW_RELEASE
};

struct MouseEvent
{
    MouseButton button;
    MouseAction action;
};

// global things to interact with GLFW (yuck!)
Vector2f gLastMousePosition;
std::queue<MouseEvent> gMouseEvents;

ColourRGBf stoneToColour(stones stone)
{
    switch (stone)
    {
    case stones::BLACK:
        return{ 0.f, 0.f, 0.f };
    case stones::EMPTY:
        return boardColour;
    case stones::WHITE:
        return { 1.f, 1.f, 1.f };
    }
}

GLFWUI::GLFWUI()
{
}

GLFWUI::~GLFWUI()
{
    quitUI();
}

void window_size_callback(GLFWwindow *window, int width, int height)
{

}

void set_frame_size_callback(GLFWwindow *window, int width, int height)
{

}

void mouseMoveCallback(GLFWwindow *window, double xPos, double yPos)
{
    gLastMousePosition.x = xPos;
    gLastMousePosition.y = yPos;
}

void mouseButtonCallback(GLFWwindow *window, int mouseButton, int action, int modifierKeys)
{
    MouseEvent mouseEvent;
    mouseEvent.button = static_cast<MouseButton>(mouseButton);
    mouseEvent.action = static_cast<MouseAction>(action);
    gMouseEvents.push(std::move(mouseEvent));
}

bool GLFWUI::setupUI(const UIOptions &options)
{
    m_options = options;

    m_killRenderThread = false;
    m_renderThread = std::make_unique<std::future<bool>>(std::async([&]()
    {
        if (!glfwInit())
        {
            return false;
        }

        m_window = glfwCreateWindow(m_options.windowWidth, m_options.windowHeight, "Hello World", nullptr, nullptr);
        if (!m_window)
        {
            glfwTerminate();
            return false;
        }

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwSetCursorPosCallback(m_window, mouseMoveCallback);
        glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
        glfwSetFramebufferSizeCallback(m_window, set_frame_size_callback);
        glfwSetWindowSizeCallback(m_window, window_size_callback);
        glfwMakeContextCurrent(m_window);

        setupDrawing();
        renderLoop();

        glfwTerminate();
        return true;
    }));

    m_setup = true;
    return true;
}

bool GLFWUI::quitUI()
{
    if (m_setup)
    {
        m_killRenderThread = true;
        if (m_renderThread)
        {
            m_renderThread->wait();
        }
    }
    return true;
}

bool GLFWUI::updateBoard(const matrix<int> &board)
{
    if (m_board == nullptr)
    {
        m_board = std::make_unique< matrix<int>>(board);
    }
    else
    {
        *m_board = board;
    }
    return true;
}

void GLFWUI::setupDrawing()
{
    glViewport(0, 0, m_options.windowWidth, m_options.windowHeight);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    glOrtho(0, m_options.windowWidth, m_options.windowHeight, 0, 100, -100);
    glMatrixMode(GL_MODELVIEW);
}

void GLFWUI::drawStone(const Vector2f &centre, float radius, const ColourRGBf &colour)
{
    const float left = centre.x - radius;
    const float right = centre.x + radius;
    const float top = centre.y - radius;
    const float bottom = centre.y + radius;

    glColor3f(colour.red, colour.green, colour.blue);
    glBegin(GL_POLYGON);
    glVertex2f(left, top);
    glVertex2f(right, top);
    glVertex2f(right, bottom);
    glVertex2f(left, bottom);
    glEnd();
}

void GLFWUI::drawBoard()
{
    const int boardWidth = m_board->size1();
    const int boardHeight = m_board->size2();

    int windowWidth = 0;
    int windowHeight = 0;
    glfwGetFramebufferSize(m_window, &windowWidth, &windowHeight);
    m_options.windowHeight = windowHeight;
    m_options.windowWidth = windowWidth;

    setupDrawing();

    const int boardHeightNoBorder = m_board->size1();
    const int boardWidthNoBorder = m_board->size2();

    const int boardSideNoBorder = std::min(windowWidth, windowHeight);
    const float borderThickness = m_options.boardBorderFraction * boardSideNoBorder;
    const float boardSide = static_cast<float>(boardSideNoBorder) - 2.f * borderThickness;
    const float xBoardStart = .5f * (static_cast<float>(windowWidth) - boardSide);
    const float yBoardStart = .5f * (static_cast<float>(windowHeight) - boardSide);

    const float squareSide = boardSide / boardWidth;

    glLineWidth(1.f);
    for (int y = 0; y <= boardHeight; ++y)
    {
        glColor3f(gridColour.red, gridColour.green, gridColour.blue);
        glBegin(GL_LINES);
        glVertex2f(xBoardStart, yBoardStart + squareSide * y);
        glVertex2f(xBoardStart + boardSide, yBoardStart + squareSide * y);
        glEnd();
    }

    for (int x = 0; x <= boardWidth; ++x)
    {
        glColor3f(gridColour.red, gridColour.green, gridColour.blue);
        glBegin(GL_LINES);
        glVertex2f(xBoardStart + squareSide * x, yBoardStart);
        glVertex2f(xBoardStart + squareSide * x, yBoardStart + boardSide);
        glEnd();
    }

    for (int y = 0; y < boardHeight; ++y)
    {
        for (int x = 0; x < boardWidth; ++x)
        {
            const stones stone = static_cast<stones>((*m_board)(y, x));
            if (stone == stones::EMPTY)
            {
                continue;
            }

            const float centreX = xBoardStart + squareSide * (0.5f + x);
            const float centreY = yBoardStart + squareSide * (0.5f + y);
            const ColourRGBf colour = stoneToColour(stone);
            const float radius = 0.4f * squareSide;

            drawStone({ centreX, centreY }, radius, colour);
        }
    }
}

void GLFWUI::handleGlobalMouseEvents()
{
    while (gMouseEvents.size())
    {
        const MouseEvent event = gMouseEvents.front();
        gMouseEvents.pop();

        switch (event.action)
        {
        case MouseAction::kPress:
            // pass to game
            break;
        case MouseAction::kRelease:
            break;
        }
    }
}

void GLFWUI::renderLoop()
{
    while (!m_killRenderThread)
    {
        glClearColor(boardColour.red, boardColour.green, boardColour.blue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawStone(gLastMousePosition, 5.f, { 1.f, 0.f, 0.f });

        if (m_board)
        {
            drawBoard();
        }

        glfwPollEvents();
        handleGlobalMouseEvents();

        m_killRenderThread = glfwWindowShouldClose(m_window);

        glfwSwapBuffers(m_window);

        glFinish();

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}