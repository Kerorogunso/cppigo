// GLFWUI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "glfw3.h"
#include "GLFWUI.h"
#include "Utilities/RectangleT.h"
#include "glfw3native.h"
#include <iostream>

using namespace boost::numeric::ublas;

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
///////////////////////////////////////////////////////
Vector2f gLastMousePosition;
std::queue<MouseEvent> gMouseEvents;
///////////////////////////////////////////////////////

ColourRGBf GLFWUI::stoneToColour(stones stone)
{
    switch (stone)
    {
    case stones::BLACK:
        return{ 0.f, 0.f, 0.f };
    case stones::EMPTY:
        return m_options.boardColour;
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

    constexpr int numVertices = 20;
    for (int i = 0; i < numVertices; ++i)
    {
        const float angle = 2.f * kPi * static_cast<float>(i) / numVertices;
        const float xPos = centre.x + radius * std::sin(angle);
        const float yPos = centre.y + radius * std::cos(angle);
        glVertex2f(xPos, yPos);
    }

    glEnd();
}

Vector2f GLFWUI::screenCoordsToBoard(const Vector2f &screenCoords)
{
    return {};
}

Vector2f GLFWUI::boardCoordsToScreen(const Vector2f &boardCoords)
{
    Vector2f screenCoords;
    screenCoords.x = m_boardStart.x + (m_boardSide / m_boardSize.width) * boardCoords.x;
    screenCoords.y = m_boardStart.y + (m_boardSide / m_boardSize.width) * boardCoords.y;
    return screenCoords;
}

void GLFWUI::drawHorizontalGridlines()
{
    glLineWidth(1.f);
    for (int y = 0; y <= m_boardSize.height; ++y)
    {
        glColor3f(m_options.gridColour.red, m_options.gridColour.green, m_options.gridColour.blue);
        glBegin(GL_LINES);
        glVertex2f(m_boardStart.x, m_boardStart.y + m_squareSide * y);
        glVertex2f(m_boardStart.x + m_boardSide, m_boardStart.y + m_squareSide * y);
        glEnd();
    }
}

void GLFWUI::drawVerticalGridlines()
{
    for (int x = 0; x <= m_boardSize.width; ++x)
    {
        glColor3f(m_options.gridColour.red, m_options.gridColour.green, m_options.gridColour.blue);
        glBegin(GL_LINES);
        glVertex2f(m_boardStart.x + m_squareSide * x, m_boardStart.y);
        glVertex2f(m_boardStart.x + m_squareSide * x, m_boardStart.y + m_boardSide);
        glEnd();
    }
}

void GLFWUI::drawBoard()
{
    m_boardSize.height = m_board->size1();
    m_boardSize.width = m_board->size2();

    int windowWidth = 0;
    int windowHeight = 0;
    glfwGetFramebufferSize(m_window, &windowWidth, &windowHeight);
    m_options.windowHeight = windowHeight;
    m_options.windowWidth = windowWidth;

    setupDrawing();

    const int boardSideNoBorder = std::min(windowWidth, windowHeight);
    const float borderThickness = m_options.boardBorderFraction * boardSideNoBorder;
    m_boardSide = static_cast<float>(boardSideNoBorder) - 2.f * borderThickness;
    m_boardStart.x = .5f * (static_cast<float>(windowWidth) - m_boardSide);
    m_boardStart.y = .5f * (static_cast<float>(windowHeight) - m_boardSide);
    m_squareSide = m_boardSide / m_boardSize.width;

    drawHorizontalGridlines();
    drawVerticalGridlines();

    for (int y = 0; y < m_boardSize.height; ++y)
    {
        for (int x = 0; x < m_boardSize.width; ++x)
        {
            const stones stone = stones::WHITE;// static_cast<stones>((*m_board)(y, x));
            if (stone == stones::EMPTY)
            {
                continue;
            }

            const ColourRGBf colour = stoneToColour(stone);
            const float radius = 0.4f * m_squareSide;

            // in board coordinates (0,0) means the top left coordinate of the square (0,0) but
            // we want to draw in the middle, so we add 0.5 to x and y
            const Vector2f centre_boardCoords{ static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f };
            const Vector2f cente = boardCoordsToScreen(centre_boardCoords);
            drawStone(cente, radius, colour);
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
        glClearColor(m_options.boardColour.red, m_options.boardColour.green, m_options.boardColour.blue, 1.0f);
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