#include "lighting_utils.h"

#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "VertexArray.h"

namespace nsk_cg
{
nsk_cg::VertexArray LoadBuffers(const nsk_cg::Mesh& object, std::vector<nsk_cg::ArrayBuffer>& arrayBuffers,
    std::vector<nsk_cg::ElementBuffer>& elementBuffers)
{
    const auto& vertices = object.GetVertices();
    const auto& indices = object.GetIndices();
    const auto& normals = object.GetNormals();
    using VertexType = std::remove_reference_t<decltype(vertices)>::value_type;
    using IndexType = std::remove_reference_t<decltype(indices)>::value_type;
    using NormalType = std::remove_reference_t<decltype(normals)>::value_type;

    nsk_cg::ArrayBuffer positionsVBO;
    positionsVBO.bufferData(vertices.size() * sizeof(VertexType), vertices.data(),
        GL_STATIC_DRAW);

    nsk_cg::ArrayBuffer normalsVBO;
    normalsVBO.bufferData(normals.size() * sizeof(NormalType), normals.data(),
        GL_STATIC_DRAW);

    nsk_cg::VertexArray vao;

    constexpr int positionIndex = 0;
    vao.vertexAttribPointer(positionsVBO, positionIndex, VertexType::nComponents, VertexType::componentType, GL_FALSE, 0, 0);
    vao.enableVertexAttribArray(positionIndex);

    constexpr int normalIndex = 1;
    vao.vertexAttribPointer(normalsVBO, normalIndex, 3, nsk_cg::AttributeType::Float, GL_FALSE, 0, 0);
    vao.enableVertexAttribArray(normalIndex);

    nsk_cg::ElementBuffer ebo;
    ebo.bufferData(vao, indices.size() * sizeof(IndexType),
        object.GetIndices().data(),
        GL_STATIC_DRAW);

    elementBuffers.push_back(std::move(ebo));
    arrayBuffers.push_back(std::move(positionsVBO));
    arrayBuffers.push_back(std::move(normalsVBO));

    return vao;
}

void SetMaterialToShader(const nsk_cg::Material& cubeMaterial, const nsk_cg::ShaderProgram& ourShader)
{
    ourShader.SetVec3("material.ambient", cubeMaterial.ambient);
    ourShader.SetVec3("material.diffuse", cubeMaterial.diffuse);
    ourShader.SetVec3("material.specular", cubeMaterial.specular);
    ourShader.SetFloat("material.shininess", cubeMaterial.shininess);
}

void SetLightSourceToShader(const nsk_cg::Light& light, const nsk_cg::ShaderProgram& ourShader, const glm::mat4& viewMatrix)
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
