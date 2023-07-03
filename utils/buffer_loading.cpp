#include "buffer_loading.h"

#include <array>
#include <vector>
#include "constants.h"
#include "Mesh.h"
#include "VertexArray.h"
#include "glm/vec2.hpp"

namespace nsk_cg
{
nsk_cg::VertexArray loadBuffers(const nsk_cg::Mesh& object, std::vector<nsk_cg::ArrayBuffer>& arrayBuffers,
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
    vao.vertexAttribPointer(positionsVBO, positionIndex, VertexType::nComponents, VertexType::componentType, GL_FALSE,
                            0, 0);
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

VertexArray loadScreenQuadBuffers(std::vector<nsk_cg::ArrayBuffer>& arrayBuffers,
                                  std::vector<nsk_cg::ElementBuffer>& elementBuffers)
{
    const std::array<glm::vec2, nsk_cg::VERTICES_IN_RECTANGLE> screenCoordinates = {
        glm::vec2{-1.0f, -1.0f},
        glm::vec2{1.0f, -1.0f},
        glm::vec2{1.0f, 1.0f},
        glm::vec2{-1.0f, 1.0f}
    };
    const std::array<glm::vec2, nsk_cg::VERTICES_IN_RECTANGLE> textureCoordinates = {
        glm::vec2{0.0f, 0.0f},
        glm::vec2{1.0f, 0.0f},
        glm::vec2{1.0f, 1.0f},
        glm::vec2{0.0f, 1.0f},
    };
    const std::array<unsigned, nsk_cg::VERTICES_IN_TRIANGLE * 2> indices = {
        0, 1, 2,
        0, 2, 3
    };
    using ScreenCoordinatesType = std::remove_reference_t<decltype(screenCoordinates)>::value_type;
    using TextureCoordinatesType = std::remove_reference_t<decltype(textureCoordinates)>::value_type;
    using IndexType = std::remove_reference_t<decltype(indices)>::value_type;

    nsk_cg::ArrayBuffer screenCoordinatesVbo;
    screenCoordinatesVbo.bufferData(screenCoordinates.size() * sizeof(ScreenCoordinatesType), screenCoordinates.data(),
                                    GL_STATIC_DRAW);

    nsk_cg::ArrayBuffer textureCoordinatesVbo;
    textureCoordinatesVbo.bufferData(textureCoordinates.size() * sizeof(TextureCoordinatesType),
                                     textureCoordinates.data(),
                                     GL_STATIC_DRAW);

    nsk_cg::VertexArray vao;

    constexpr int positionIndex = 0;
    // TODO constants
    vao.vertexAttribPointer(screenCoordinatesVbo, positionIndex, 2, nsk_cg::AttributeType::Float, GL_FALSE, 0, 0);
    vao.enableVertexAttribArray(positionIndex);

    constexpr int normalIndex = 1;
    // TODO constants
    vao.vertexAttribPointer(textureCoordinatesVbo, normalIndex, 2, nsk_cg::AttributeType::Float, GL_FALSE, 0, 0);
    vao.enableVertexAttribArray(normalIndex);

    nsk_cg::ElementBuffer ebo;
    ebo.bufferData(vao, indices.size() * sizeof(IndexType),
                   indices.data(),
                   GL_STATIC_DRAW);

    elementBuffers.push_back(std::move(ebo));
    arrayBuffers.push_back(std::move(screenCoordinatesVbo));
    arrayBuffers.push_back(std::move(textureCoordinatesVbo));

    return vao;
}
}
