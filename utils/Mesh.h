#pragma once

#include <vector>

#include "IndexTriangle.h"
#include "Vertex.h"

namespace nsk_cg
{
    class Mesh
    {
    public:
        Mesh(std::vector<Vertex> vertices, const std::vector<IndexTriangle>& triangles, std::vector<glm::vec3> normals);
        const std::vector<Vertex>& GetVertices() const;
        const std::vector<glm::vec3>& GetNormals() const { return m_normals; }
        const std::vector<unsigned int>& GetIndices() const;

    private:
        const std::vector<Vertex> m_vertices;
        // TODO const
        std::vector<unsigned int> m_indices;
        const std::vector<glm::vec3> m_normals;
    };
}