#include "draw_helpers.h"

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/fwd.hpp>

#include "DrawData.h"
#include "lighting_utils.h"
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

void drawQuad(const ShaderProgram& screenQuadShader, const VertexArray& screenQuadVao,
    const Texture& colorTexture)
{
    screenQuadShader.Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorTexture.GetRaw()); // use the color attachment texture as the texture of the quad plane
    screenQuadVao.drawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void drawQuadBufferless(const ShaderProgram& screenQuadShader, const VertexArray& screenQuadVao,
    const Texture& colorTexture)
{
    screenQuadShader.Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorTexture.GetRaw()); // use the color attachment texture as the texture of the quad plane
    screenQuadVao.drawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
}
}
