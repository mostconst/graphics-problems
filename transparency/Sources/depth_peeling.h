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
Texture makeTexture(const int format, const int width, const int height);
std::vector<Texture> makeColorTextures(const int layers, const int width, const int height);
void drawLayers(const ShaderProgram& screenQuadShader, const VertexArray& screenQuadVao,
                const std::vector<Texture>& colorTexture);

class DepthPeelingResources final : public IScreenSizeListener
{
public:
    explicit DepthPeelingResources(const ScreenSize& screenSize, int nLayers);
    const std::vector<Texture>& GetColorTextures() const;
    const std::array<Texture, 2>& GetDepthTextures() const;
    void OnWindowSizeChange(int width, int height) override;

private:
    std::vector<Texture> m_colorTextures;
    std::array<Texture, 2> m_depthTextures;
};
}
