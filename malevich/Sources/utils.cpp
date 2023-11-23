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
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
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

MalevichCore::MalevichCore(const int windowWidth)
: m_window(makeMinimalWindow(windowWidth, windowWidth, "Title")),
  m_size(windowWidth)
{
    assert(m_window);
}


MalevichCore::~MalevichCore()
{
    glfwTerminate();
}


void MalevichCore::SetBackground(const Color& color)
{
    m_background = color;
}


void MalevichCore::SetSquareColor(const Color& color)
{
    m_squareColor = color;
}


void MalevichCore::Draw()
{
    glClearColor(m_background.r, m_background.g, m_background.b, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
    glColor3d(m_squareColor.r, m_squareColor.g, m_squareColor.b);
    glVertex3d(0.5f, 0.5f, 0.0f);
    glVertex3d(-0.5f, 0.5f, 0.0f);
    glVertex3d(-0.5f, -0.5f, 0.0f);
    glVertex3d(0.5f, -0.5f, 0.0f);
    glEnd();
}


SimpleImage MalevichCore::DrawToBuffer()
{
    Draw();
    glFinish();
    constexpr int components = 3;
    std::vector<unsigned char> buffer(m_size * m_size * components);
    glReadPixels(0, 0, m_size, m_size, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());

    return SimpleImage(std::move(buffer), m_size, m_size, components);
}
