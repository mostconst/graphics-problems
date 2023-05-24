#include "lighting_utils.h"

#include <array>

#include "constants.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "VertexArray.h"

namespace nsk_cg
{
void setMaterialToShader(const nsk_cg::Material& material, const nsk_cg::ShaderProgram& ourShader)
{
    ourShader.SetVec3("material.ambient", material.ambient);
    ourShader.SetVec3("material.diffuse", material.diffuse);
    ourShader.SetVec3("material.specular", material.specular);
    ourShader.SetFloat("material.shininess", material.shininess);
    ourShader.SetFloat("material.opacity", material.opacity);
}

void setLightSourceToShader(const nsk_cg::Light& light, const nsk_cg::ShaderProgram& ourShader, const glm::mat4& viewMatrix)
{
    ourShader.SetInt("light.type", static_cast<int>(light.type));
    ourShader.SetVec3("light.ambient", light.ambient);
    ourShader.SetVec3("light.diffuse", light.diffuse); // darken diffuse light a bit
    ourShader.SetVec3("light.specular", light.specular);
    const glm::vec3 viewLightPosition = glm::vec3(viewMatrix * glm::vec4(light.position, 1.0f));
    const glm::vec3 viewLightDirection = glm::vec3(viewMatrix * glm::vec4(light.direction, 0.0f));
    ourShader.SetVec3("light.viewPosition", viewLightPosition);
    ourShader.SetVec3("light.viewDirection", viewLightDirection);
}
}
