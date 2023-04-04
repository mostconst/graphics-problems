#include "Mesh.h"

namespace nsk_cg
{
Mesh::Mesh(std::vector<Vertex> first, std::vector<IndexTriangle> second) : m_vertices(std::move(first)),
    m_triangles(std::move(second))
{
    for(const auto& triangle: m_triangles)
    {
        assert(triangle.First() < m_vertices.size());
        assert(triangle.Second() < m_vertices.size());
        assert(triangle.Third() < m_vertices.size());
    }
}

const std::vector<Vertex>& Mesh::GetVertices() const
{
    return m_vertices;
}

const std::vector<IndexTriangle>& Mesh::GetTriangles() const
{
    return m_triangles;
}

}