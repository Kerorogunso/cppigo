// GLFWUI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "glfw3.h"
#include "GLFWUI.h"
#include "glfw3native.h"
#include <iostream>

using namespace boost::numeric::ublas;

struct Vector3f
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

using ColourRGB = Vector3f;
constexpr ColourRGB boardColour{ 219.f / 255.f, 158.f / 255.f, 46.f / 255.f };
constexpr ColourRGB gridColour{ 0.f, 0.f, 0.f };

ColourRGB stoneToColour(stones stone)
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

struct Vector2f
{
    float x = 0.f;
    float y = 0.f;
};

void GLFWUI::setupDrawing()
{
    glViewport(0, 0, m_options.windowWidth, m_options.windowHeight);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    glOrtho(0, m_options.windowWidth, m_options.windowHeight, 0, 100, -100);
    glMatrixMode(GL_MODELVIEW);
}

void GLFWUI::drawBoard()
{
    const int boardWidth = m_board->size1();
    const int boardHeight = m_board->size2();

    const int boardHeightNoBorder = m_board->size1();
    const int boardWidthNoBorder = m_board->size2();

    const int boardSideNoBorder = std::min(m_options.windowWidth, m_options.windowHeight);
    const float borderThickness = m_options.boardBorderFraction * boardSideNoBorder;
    const float boardSide = static_cast<float>(boardSideNoBorder) - 2.f * borderThickness;
    const float xBoardStart = .5f * (static_cast<float>(m_options.windowWidth) - boardSide);
    const float yBoardStart = .5f * (static_cast<float>(m_options.windowHeight) - boardSide);

    const float squareSide = boardSide / boardWidth;

    glLineWidth(1.f);
    for (int y = 0; y <= boardHeight; ++y)
    {
        glColor3f(gridColour.x, gridColour.y, gridColour.z);
        glBegin(GL_LINES);
        glVertex2f(xBoardStart, yBoardStart + squareSide * y);
        glVertex2f(xBoardStart + boardSide, yBoardStart + squareSide * y);
        glEnd();
    }

    for (int x = 0; x <= boardWidth; ++x)
    {
        glColor3f(gridColour.x, gridColour.y, gridColour.z);
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

            const float left = xBoardStart + squareSide * x;
            const float right = xBoardStart + squareSide * (x + 1);
            const float top = yBoardStart + squareSide * y;
            const float bottom = yBoardStart + squareSide * (y + 1);
            const ColourRGB colour = stoneToColour(stone);

            glColor3f(colour.x, colour.y, colour.z);
            glBegin(GL_POLYGON);
            glVertex2f(left, top);
            glVertex2f(right, top);
            glVertex2f(right, bottom);
            glVertex2f(left, bottom);
            glEnd();
        }
    }
}

void GLFWUI::renderLoop()
{
    while (!m_killRenderThread)
    {
        glClearColor(boardColour.x, boardColour.y, boardColour.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_board)
        {
            drawBoard();
        }

        glfwPollEvents();

        m_killRenderThread = glfwWindowShouldClose(m_window);

        glfwSwapBuffers(m_window);

        glFinish();

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}