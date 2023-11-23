#pragma once

#include <vector>

#include <glm/fwd.hpp>

#include "Texture.h"

namespace nsk_cg
{
class VertexArray;
class Framebuffer;
class ExtendedDrawData;
class ShaderProgram;
class ScreenSize;

void drawObject(const ShaderProgram& ourShader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix,
    const std::vector<ExtendedDrawData>::value_type& drawObject);
void drawQuad(const ShaderProgram& screenQuadShader, const VertexArray& screenQuadVao,
    const Texture& colorTexture);
}
