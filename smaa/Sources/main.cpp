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
#include "SearchTex.h"
#include "AreaTex.h"
#include "Image.h"

int main()
{
    constexpr glm::vec3 lookAt(0.0f);
    // HW_ITEM 3
    nsk_cg::UserContext userContext{ lookAt, 10.0f, nsk_cg::ScreenSize{1280, 720} };

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
    nsk_cg::ShaderProgram smaaEdgeDetectionShader;
    nsk_cg::ShaderProgram smaaEdgeDetectionShaderInclude;
    nsk_cg::ShaderProgram smaaBlendingShader;
    nsk_cg::ShaderProgram smaaBlendingShaderInclude;
    try
    {
        ourShader = nsk_cg::createShader("GourandBlinn.vert", "Gourand.frag");
        screenQuadShader = nsk_cg::createShader("ScreenQuad.vert", "ScreenQuad.frag");
        fxaaShader = nsk_cg::createShader("ScreenQuad.vert", "fxaa.frag");
        smaaEdgeDetectionShader = nsk_cg::createShader("SMAAEdgeDetection.vert", "SMAALumaEdgeDetection.frag");
        smaaEdgeDetectionShaderInclude = nsk_cg::createShader("SMAAEdgeDetectionComplete.vert", "SMAAEdgeDetectionComplete.frag");
        smaaBlendingShader = nsk_cg::createShader("SMAABlendingWeightCalculation.vert", "SMAABlendingWeightCalculation.frag");
        smaaBlendingShaderInclude = nsk_cg::createShader("SMAABlendingWeightCalculationComplete.vert", "SMAABlendingWeightCalculationComplete.frag");
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

    smaaBlendingShaderInclude.Use();
    smaaBlendingShaderInclude.SetInt("edgesTex", 0);
    smaaBlendingShaderInclude.SetInt("areaTex", 1);
    smaaBlendingShaderInclude.SetInt("searchTex", 2);

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

    nsk_cg::Texture colorTexture{nsk_cg::TextureFormat::Color, userContext.GetScreenWidth(), userContext.GetScreenHeight() };
    nsk_cg::Texture depthTexture{nsk_cg::TextureFormat::Depth, userContext.GetScreenWidth(), userContext.GetScreenHeight() };
    nsk_cg::Texture edgesTexture{ nsk_cg::TextureFormat::Color, userContext.GetScreenWidth(), userContext.GetScreenHeight() };
    nsk_cg::Texture blendTexture{ nsk_cg::TextureFormat::Color, userContext.GetScreenWidth(), userContext.GetScreenHeight() };

    std::vector<unsigned char> flippedSearchTex(searchTexBytes, searchTexBytes + SEARCHTEX_SIZE);
    nsk_cg::flipImage(flippedSearchTex.data(), SEARCHTEX_WIDTH, SEARCHTEX_HEIGHT, 1);
    nsk_cg::Texture searchTexture{ GL_R8, GL_RED, SEARCHTEX_WIDTH, SEARCHTEX_HEIGHT, flippedSearchTex.data() };
    searchTexture.SetWrap(GL_CLAMP);
    std::vector<unsigned char> flippedAreaTex(areaTexBytes, areaTexBytes + AREATEX_SIZE);
    nsk_cg::flipImage(flippedAreaTex.data(), AREATEX_WIDTH, AREATEX_HEIGHT, 2);
    nsk_cg::Texture areaTexture{ GL_RG8, GL_RG, AREATEX_WIDTH, AREATEX_HEIGHT, flippedAreaTex.data() };
    areaTexture.SetWrap(GL_CLAMP);

    nsk_cg::Framebuffer framebuffer;
    while (!glfwWindowShouldClose(window))
    {
        nsk_cg::processInput(window);
         
        const glm::mat4 viewMatrix = userContext.GetViewMatrix();
        ourShader.Use();
        setLightSourceToShader(light, ourShader, viewMatrix);

        glEnable(GL_DEPTH_TEST);

        framebuffer.Attach(colorTexture, depthTexture);
        framebuffer.Bind();
        glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (const auto& object : sceneObjects.GetOpaqueObjects())
        {
            const glm::mat4 projectionMatrix = userContext.GetProjection();
            drawObject(ourShader, viewMatrix, projectionMatrix, object);
        }

        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

        framebuffer.Attach(edgesTexture, depthTexture);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        drawQuadBufferless(smaaEdgeDetectionShaderInclude, screenQuadVao, colorTexture);

        framebuffer.Attach(blendTexture, depthTexture);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        smaaBlendingShaderInclude.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, edgesTexture.GetRaw());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, areaTexture.GetRaw());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, searchTexture.GetRaw());
        screenQuadVao.drawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        glActiveTexture(GL_TEXTURE0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        drawQuad(screenQuadShader, screenQuadVao, blendTexture);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
