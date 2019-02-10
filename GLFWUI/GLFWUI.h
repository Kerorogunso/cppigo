#pragma once

#include "GLFWUIExports.h"
#include "board.h"
#include <boost/numeric/ublas/matrix.hpp>
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

    GLFWUI();
    ~GLFWUI();
    
    // creates window and starts rendering in a separate thread (non-blocking)
    bool setupUI(const UIOptions &options);

    // shutdown the UI
    bool quitUI();

    bool updateBoard(const boost::numeric::ublas::matrix<int> &board);

private:
    void renderLoop();

    std::unique_ptr<boost::numeric::ublas::matrix<int>> m_board = nullptr;
    UIOptions m_options;
    bool m_setup = false;
    bool m_killRenderThread;
    std::unique_ptr<std::future<void>> m_renderThread = nullptr;
};