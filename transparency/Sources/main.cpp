#include <fstream>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "ShaderProgram.h"
#include "gldebug.h"
#include "ArrayBuffer.h"
#include "VertexArray.h"
#include "DrawData.h"
#include "MeshBuilding.h"
#include "UserContext.h"
#include "Material.h"
#include "Light.h"
#include "lighting_utils.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double screenX, double screenY);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    constexpr glm::vec3 backgroundColor(0.29f);
    constexpr glm::vec3 cubeLocation(0.0f, 0.0f, 0.0f);
    constexpr glm::vec3 rectangleLocation(1.0f, 1.0f, 1.0f);
    const nsk_cg::Material cubeMaterial = { { 1.0f, 0.5f, 0.31f },
        { 1.0f, 0.5f, 0.31f },
        { 0.5f, 0.5f, 0.5f },
        32.0f
    };
    const nsk_cg::Material rectangleMaterial = { { 1.0f, 0.0f, 0.0f },
    { 1.0f, 0.5f, 0.31f },
    { 0.5f, 0.5f, 0.5f },
    32.0f
    };

    const nsk_cg::Light light = {
        nsk_cg::LightType::Positional,
    	{ 5.0f, 5.0f, 5.0f },
        {0.0f, -1.0f, 0.0f},
        { 0.2f, 0.2f, 0.2f },
        { 0.5f, 0.5f, 0.5f },
        { 1.0f, 1.0f, 1.0f },
    };
    constexpr glm::vec3 lookAt(0.0f);
    // HW_ITEM 3
    nsk_cg::UserContext userContext(lookAt, 10.0f);

    GLFWwindow* window = glfwCreateWindow(userContext.GetScreenWidth(), userContext.GetScreenHeight(), "Transparency",
                                          nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetWindowUserPointer(window, &userContext);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return EXIT_FAILURE;
    }

#ifndef NDEBUG
    nsk_cg::enableDebugOutput();
#endif

    nsk_cg::ShaderProgram ourShader;
    nsk_cg::ShaderProgram lightSourceShader;
    try
    {
        ourShader = nsk_cg::createShader("GourandBlinn.vert", "Gourand.frag");
        lightSourceShader = nsk_cg::createShader("lightSourceShader.vs", "lightSourceShader.fs");
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

    const nsk_cg::Mesh cube = nsk_cg::makeCubeForLighting();
    const nsk_cg::Mesh rectangle = nsk_cg::makeRectangle(1.0f);

    std::vector<nsk_cg::ArrayBuffer> arrayBuffers;
    std::vector<nsk_cg::ElementBuffer> elementBuffers;
    nsk_cg::VertexArray cubeVao = LoadBuffers(cube, arrayBuffers, elementBuffers);
    nsk_cg::VertexArray rectangleVao = LoadBuffers(rectangle, arrayBuffers, elementBuffers);

    const std::vector<nsk_cg::ExtendedDrawData> objectsToDraw = {
        nsk_cg::ExtendedDrawData(&cubeVao, cubeMaterial, glm::translate(glm::mat4(1.0f), cubeLocation), cube.GetIndices().size()),
        nsk_cg::ExtendedDrawData(&rectangleVao, rectangleMaterial, glm::translate(glm::mat4(1.0f), rectangleLocation), rectangle.GetIndices().size())
    };
    auto lightSource = nsk_cg::DrawData(&cubeVao, glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)), light.position), cube.GetIndices().size());

    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
    glEnable(GL_DEPTH_TEST);
     
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::mat4 viewMatrix = userContext.GetViewMatrix();
        ourShader.Use();
        SetLightSourceToShader(light, ourShader, viewMatrix);
        for (const auto& drawObject : objectsToDraw)
        {
            const glm::mat4 modelViewMatrix = viewMatrix * drawObject.placement;
            const auto mvpMatrix = userContext.GetProjection() * modelViewMatrix;
            ourShader.SetMat4("mvpMatrix", mvpMatrix);
            ourShader.SetMat4("modelViewMatrix", modelViewMatrix);
            const glm::mat3 modelViewNormalMatrix = glm::mat3(glm::transpose(glm::inverse(modelViewMatrix)));
            ourShader.SetMat3("modelViewNormalMatrix", modelViewNormalMatrix);
            SetMaterialToShader(drawObject.m_material, ourShader);
            drawObject.vao->drawElements(GL_TRIANGLES, drawObject.nVertices, GL_UNSIGNED_INT, nullptr);
        }

        lightSourceShader.Use();
        const auto mvpMatrix = userContext.GetProjection() * viewMatrix * lightSource.placement;
        lightSourceShader.SetMat4("mvpMatrix", mvpMatrix);
        lightSource.vao->drawElements(GL_TRIANGLES, lightSource.nVertices, GL_UNSIGNED_INT, nullptr);

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
