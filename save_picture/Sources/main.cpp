#include <fstream>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "UserContext.h"
#include "Texture.h"
#include "Framebuffer.h"

#include "glfw_utils.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double screenX, double screenY);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

int main()
{
    constexpr int windowWidth = 800;
    constexpr int windowHeight = 600;

    const auto window = nsk_cg::makeMinimalWindow({ windowWidth, windowHeight }, "Save image");

    // HW_ITEM 9
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    nsk_cg::Texture colorTexture(nsk_cg::TextureFormat::Color, windowWidth, windowHeight);
    nsk_cg::Texture depthTexture(nsk_cg::TextureFormat::Depth, windowWidth, windowHeight);
    nsk_cg::Framebuffer framebuffer;
    framebuffer.Attach(colorTexture, depthTexture);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
{
    auto* context = static_cast<nsk_cg::UserContext*>(glfwGetWindowUserPointer(window));
    context->OnWindowSizeChange(width, height);
}

void mouse_callback(GLFWwindow* window, const double screenX, const double screenY)
{
    auto* context = static_cast<nsk_cg::UserContext*>(glfwGetWindowUserPointer(window));
    context->OnMouseMove(screenX, screenY);
}

void mouse_button_callback(GLFWwindow* window, const int button, const int action, int)
{
    auto* context = static_cast<nsk_cg::UserContext*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        context->OnLeftMouseButtonAction(true);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        context->OnLeftMouseButtonAction(false);
    }
}
