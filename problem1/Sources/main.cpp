#include <iostream>
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
    ArrayBuffer positionsVBO;
    ArrayBuffer colorsVBO;
    ElementBuffer ebo;
    VertexArray vao_;

    positionsVBO.bufferData(object.GetVertices().size() * sizeof(float), object.GetVertices().data(),
                            GL_STATIC_DRAW);

    colorsVBO.bufferData(object.GetColors().size() * sizeof(float), object.GetColors().data(),
                         GL_STATIC_DRAW);

    vao_.vertexAttribPointer(positionsVBO, 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    vao_.enableVertexAttribArray(0);

    vao_.vertexAttribPointer(colorsVBO, 1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    vao_.enableVertexAttribArray(1);

    ebo.bufferData(vao_, object.GetIndices().size() * sizeof(unsigned/*decltype(indices)::value_type*/),
                   object.GetIndices().data(),
                   GL_STATIC_DRAW);

    elementBuffers.push_back(std::move(ebo));
    arrayBuffers.push_back(std::move(positionsVBO));
    arrayBuffers.push_back(std::move(colorsVBO));

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

    UserContext userContext;

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(userContext.GetScreenWidth(), userContext.GetScreenHeight(), "Problem 1", nullptr, nullptr);
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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
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

    const GeometryObject cube = makeRainbowCube();
    const GeometryObject platform = makePlatform();

    std::vector<ArrayBuffer> arrayBuffers;
    std::vector<ElementBuffer> elementBuffers;
    VertexArray cubeVAO = LoadBuffers(cube, arrayBuffers, elementBuffers);
    VertexArray rectVAO = LoadBuffers(platform, arrayBuffers, elementBuffers);

    const std::vector<DrawData> objectsToDraw = {
        DrawData(&rectVAO, glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, -10.0f, 0.0f)),
                 platform.GetIndices().size()),
        DrawData(&cubeVAO, glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.5f)), cube.GetIndices().size()),
        DrawData(&cubeVAO, glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 0.5f)), cube.GetIndices().size()),
    };

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
void framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    auto* context =  static_cast<UserContext*>(glfwGetWindowUserPointer(window));
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
