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

std::vector<unsigned> Mesh::GetIndices() const
{
    std::vector<unsigned int> indicesRes;
    indicesRes.reserve(m_triangles.size() * 3);
    for (const auto& triangle : m_triangles)
    {
        indicesRes.push_back(triangle.First());
        indicesRes.push_back(triangle.Second());
        indicesRes.push_back(triangle.Third());
    }

    return indicesRes;
}
}
