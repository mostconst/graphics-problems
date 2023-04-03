#include "Mesh.h"

namespace nsk_cg
{
Mesh::Mesh(std::vector<Vertex> first, std::vector<IndexTriangle> second) : first(std::move(first)),
    second(std::move(second))
{
}

const std::vector<Vertex>& Mesh::GetVertices() const
{
    return first;
}

const std::vector<IndexTriangle>& Mesh::GetTriangles() const
{
    return second;
}

}