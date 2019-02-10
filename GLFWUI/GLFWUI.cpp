// GLFWUI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "glfw3.h"
#include "GLFWUI.h"
#include "glfw3native.h"
#include <iostream>

using namespace boost::numeric::ublas;

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

bool GLFWUI::updateBoard(const boost::numeric::ublas::matrix<int> &board)
{
    *m_board = board;
    return true;
}

void GLFWUI::drawBoard()
{
    const int height = m_board->size1();
    const int width = m_board->size2();


    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // draw stuff
        }
    }
}

void GLFWUI::renderLoop()
{
    while (!m_killRenderThread)
    {
        glClearColor(0.f, 0.f, 0.f, 1.0f);
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