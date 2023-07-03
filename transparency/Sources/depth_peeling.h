#pragma once

#include <array>
#include <vector>

#include <glm/fwd.hpp>

#include "listener_interface.h"
#include "Texture.h"

namespace nsk_cg
{
class VertexArray;
class Framebuffer;
class ExtendedDrawData;
class ShaderProgram;
struct ScreenSize;

void drawObject(const ShaderProgram& ourShader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix,
                const std::vector<ExtendedDrawData>::value_type& drawObject);
Texture makeTexture(const TextureFormat& format, const int width, const int height);
std::vector<Texture> makeColorTextures(const int layers, const int width, const int height);
void drawLayers(const ShaderProgram& screenQuadShader, const VertexArray& screenQuadVao,
                const std::vector<Texture>& colorTextures, const Texture& opaqueLayer);

class DepthPeelingResources final : public IScreenSizeListener
{
public:
    explicit DepthPeelingResources(const ScreenSize& screenSize, int nLayers);

    const Texture& GetOpaqueColor() const;
    const Texture& GetOpaqueDepth() const;
    const std::vector<Texture>& GetTransparencyTextures() const;
    const std::array<Texture, 2>& GetDepthTextures() const;
    void OnWindowSizeChange(const ScreenSize& size) override;

private:
    Texture m_opaqueColor;
    Texture m_opaqueDepth;
    const std::vector<Texture> m_colorTextures;
    const std::array<Texture, 2> m_depthTextures;
};
}
