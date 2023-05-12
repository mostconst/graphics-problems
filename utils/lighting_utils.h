#pragma once
#include <vector>

#include "VertexArray.h"
#include "glm/fwd.hpp"

namespace nsk_cg
{
struct Light;
class ShaderProgram;
struct Material;
class Mesh;
nsk_cg::VertexArray LoadBuffers(const nsk_cg::Mesh& object, std::vector<nsk_cg::ArrayBuffer>& arrayBuffers,
                                std::vector<nsk_cg::ElementBuffer>& elementBuffers);
void SetMaterialToShader(const nsk_cg::Material& cubeMaterial, const nsk_cg::ShaderProgram& ourShader);
void SetLightSourceToShader(const nsk_cg::Light& light, const nsk_cg::ShaderProgram& ourShader, const glm::mat4& viewMatrix);
}
