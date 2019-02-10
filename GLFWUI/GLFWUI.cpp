// GLFWUI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "GLFWUI.h"
#include "glfw3.h"
#include "glfw3native.h"
#include <iostream>

GLFWUI::GLFWUI(Goban *board)
    : m_board(board)
{
}

GLFWUI::~GLFWUI()
{
    quitUI();
}

bool GLFWUI::setupUI(const UIOptions &options)
{
    m_options = options;

    GLFWwindow* window;

    if (!glfwInit())
    {
        return false;
    }

    int screenCount;
    GLFWmonitor** monitors = glfwGetMonitors(&screenCount);

    int screenIndex = screenCount - 1;
    GLFWmonitor *screen = monitors[screenIndex];

    window = glfwCreateWindow(m_options.windowWidth, m_options.windowHeight, "Hello World", screen, NULL);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwPollEvents();

    m_killRenderThread = false;
    m_renderThread = std::make_unique<std::future<void>>(std::async([&]()
    {
        renderLoop();
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

        glfwTerminate();
    }
    return true;
}

void GLFWUI::renderLoop()
{
    while (!m_killRenderThread)
    {
        glfwPollEvents();
    }
}