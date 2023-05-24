#include "depth_peeling.h"

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/fwd.hpp>

#include "DrawData.h"
#include "Framebuffer.h"
#include "lighting_utils.h"
#include "ScreenSize.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexArray.h"

namespace nsk_cg
{
void drawObject(const ShaderProgram& ourShader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const std::vector<
                    ExtendedDrawData>::value_type& drawObject)
{
    const glm::mat4 modelViewMatrix = viewMatrix * drawObject.placement;
    const auto mvpMatrix = projectionMatrix * modelViewMatrix;
    ourShader.SetMat4("mvpMatrix", mvpMatrix);
    ourShader.SetMat4("modelViewMatrix", modelViewMatrix);
    const glm::mat3 modelViewNormalMatrix = glm::mat3(transpose(inverse(modelViewMatrix)));
    ourShader.SetMat3("modelViewNormalMatrix", modelViewNormalMatrix);
    setMaterialToShader(drawObject.m_material, ourShader);
    drawObject.vao->drawElements(GL_TRIANGLES, drawObject.nVertices, GL_UNSIGNED_INT, nullptr);
}

Texture makeTexture(const int format, const int width, const int height) {
    Texture textureColorBuffer;
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer.GetRaw());
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureColorBuffer;
}



std::vector<Texture> makeColorTextures(const int layers, const int width, const int height)
{
    assert(layers > 0);
    assert(width > 0);
    assert(height > 0);

    std::vector<Texture> res;
    for(int i = 0; i < layers; ++i)
    {
        res.push_back(makeTexture(GL_RGBA, width, height));
    }

    return res;
}

void drawLayers(const ShaderProgram& screenQuadShader, const VertexArray& screenQuadVao,
                const std::vector<Texture>& colorTexture)
{
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    screenQuadShader.Use();
    glActiveTexture(GL_TEXTURE0);
    for (auto it = colorTexture.crbegin(); it != colorTexture.crend(); ++it)
    {
        glBindTexture(GL_TEXTURE_2D, it->GetRaw()); // use the color attachment texture as the texture of the quad plane
        screenQuadVao.drawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

DepthPeelingResources::DepthPeelingResources(const ScreenSize& screenSize, const int nLayers)
{
    assert(nLayers > 0);

    m_colorTextures = nsk_cg::makeColorTextures(
        nLayers, screenSize.m_width, screenSize.m_height);
    m_depthTextures = {
        nsk_cg::makeTexture(GL_DEPTH_COMPONENT, screenSize.m_width, screenSize.m_height),
        nsk_cg::makeTexture(GL_DEPTH_COMPONENT, screenSize.m_width, screenSize.m_height)
    };
}

const std::vector<Texture>& DepthPeelingResources::GetColorTextures() const
{
    return m_colorTextures;
}

const std::array<Texture, 2>& DepthPeelingResources::GetDepthTextures() const
{
    return m_depthTextures;
}

void DepthPeelingResources::OnWindowSizeChange(int width, int height)
{

}
}
