#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shader_s.h"
#include "gldebug.h"
#include "camera.h"
#include "ArrayBuffer.h"
#include "VertexArray.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double screenX, double screenY);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// settings
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;
constexpr double cameraSensitivity = 100.0; // pixels per rad
constexpr float fovDeg = 45.0f;
constexpr float zNear = 0.1f;
constexpr float zFar = 100.0f;

std::pair lastCursorPos = {0.0, 0.0};
bool buttonPressed = false;

Camera camera{ 5.0, glm::vec3(-1.0f, -1.0f, 0.0f) };
glm::mat4 projection = glm::perspective(glm::radians(fovDeg),
                                        static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), zNear,
                                        zFar);

class DrawData
{
public:
    DrawData(VertexArray* vao, const glm::mat4& placement, size_t nVertices)
        : vao(vao),
          placement(placement),
          nVertices(nVertices)
    {
    }

    VertexArray* vao;
    glm::mat4 placement;
    size_t nVertices;
};

VertexArray LoadBuffers(const std::vector<float>& vertices, const std::vector<float>& colors, const std::vector<unsigned>& indices, std::vector<ArrayBuffer>& arrayBuffers, std::vector<ElementBuffer>& elementBuffers)
{
    ArrayBuffer positionsVBO_;
    ArrayBuffer colorsVBO_;
    ElementBuffer ebo_;
    VertexArray vao_;

    positionsVBO_.bufferData(vertices.size() * sizeof(float/*decltype(vertices)::value_type*/), vertices.data(),
        GL_STATIC_DRAW);

    colorsVBO_.bufferData(colors.size() * sizeof(float/*decltype(vertices)::value_type*/), colors.data(),
        GL_STATIC_DRAW);

    vao_.vertexAttribPointer(positionsVBO_, 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    vao_.enableVertexAttribArray(0);

    vao_.vertexAttribPointer(colorsVBO_, 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    vao_.enableVertexAttribArray(1);

    ebo_.bufferData(vao_, indices.size() * sizeof(unsigned/*decltype(indices)::value_type*/), indices.data(),
        GL_STATIC_DRAW);

    elementBuffers.push_back(std::move(ebo_));
    arrayBuffers.push_back(std::move(positionsVBO_));
    arrayBuffers.push_back(std::move(colorsVBO_));

    return vao_;
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

#ifndef NDEBUG
    //enableDebugOutput();
#endif

    ShaderProgram ourShader;
    try
    {
        ourShader = createShader("shader.vs", "shader.fs");
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
        return -1;
    }
    catch (std::invalid_argument& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }

    const std::vector<float> vertices = {
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
    };
    assert(vertices.size() == 8 * 3);
    std::vector<float> colors = {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.5f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.3f, 0.0f, 0.5f,
        0.6f, 0.0f, 0.8f,
        1.0f, 0.4f, 0.7f,
    };
    const std::vector<unsigned int> indices = {
        0, 1, 3, 0, 3, 2,
        5, 4, 6, 5, 6, 7,
        1, 5, 7, 1, 7, 3,
        4, 0, 2, 4, 2, 6,
        4, 5, 1, 4, 1, 0,
        2, 3, 7, 2, 7, 6,
    };
    assert(indices.size() == 6 * 2 * 3);

    const std::vector<float> rectVertices = {
        0.0f, 0.0f, 0.0f,
        20.0f, 0.0f, 0.0f,
        20.0f, 20.0f, 0.0f,
        0.0f, 20.0f, 0.0f,
    };
    const std::vector<float> rectColors(8 * 3, 0.5f);
    const std::vector<unsigned int> rectIndices = {
        0, 1, 2,
        0, 2, 3,
    };

    std::vector<ArrayBuffer> arrayBuffers;
    std::vector<ElementBuffer> elementBuffers;
    VertexArray cubeVAO = LoadBuffers(vertices, colors, indices, arrayBuffers, elementBuffers);
    VertexArray rectVAO = LoadBuffers(rectVertices, rectColors, rectIndices, arrayBuffers, elementBuffers);

    std::vector<DrawData> objectsToDraw = {
        DrawData(&rectVAO, glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, -10.0f, 0.0f)), rectIndices.size()),
        DrawData(&cubeVAO, glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.5f)), indices.size()),
        DrawData(&cubeVAO, glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 0.5f)), indices.size()),
    };

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    ourShader.use();

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::mat4 view = camera.GetViewMatrix();
        for (const auto& drawObject : objectsToDraw)
        {
            const auto mvpMatrix = projection * view * drawObject.placement;
            ourShader.setMat4("mvpMatrix", mvpMatrix);
            drawObject.vao->drawElements(GL_TRIANGLES, drawObject.nVertices, GL_UNSIGNED_INT, nullptr);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return EXIT_SUCCESS;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, const int width, const int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    projection = glm::perspective(glm::radians(fovDeg),
                                  static_cast<float>(width) / static_cast<float>(height), zNear,
                                  zFar);
}

void mouse_callback([[maybe_unused]] GLFWwindow* window, const double screenX, const double screenY)
{
    if (buttonPressed)
    {
        camera.RotateHorizontal(static_cast<float>((screenY - lastCursorPos.second) / -cameraSensitivity));
        camera.RotateVertical(static_cast<float>((screenX - lastCursorPos.first) / -cameraSensitivity));
    }
    lastCursorPos = {screenX, screenY};
}

void mouse_button_callback([[maybe_unused]] GLFWwindow* window, const int button, const int action,
                           [[maybe_unused]] int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        buttonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        buttonPressed = false;
    }
}