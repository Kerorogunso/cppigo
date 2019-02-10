#include "pch.h"

#include <random>
#include <chrono>
#include <thread>
#include "cppigo_lib/GoGame.h"
#include "GLFWUI/GLFWUI.h"

TEST(GLFWUI, checkUIStarts)
{
    GLFWUI ui;
    GLFWUI::UIOptions options;
    ASSERT_TRUE(ui.setupUI(options));
    std::this_thread::sleep_for(std::chrono::seconds(2));
    ASSERT_TRUE(ui.quitUI());
}