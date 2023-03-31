#pragma once

namespace nsk_cg
{
class DrawData final
{
public:
    DrawData(VertexArray* vao, const glm::mat4& placement, const size_t nVertices)
        : vao(vao),
        placement(placement),
        nVertices(nVertices)
    {
    }

    VertexArray* vao;
    glm::mat4 placement;
    size_t nVertices;
};
}