#include <fstream>
#include <iostream>
#include <vector>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "ShaderProgram.h"
#include "gldebug.h"
#include "ArrayBuffer.h"
#include "buffer_loading.h"
#include "VertexArray.h"
#include "DrawData.h"
#include "UserContext.h"
#include "Light.h"
#include "lighting_utils.h"
#include "Texture.h"
#include "Framebuffer.h"
#include "utils.h"
#include "glfw_utils.h"
#include "draw_helpers.h"
#include "ScreenSize.h"

int main()
{
    constexpr glm::vec3 lookAt(0.0f);
    // HW_ITEM 3
    nsk_cg::UserContext userContext(lookAt, 10.0f);

    GLFWwindow* window = makeWindow(userContext, "Transparency");
    if (window == nullptr)
    {
        return EXIT_FAILURE;
    }

#ifndef NDEBUG
    nsk_cg::enableDebugOutput();
#endif

    nsk_cg::ShaderProgram ourShader;
    nsk_cg::ShaderProgram screenQuadShader;
    nsk_cg::ShaderProgram fxaaShader;
    try
    {
        ourShader = nsk_cg::createShader("GourandBlinn.vert", "Gourand.frag");
        screenQuadShader = nsk_cg::createShader("ScreenQuad.vert", "ScreenQuad.frag");
        fxaaShader = nsk_cg::createShader("ScreenQuad.vert", "fxaa.frag");
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "Failed to read shader file: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::invalid_argument& e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<nsk_cg::ArrayBuffer> arrayBuffers;
    std::vector<nsk_cg::ElementBuffer> elementBuffers;
    std::vector<nsk_cg::VertexArray> vertexArrays;

    nsk_cg::SceneObjects sceneObjects = prepareScene(arrayBuffers, elementBuffers, vertexArrays);
    nsk_cg::VertexArray screenQuadVao = loadScreenQuadBuffers(arrayBuffers, elementBuffers);

    const nsk_cg::Light light = {
        nsk_cg::LightType::Positional,
        { 5.0f, 5.0f, 5.0f },
        {0.0f, -1.0f, 0.0f},
        { 0.2f, 0.2f, 0.2f },
        { 0.5f, 0.5f, 0.5f },
        { 1.0f, 1.0f, 1.0f },
    };

    constexpr glm::vec3 backgroundColor(1.0f);
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);

    nsk_cg::Framebuffer framebuffer;
    nsk_cg::Texture colorTexture{nsk_cg::TextureFormat::Color, userContext.GetScreenWidth(), userContext.GetScreenHeight() };
    nsk_cg::Texture depthTexture{nsk_cg::TextureFormat::Depth, userContext.GetScreenWidth(), userContext.GetScreenHeight() };
    while (!glfwWindowShouldClose(window))
    {
        nsk_cg::processInput(window);

        const glm::mat4 viewMatrix = userContext.GetViewMatrix();
        ourShader.Use();
        setLightSourceToShader(light, ourShader, viewMatrix);

        glEnable(GL_DEPTH_TEST);

        framebuffer.Attach(colorTexture, depthTexture);
        framebuffer.Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (const auto& object : sceneObjects.GetOpaqueObjects())
        {
            const glm::mat4 projectionMatrix = userContext.GetProjection();
            drawObject(ourShader, viewMatrix, projectionMatrix, object);
        }
        for (const auto& object : sceneObjects.GetTransparentObjects())
        {
            const glm::mat4 projectionMatrix = userContext.GetProjection();
            drawObject(ourShader, viewMatrix, projectionMatrix, object);
        }

        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        drawQuad(fxaaShader, screenQuadVao, colorTexture);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
