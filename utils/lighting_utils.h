#pragma once

#include <glm/fwd.hpp>

namespace nsk_cg
{
struct Light;
class ShaderProgram;
struct Material;
class Mesh;

void setMaterialToShader(const nsk_cg::Material& material, const nsk_cg::ShaderProgram& ourShader);
void setLightSourceToShader(const nsk_cg::Light& light, const nsk_cg::ShaderProgram& ourShader,
                            const glm::mat4& viewMatrix);
}
