#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shader_s.h"
#include "gldebug.h"
#include "ArrayBuffer.h"
#include "VertexArray.h"
#include "DrawData.h"
#include "GeometryObject.h"
#include "UserContext.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double screenX, double screenY);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

VertexArray LoadBuffers(const GeometryObject& object, std::vector<ArrayBuffer>& arrayBuffers,
    std::vector<ElementBuffer>& elementBuffers)
{
    const auto& colors = object.GetColors();
    const auto& vertices = object.GetVertices();
    const auto& indices = object.GetIndices();
    using ColorType = std::remove_reference_t<decltype(colors)>::value_type;
    using VertexType = std::remove_reference_t<decltype(vertices)>::value_type;
    using IndexType = std::remove_reference_t<decltype(indices)>::value_type;

    ArrayBuffer positionsVBO;
    positionsVBO.bufferData(vertices.size() * sizeof(VertexType), vertices.data(),
        GL_STATIC_DRAW);
    ArrayBuffer colorsVBO;
    colorsVBO.bufferData(colors.size() * sizeof(ColorType), colors.data(),
        GL_STATIC_DRAW);

    // HW_ITEM 7
    constexpr int positionIndex = 0;
    VertexArray vao;
    vao.vertexAttribPointer(positionsVBO, positionIndex, VertexType::nComponents, VertexType::componentType, GL_FALSE, 0, 0);
    vao.enableVertexAttribArray(positionIndex);

    constexpr int colorIndex = 1;
    vao.vertexAttribPointer(colorsVBO, colorIndex, ColorType::nComponents, ColorType::componentType, GL_FALSE, 0, 0);
    vao.enableVertexAttribArray(colorIndex);

    ElementBuffer ebo;
    ebo.bufferData(vao, indices.size() * sizeof(IndexType),
        object.GetIndices().data(),
        GL_STATIC_DRAW);

    elementBuffers.push_back(std::move(ebo));
    arrayBuffers.push_back(std::move(positionsVBO));
    arrayBuffers.push_back(std::move(colorsVBO));

    return vao;
}

using VertInd = std::pair<std::vector<utils::Vertex>, std::vector<unsigned int>>;

utils::Vertex middle(const utils::Vertex& v1, const utils::Vertex& v2)
{
    return { (v1.x + v2.x) / 2.0f, (v1.y + v2.y) / 2.0f, (v1.z + v2.z) / 2.0f };
}

utils::Vertex norm(const utils::Vertex& v)
{
    auto length = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    return { v.x / length, v.y / length, v.z / length };
}

VertInd tesselate(const std::vector<utils::Vertex>& vertices, const std::vector<unsigned int>& indices)
{
    auto out_vertices = vertices;
    auto out_indices = indices;
    for(size_t i = 0; i < indices.size(); i+=3)
    {
        const auto ind0 = indices[i];
        const auto ind1 = indices[i + 1];
        const auto ind2 = indices[i + 2];
        const auto ind3 = out_vertices.size() + 0;
        const auto ind4 = out_vertices.size() + 1;
        const auto ind5 = out_vertices.size() + 2;
        out_vertices.push_back(middle(vertices[ind0], vertices[ind1]));
        out_vertices.push_back(middle(vertices[ind1], vertices[ind2]));
        out_vertices.push_back(middle(vertices[ind2], vertices[ind0]));

        out_indices.push_back(ind0);
        out_indices.push_back(ind3);
        out_indices.push_back(ind5);

        out_indices.push_back(ind3);
        out_indices.push_back(ind1);
        out_indices.push_back(ind4);

        out_indices.push_back(ind5);
        out_indices.push_back(ind4);
        out_indices.push_back(ind2);

        out_indices.push_back(ind3);
        out_indices.push_back(ind4);
        out_indices.push_back(ind5);
    }

    return { out_vertices, out_indices };
}

GeometryObject makePyramid()
{
    std::vector<utils::Vertex> _vertices = {
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, -1.0f},
    };
    std::vector<unsigned int> _indices = {
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4,
        0, 1, 5,
        1, 2, 5,
        2, 3, 5,
        3, 0, 5,
    };

    auto tessResult = tesselate(_vertices, _indices);
    tessResult = tesselate(tessResult.first, tessResult.second);
    //tessResult = tesselate(tessResult.first, tessResult.second);
    //tessResult = tesselate(tessResult.first, tessResult.second);
    auto vertices = tessResult.first;
    auto indices = tessResult.second;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 0xFFFFFF);
    std::vector<utils::Color> colors;
    for (size_t i = 0; i != vertices.size(); ++i)
    {
        colors.emplace_back(dist6(rng));
    }

    std::vector<utils::Vertex> norm_vertices;
    for(auto v: vertices)
    {
        norm_vertices.push_back(norm(v));
    }

    return { std::move(norm_vertices), std::move(colors), std::move(indices) };
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

    constexpr glm::vec3 lookAt(0.0f, 0.0f, 0.0f);
    // HW_ITEM 3
    UserContext userContext(lookAt, 5.5f);

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
    enableDebugOutput();
#endif

    ShaderProgram ourShader;
    try
    {
        ourShader = createShader("shader.vs", "shader.fs");
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

    const GeometryObject cube = makePyramid();

    std::vector<ArrayBuffer> arrayBuffers;
    std::vector<ElementBuffer> elementBuffers;
    VertexArray cubeVAO = LoadBuffers(cube, arrayBuffers, elementBuffers);

    // HW_ITEM 6
    const std::vector<DrawData> objectsToDraw = {
        DrawData(&cubeVAO, glm::mat4(1.0f), cube.GetIndices().size()),
    };

    // HW_ITEM 9
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    ourShader.use();

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::mat4 view = userContext.GetViewMatrix();
        for (const auto& drawObject : objectsToDraw)
        {
            const auto mvpMatrix = userContext.GetProjection() * view * drawObject.placement;
            // HW_ITEM 5
            ourShader.setMat4("mvpMatrix", mvpMatrix);
            drawObject.vao->drawElements(GL_TRIANGLES, drawObject.nVertices, GL_UNSIGNED_INT, nullptr);
        }

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
    auto* context = static_cast<UserContext*>(glfwGetWindowUserPointer(window));
    context->OnWindowSizeChange(width, height);
}

void mouse_callback(GLFWwindow* window, const double screenX, const double screenY)
{
    auto* context = static_cast<UserContext*>(glfwGetWindowUserPointer(window));
    context->OnMouseMove(screenX, screenY);
}

void mouse_button_callback(GLFWwindow* window, const int button, const int action, int)
{
    auto* context = static_cast<UserContext*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        context->OnLeftMouseButtonAction(true);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        context->OnLeftMouseButtonAction(false);
    }
}
