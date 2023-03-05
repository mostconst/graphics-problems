#pragma once

class DrawData
{
public:
    DrawData(VertexArray* vao, const glm::mat4& placement, size_t nVertices)
        : vao(vao),
        placement(placement),
        nVertices(nVertices)
    {
    }

    VertexArray* vao;
    glm::mat4 placement;
    size_t nVertices;
};