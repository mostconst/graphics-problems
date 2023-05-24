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
#include "depth_peeling.h"
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
    try
    {
        ourShader = nsk_cg::createShader("GourandBlinn.vert", "Gourand.frag");
        screenQuadShader = nsk_cg::createShader("ScreenQuad.vert", "ScreenQuad.frag");
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

    constexpr int nLayers = 3;
    nsk_cg::DepthPeelingResources depthPeelingResources({userContext.GetScreenWidth(), userContext.GetScreenHeight()}, nLayers);
    userContext.AddScreenSizeListener(&depthPeelingResources);
    nsk_cg::Framebuffer framebuffer;
    while (!glfwWindowShouldClose(window))
    {
        nsk_cg::processInput(window);

        glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::mat4 viewMatrix = userContext.GetViewMatrix();
        ourShader.Use();
        setLightSourceToShader(light, ourShader, viewMatrix);
        const glm::mat4 projectionMatrix = userContext.GetProjection();

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.GetRaw());
        framebuffer.Bind();
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //for (const auto& object : opaqueObjects)
        //{
        //    drawObject(ourShader, viewMatrix, projectionMatrix, object);
        //}
        const std::vector<nsk_cg::Texture>& colorTextures = depthPeelingResources.GetColorTextures();
        const auto& depthTextures = depthPeelingResources.GetDepthTextures();
        framebuffer.Attach(colorTextures[0], depthTextures[1]);
        glClearDepth(0.0);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClearDepth(1.0);

        for (int i = 0; i < nLayers; ++i)
        {
            framebuffer.Attach(colorTextures[i], depthTextures[i % 2]);
            framebuffer.Bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthTextures[(i + 1) % 2].GetRaw());
            for (const auto& object : sceneObjects.TransparentObjects())
            {
                drawObject(ourShader, viewMatrix, projectionMatrix, object);
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        drawLayers(screenQuadShader, screenQuadVao, colorTextures);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
