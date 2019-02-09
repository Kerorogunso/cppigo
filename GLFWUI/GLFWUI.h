#pragma once

#include "GLFWUIExports.h"

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

    bool setupUI(const UIOptions &options);
    bool quitUI();

private:
    UIOptions m_options;
    bool m_setup = false;
};