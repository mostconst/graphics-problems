#include <fstream>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "glfw_utils.h"
#include "ScreenSize.h"
#include "utils.h"

int main()
{
    constexpr int windowWidth = 800;
    constexpr int windowHeight = 600;

    const auto window = nsk_cg::makeMinimalWindow({ windowWidth, windowHeight }, "Save image");

    setColor(0.0f, 1.0f, 0.0f);

    //nsk_cg::Texture colorTexture(nsk_cg::TextureFormat::Color, windowWidth, windowHeight);
    //nsk_cg::Texture depthTexture(nsk_cg::TextureFormat::Depth, windowWidth, windowHeight);
    //nsk_cg::Framebuffer framebuffer;
    //framebuffer.Attach(colorTexture, depthTexture);
    while (!glfwWindowShouldClose(window))
    {
        draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
