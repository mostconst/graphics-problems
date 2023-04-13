#include "Mesh.h"

#include <utility>

namespace nsk_cg
{
Mesh::Mesh(std::vector<Vertex> vertices, const std::vector<IndexTriangle>& triangles, std::vector<glm::vec3> normals)
    : m_vertices(std::move(vertices))
    , m_normals(std::move(normals))
{
    m_indices.reserve(triangles.size() * 3);
    for (const auto& triangle : triangles)
    {
        m_indices.push_back(triangle.First());
        m_indices.push_back(triangle.Second());
        m_indices.push_back(triangle.Third());
    }
}

const std::vector<Vertex>& Mesh::GetVertices() const
{
    return m_vertices;
}

const std::vector<unsigned>& Mesh::GetIndices() const
{
    return m_indices;
}

}