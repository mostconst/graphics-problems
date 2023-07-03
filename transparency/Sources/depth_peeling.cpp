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

Texture makeTexture(const TextureFormat& format, const int width, const int height) {
    Texture textureColorBuffer(format, width, height);
    return textureColorBuffer;
}



std::vector<Texture> makeColorTextures(const int layers, const int width, const int height)
{
    assert(layers > 0);

    std::vector<Texture> res;
    for(int i = 0; i < layers; ++i)
    {
        res.push_back(makeTexture(TextureFormat::Color, width, height));
    }

    return res;
}

void drawLayers(const ShaderProgram& screenQuadShader, const VertexArray& screenQuadVao,
                const std::vector<Texture>& colorTextures, const Texture& opaqueLayer)
{
    screenQuadShader.Use();
    glActiveTexture(GL_TEXTURE0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // TODO remove?

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (auto it = colorTextures.crbegin(); it != colorTextures.crend(); ++it)
    {
        glBindTexture(GL_TEXTURE_2D, it->GetRaw()); // use the color attachment texture as the texture of the quad plane
        screenQuadVao.drawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
    glDisable(GL_BLEND);
}

DepthPeelingResources::DepthPeelingResources(const ScreenSize& screenSize, const int nLayers)
    : m_opaqueColor(TextureFormat::Color, screenSize.m_width, screenSize.m_height),
      m_opaqueDepth(TextureFormat::Depth, screenSize.m_width, screenSize.m_height),
      m_colorTextures(nsk_cg::makeColorTextures(
          nLayers, screenSize.m_width, screenSize.m_height)),
      m_depthTextures{
          nsk_cg::makeTexture(TextureFormat::Depth, screenSize.m_width, screenSize.m_height),
          nsk_cg::makeTexture(TextureFormat::Depth, screenSize.m_width, screenSize.m_height)
      }
{
}

const Texture& DepthPeelingResources::GetOpaqueColor() const
{
    return m_opaqueColor;
}

const Texture& DepthPeelingResources::GetOpaqueDepth() const
{
    return m_opaqueDepth;
}

const std::vector<Texture>& DepthPeelingResources::GetTransparencyTextures() const
{
    return m_colorTextures;
}

const std::array<Texture, 2>& DepthPeelingResources::GetDepthTextures() const
{
    return m_depthTextures;
}

void DepthPeelingResources::OnWindowSizeChange(const ScreenSize& size)
{
    m_opaqueColor.SetSize(size);
    m_opaqueDepth.SetSize(size);

    for (auto& colorTexture : m_colorTextures)
    {
        colorTexture.SetSize(size);
    }
    for (auto& depthTexture : m_depthTextures)
    {
        depthTexture.SetSize(size);
    }
}
}
