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
#include "GeometryObject.h"
#include "MeshBuilding.h"
#include "UserContext.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double screenX, double screenY);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

nsk_cg::VertexArray LoadBuffers(const nsk_cg::Mesh& object, std::vector<nsk_cg::ArrayBuffer>& arrayBuffers,
                        std::vector<nsk_cg::ElementBuffer>& elementBuffers)
{
    const auto& vertices = object.GetVertices();
    const auto indices = object.GetIndices();
    using VertexType = std::remove_reference_t<decltype(vertices)>::value_type;
    using IndexType= std::remove_reference_t<decltype(indices)>::value_type;

    nsk_cg::ArrayBuffer positionsVBO;
    positionsVBO.bufferData(vertices.size() * sizeof(VertexType), vertices.data(),
                            GL_STATIC_DRAW);

    constexpr int positionIndex = 0;
    nsk_cg::VertexArray vao;
    vao.vertexAttribPointer(positionsVBO, positionIndex, VertexType::nComponents, VertexType::componentType, GL_FALSE, 0, 0);
    vao.enableVertexAttribArray(positionIndex);

    nsk_cg::ElementBuffer ebo;
    ebo.bufferData(vao, indices.size() * sizeof(IndexType),
                   object.GetIndices().data(),
                   GL_STATIC_DRAW);

    elementBuffers.push_back(std::move(ebo));
    arrayBuffers.push_back(std::move(positionsVBO));

    return vao;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    constexpr glm::vec3 firstCubeLocation(0.0f, 0.0f, 0.0f);
    constexpr glm::vec3 lightSourceLocation(2.0f, 2.0f, 2.0f);
    constexpr glm::vec3 cubeColor = { 1.0f, 0.5f, 0.31f };
    constexpr glm::vec3 lightColor = { 1.0f, 1.0f, 1.0f };
    constexpr glm::vec3 lookAt(firstCubeLocation);
    // HW_ITEM 3
    nsk_cg::UserContext userContext(lookAt, 5.5f);

    GLFWwindow* window = glfwCreateWindow(userContext.GetScreenWidth(), userContext.GetScreenHeight(), "Problem 1",
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
        ourShader = nsk_cg::createShader("shader.vs", "shader.fs");
        lightSourceShader = nsk_cg::createShader("shader.vs", "lightSourceShader.fs");
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

    std::vector<nsk_cg::ArrayBuffer> arrayBuffers;
    std::vector<nsk_cg::ElementBuffer> elementBuffers;
    nsk_cg::VertexArray cubeVAO = LoadBuffers(cube, arrayBuffers, elementBuffers);

    const std::vector<nsk_cg::DrawData> objectsToDraw = {
        nsk_cg::DrawData(&cubeVAO, glm::translate(glm::mat4(1.0f), firstCubeLocation), cube.GetIndices().size()),
    };
    auto lightSource = nsk_cg::DrawData(&cubeVAO, glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)), lightSourceLocation), cube.GetIndices().size());

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::mat4 view = userContext.GetViewMatrix();
        ourShader.Use();
        for (const auto& drawObject : objectsToDraw)
        {
            const auto mvpMatrix = userContext.GetProjection() * view * drawObject.placement;
            ourShader.SetMat4("mvpMatrix", mvpMatrix);
            ourShader.SetVec3("objectColor", cubeColor);
            ourShader.SetVec3("lightColor", lightColor);
            drawObject.vao->drawElements(GL_TRIANGLES, drawObject.nVertices, GL_UNSIGNED_INT, nullptr);
        }

        lightSourceShader.Use();
        const auto mvpMatrix = userContext.GetProjection() * view * lightSource.placement;
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
