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
#include "VertexArray.h"
#include "DrawData.h"
#include "MeshBuilding.h"
#include "UserContext.h"
#include "Material.h"
#include "Light.h"
#include "lighting_utils.h"
#include "constants.h"
#include "Texture.h"
#include "Framebuffer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double screenX, double screenY);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void drawObject(const nsk_cg::ShaderProgram& ourShader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const std::vector<nsk_cg::ExtendedDrawData>::value_type& drawObject)
{
    const glm::mat4 modelViewMatrix = viewMatrix * drawObject.placement;
    const auto mvpMatrix = projectionMatrix * modelViewMatrix;
    ourShader.SetMat4("mvpMatrix", mvpMatrix);
    ourShader.SetMat4("modelViewMatrix", modelViewMatrix);
    const glm::mat3 modelViewNormalMatrix = glm::mat3(glm::transpose(glm::inverse(modelViewMatrix)));
    ourShader.SetMat3("modelViewNormalMatrix", modelViewNormalMatrix);
    SetMaterialToShader(drawObject.m_material, ourShader);
    drawObject.vao->drawElements(GL_TRIANGLES, drawObject.nVertices, GL_UNSIGNED_INT, nullptr);
}

nsk_cg::Texture makeTexture(const int format, const int width, const int height) {
    nsk_cg::Texture textureColorbuffer;
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer.GetRaw());
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureColorbuffer;
}

void makeFramebuffer(const nsk_cg::Framebuffer& framebuffer, const nsk_cg::Texture& textureColorbuffer, const nsk_cg::Texture& textureDepthBuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.GetRaw());
    // create a color attachment texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer.GetRaw(), 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    //const unsigned rbo = makeRenderbuffer(width, height);
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureDepthBuffer.GetRaw(), 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
}

nsk_cg::VertexArray loadScreenQuadBuffers(std::vector<nsk_cg::ArrayBuffer>& arrayBuffers,
    std::vector<nsk_cg::ElementBuffer>& elementBuffers)
{
    const std::array<glm::vec2, nsk_cg::VERTICES_IN_RECTANGLE> screenCoordinates = {
        glm::vec2{ -1.0f, -1.0f },
        glm::vec2{  1.0f, -1.0f },
        glm::vec2{  1.0f,  1.0f },
        glm::vec2{ -1.0f,  1.0f }
    };
    const std::array<glm::vec2, nsk_cg::VERTICES_IN_RECTANGLE> textureCoordinates = {
        glm::vec2{ 0.0f, 0.0f },
        glm::vec2{ 1.0f, 0.0f },
        glm::vec2{ 1.0f, 1.0f },
        glm::vec2{ 0.0f, 1.0f },
    };
    const std::array<unsigned, nsk_cg::VERTICES_IN_TRIANGLE * 2> indices = {
        0, 1, 2,
        0, 2, 3
    };
    using ScreenCoordinatesType = std::remove_reference_t<decltype(screenCoordinates)>::value_type;
    using TextureCoordinatesType = std::remove_reference_t<decltype(textureCoordinates)>::value_type;
    using IndexType = std::remove_reference_t<decltype(indices)>::value_type;

    nsk_cg::ArrayBuffer screenCoordinatesVbo;
    screenCoordinatesVbo.bufferData(screenCoordinates.size() * sizeof(ScreenCoordinatesType), screenCoordinates.data(),
        GL_STATIC_DRAW);

    nsk_cg::ArrayBuffer textureCoordinatesVbo;
    textureCoordinatesVbo.bufferData(textureCoordinates.size() * sizeof(TextureCoordinatesType), textureCoordinates.data(),
        GL_STATIC_DRAW);

    nsk_cg::VertexArray vao;

    constexpr int positionIndex = 0;
    // TODO constants
    vao.vertexAttribPointer(screenCoordinatesVbo, positionIndex, 2, nsk_cg::AttributeType::Float, GL_FALSE, 0, 0);
    vao.enableVertexAttribArray(positionIndex);

    constexpr int normalIndex = 1;
    // TODO constants
    vao.vertexAttribPointer(textureCoordinatesVbo, normalIndex, 2, nsk_cg::AttributeType::Float, GL_FALSE, 0, 0);
    vao.enableVertexAttribArray(normalIndex);

    nsk_cg::ElementBuffer ebo;
    ebo.bufferData(vao, indices.size() * sizeof(IndexType),
        indices.data(),
        GL_STATIC_DRAW);

    elementBuffers.push_back(std::move(ebo));
    arrayBuffers.push_back(std::move(screenCoordinatesVbo));
    arrayBuffers.push_back(std::move(textureCoordinatesVbo));

    return vao;
}

struct RectangleParameters
{
    const glm::vec3 location;
    const nsk_cg::Material material;
};

nsk_cg::Material makeTransparentMaterial(const glm::vec4& color)
{
    return { glm::vec3(color),
{ 1.0f, 0.5f, 0.31f },
{ 0.5f, 0.5f, 0.5f },
32.0f,
    color.a
    };
}

std::vector<nsk_cg::Texture> makeColorTextures(const int layers, const int width, const int height)
{
    assert(layers > 0);
    assert(width > 0);
    assert(height > 0);

    std::vector<nsk_cg::Texture> res;
    for(int i = 0; i < layers; ++i)
    {
        res.push_back(makeTexture(GL_RGBA, width, height));
    }

    return res;
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

    constexpr glm::vec3 backgroundColor(0.5f);
    constexpr glm::vec3 cubeLocation(0.0f, 0.0f, 0.0f);
    const nsk_cg::Material cubeMaterial = { { 1.0f, 0.5f, 0.31f },
        { 1.0f, 0.5f, 0.31f },
        { 0.5f, 0.5f, 0.5f },
        32.0f
    };

    std::vector<RectangleParameters> rectangles = {
        {{1.0f, 1.0f, 1.0f}, makeTransparentMaterial({0.0f, 1.0f, 0.0f, 0.5f})},
        {{0.0f, 0.0f, 0.0f}, makeTransparentMaterial({0.0f, 0.0f, 1.0f, 0.5f})}
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
    nsk_cg::ShaderProgram screenQuadShader;
    try
    {
        ourShader = nsk_cg::createShader("GourandBlinn.vert", "Gourand.frag");
        lightSourceShader = nsk_cg::createShader("lightSourceShader.vs", "lightSourceShader.fs");
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

    screenQuadShader.Use();
    //screenQuadShader.SetInt("screenTexture", 0);

    const nsk_cg::Mesh cube = nsk_cg::makeCubeForLighting();
    const nsk_cg::Mesh rectangleMesh = nsk_cg::makeRectangle(1.0f);

    std::vector<nsk_cg::ArrayBuffer> arrayBuffers;
    std::vector<nsk_cg::ElementBuffer> elementBuffers;
    nsk_cg::VertexArray cubeVao = LoadBuffers(cube, arrayBuffers, elementBuffers);
    nsk_cg::VertexArray rectangleVao = LoadBuffers(rectangleMesh, arrayBuffers, elementBuffers);
    nsk_cg::VertexArray screenQuadVao = loadScreenQuadBuffers(arrayBuffers, elementBuffers);

    const std::vector<nsk_cg::ExtendedDrawData> opaqueObjects = {
        nsk_cg::ExtendedDrawData(&cubeVao, cubeMaterial, glm::translate(glm::mat4(1.0f), cubeLocation), cube.GetIndices().size())
    };
    std::vector<nsk_cg::ExtendedDrawData> transparentObjects;

    for(const auto& rectangle : rectangles)
    {
        auto drawData = nsk_cg::ExtendedDrawData(&rectangleVao, rectangle.material, glm::translate(glm::mat4(1.0f), rectangle.location), rectangleMesh.GetIndices().size());
        transparentObjects.push_back(drawData);
    }

    auto lightSource = nsk_cg::DrawData(&cubeVao, glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)), light.position), cube.GetIndices().size());

    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    constexpr int nLayers = 2;
    std::vector<nsk_cg::Texture> colorTexture = makeColorTextures(nLayers, userContext.GetScreenWidth(),
                                                                  userContext.GetScreenHeight());
    const std::array<nsk_cg::Texture, 2> depthTexture = { makeTexture(GL_DEPTH_COMPONENT, userContext.GetScreenWidth(), userContext.GetScreenHeight()),
        makeTexture(GL_DEPTH_COMPONENT, userContext.GetScreenWidth(), userContext.GetScreenHeight())
    };
    nsk_cg::Framebuffer framebuffer;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::mat4 viewMatrix = userContext.GetViewMatrix();
        ourShader.Use();
        SetLightSourceToShader(light, ourShader, viewMatrix);
        const glm::mat4 projectionMatrix = userContext.GetProjection();

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.GetRaw());
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //for (const auto& object : opaqueObjects)
        //{
        //    drawObject(ourShader, viewMatrix, projectionMatrix, object);
        //}
        makeFramebuffer(framebuffer, colorTexture[0], depthTexture[1]);
        glClearDepth(0.0);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClearDepth(1.0);

        for (size_t i = 0; i < 2; i++)
        {
            makeFramebuffer(framebuffer, colorTexture[i], depthTexture[i % 2]);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthTexture[(i + 1) % 2].GetRaw());
            for (const auto& object : transparentObjects)
            {
                drawObject(ourShader, viewMatrix, projectionMatrix, object);
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
        glDisable(GL_BLEND);
        screenQuadShader.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorTexture[0].GetRaw());	// use the color attachment texture as the texture of the quad plane
        screenQuadVao.drawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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
