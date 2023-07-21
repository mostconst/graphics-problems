#include "utils.h"

#include <cassert>
#include <iostream>
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

GLFWwindow* makeMinimalWindow(const int width, const int height, const std::string_view& title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    GLFWwindow* const window = glfwCreateWindow(width, height, title.data(),
        nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    return window;
}

MalevichCore::MalevichCore(const int windowWidth, const int windowHeight)
: m_window(makeMinimalWindow(windowWidth, windowHeight, "Title")),
  windowWidth(windowWidth),
  windowHeight(windowHeight)
{
    assert(m_window);
}


MalevichCore::~MalevichCore()
{
    glfwTerminate();
}


void MalevichCore::SetColor(const float r, const float g, const float b)
{
    glClearColor(r, g, b, 1.0);
}

void MalevichCore::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


SimpleImage MalevichCore::DrawToBuffer()
{
    Draw();
    glFinish();
    constexpr int components = 3;
    std::vector<unsigned char> buffer(windowWidth * windowHeight * components);
    glReadPixels(0, 0, windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());

    return SimpleImage(std::move(buffer), windowWidth, windowHeight, components);
}
