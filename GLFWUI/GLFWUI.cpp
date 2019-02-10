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
    Vector2d position;
    MouseButton button;
    MouseAction action;
};

// global things to interact with GLFW (yuck!)
///////////////////////////////////////////////////////
Vector2d gLastMousePosition;
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
    mouseEvent.position = gLastMousePosition;
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

bool GLFWUI::updateBoard(const GameState &newState)
{
    m_gameState = newState;
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

void GLFWUI::drawStone(const Vector2d &centre, double radius, const ColourRGBf &colour)
{
    const double left = centre.x - radius;
    const double right = centre.x + radius;
    const double top = centre.y - radius;
    const double bottom = centre.y + radius;

    glColor3f(colour.red, colour.green, colour.blue);
    glBegin(GL_POLYGON);

    constexpr int numVertices = 20;
    for (int i = 0; i < numVertices; ++i)
    {
        const double angle = 2.0 * kPi * static_cast<double>(i) / numVertices;
        const double xPos = centre.x + radius * std::sin(angle);
        const double yPos = centre.y + radius * std::cos(angle);
        glVertex2d(xPos, yPos);
    }

    glEnd();
}

Vector2i GLFWUI::getNearestGridPoint(const Vector2d &point)
{
    Vector2d boardPosition = screenCoordsToBoard(point);
    boardPosition.x += .5;
    boardPosition.y += .5;

    const Vector2i boardPositionInteger{
        static_cast<int>(boardPosition.x),
        static_cast<int>(boardPosition.y)
    };

    return boardPositionInteger;
}

Vector2d GLFWUI::screenCoordsToBoard(const Vector2d &screenCoords)
{
    if (m_boardSide == 0.f)
    {
        return {};
    }

    Vector2d boardCoords;
    boardCoords.x = (screenCoords.x - m_boardStart.x) * (m_boardSize.width / m_boardSide);
    boardCoords.y = (screenCoords.y - m_boardStart.y) * (m_boardSize.height / m_boardSide);
    return boardCoords;
}

Vector2d GLFWUI::boardCoordsToScreen(const Vector2d &boardCoords)
{
    if (m_boardSize.width == 0.0 || m_boardSize.height == 0.0)
    {
        return {};
    }

    Vector2d screenCoords;
    screenCoords.x = m_boardStart.x + (m_boardSide / m_boardSize.width) * boardCoords.x;
    screenCoords.y = m_boardStart.y + (m_boardSide / m_boardSize.height) * boardCoords.y;
    return screenCoords;
}

void GLFWUI::drawHorizontalGridlines()
{
    glLineWidth(1.f);
    for (int y = 0; y <= m_boardSize.height; ++y)
    {
        glColor3f(m_options.gridColour.red, m_options.gridColour.green, m_options.gridColour.blue);
        glBegin(GL_LINES);
        glVertex2d(m_boardStart.x, m_boardStart.y + m_squareSide * y);
        glVertex2d(m_boardStart.x + m_boardSide, m_boardStart.y + m_squareSide * y);
        glEnd();
    }
}

void GLFWUI::drawVerticalGridlines()
{
    for (int x = 0; x <= m_boardSize.width; ++x)
    {
        glColor3f(m_options.gridColour.red, m_options.gridColour.green, m_options.gridColour.blue);
        glBegin(GL_LINES);
        glVertex2d(m_boardStart.x + m_squareSide * x, m_boardStart.y);
        glVertex2d(m_boardStart.x + m_squareSide * x, m_boardStart.y + m_boardSide);
        glEnd();
    }
}

void GLFWUI::updateForNewScreenSize()
{
    m_boardSize.height = static_cast<int>(m_gameState.board->size1());
    m_boardSize.width = static_cast<int>(m_gameState.board->size2());

    int windowWidth = 0;
    int windowHeight = 0;
    glfwGetFramebufferSize(m_window, &windowWidth, &windowHeight);
    m_options.windowHeight = windowHeight;
    m_options.windowWidth = windowWidth;

    setupDrawing();

    const int boardSideNoBorder = std::min(windowWidth, windowHeight);
    const float borderThickness = m_options.boardBorderFraction * boardSideNoBorder;
    m_boardSide = static_cast<double>(boardSideNoBorder) - 2.f * borderThickness;
    m_boardStart.x = .5f * (static_cast<double>(windowWidth) - m_boardSide);
    m_boardStart.y = .5f * (static_cast<double>(windowHeight) - m_boardSide);
    m_squareSide = m_boardSide / m_boardSize.width;
}

void GLFWUI::drawBoard()
{
    updateForNewScreenSize();
    drawHorizontalGridlines();
    drawVerticalGridlines();

    for (int y = 0; y < m_boardSize.height; ++y)
    {
        for (int x = 0; x < m_boardSize.width; ++x)
        {
            const stones stone = static_cast<stones>((*m_gameState.board)(y, x));
            if (stone == stones::EMPTY)
            {
                continue;
            }

            const ColourRGBf colour = stoneToColour(stone);
            const double radius = 0.5f * m_options.stoneDiameterAsFractionOfGridCell * m_squareSide;

            const Vector2d centre_boardCoords{ static_cast<double>(x), static_cast<double>(y) };
            const Vector2d cente = boardCoordsToScreen(centre_boardCoords);
            drawStone(cente, radius, colour);
        }
    }
}

void GLFWUI::addBoardPositionClickedCallback(std::function<void(Vector2i)> callback)
{
    m_boardPositionClickedCallbacks.push_back(callback);
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
        {
            const Vector2i boardPositionInteger = getNearestGridPoint(event.position);

            if (boardPositionInteger.x >= 0 && boardPositionInteger.x < m_boardSize.width &&
                boardPositionInteger.y >= 0 && boardPositionInteger.y < m_boardSize.height)
            {
                for (auto callback : m_boardPositionClickedCallbacks)
                {
                    callback(boardPositionInteger);
                }
            }
            break;
        }
        case MouseAction::kRelease:
            break;
        }
    }
}

void GLFWUI::drawStoneAtNearestGridPointToMouse()
{
    const Vector2i boardPositionInteger = getNearestGridPoint(gLastMousePosition);

    const int boardX = boardPositionInteger.x;
    const int boardY = boardPositionInteger.y;
    if (boardX >= 0 && boardX < m_boardSize.width &&
        boardY >= 0 && boardY < m_boardSize.height)
    {
        const double radius = (m_boardSide / m_boardSize.width) * m_options.stoneDiameterAsFractionOfGridCell * 0.5f;
        const Vector2d posiion = boardCoordsToScreen({ static_cast<double>(boardX), static_cast<double>(boardY) });
        drawStone(posiion, radius, m_gameState.nextColour);
    }
}

void GLFWUI::renderLoop()
{
    while (!m_killRenderThread)
    {
        glClearColor(m_options.boardColour.red, m_options.boardColour.green, m_options.boardColour.blue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        if (m_gameState.board)
        {
            drawBoard();
            drawStoneAtNearestGridPointToMouse();
        }

        glfwPollEvents();
        handleGlobalMouseEvents();

        m_killRenderThread = glfwWindowShouldClose(m_window);

        glfwSwapBuffers(m_window);

        glFinish();

        //std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}