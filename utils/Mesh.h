#pragma once
#include <vector>

#include "IndexTriangle.h"
#include "Vertex.h"

namespace nsk_cg
{
class Mesh
{
public:
    Mesh(std::vector<Vertex> first, std::vector<IndexTriangle> second);
    const std::vector<Vertex>& GetVertices() const;
    const std::vector<IndexTriangle>& GetTriangles() const;

private:
    std::vector<Vertex> m_vertices;
    std::vector<IndexTriangle> m_triangles;
};
}
