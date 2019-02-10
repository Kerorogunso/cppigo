#pragma once

#include "GLFWUIExports.h"
#include "board.h"

#include <future>
#include <memory>
#include <thread>

class GLFWUI_DLL GLFWUI
{
public:

    struct UIOptions
    {
        bool fullscreen = false;
        int windowWidth = 400;
        int windowHeight = 300;
    };

    GLFWUI(Goban *board);
    ~GLFWUI();
    
    // creates window and starts rendering in a separate thread (non-blocking)
    bool setupUI(const UIOptions &options);

    // shutdown the UI
    bool quitUI();

private:
    void renderLoop();

    Goban *m_board;
    UIOptions m_options;
    bool m_setup = false;
    bool m_killRenderThread;
    std::unique_ptr<std::future<void>> m_renderThread = nullptr;
};