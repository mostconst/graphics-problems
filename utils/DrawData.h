﻿#pragma once
#include "Material.h"

namespace nsk_cg
{
class DrawData final
{
public:
    DrawData(const VertexArray* const vao, const glm::mat4& placement, const size_t nVertices)
        : vao(vao),
        placement(placement),
        nVertices(nVertices)
    {
    }

    const VertexArray* vao;
    const glm::mat4 placement;
    const size_t nVertices;
};

class ExtendedDrawData final
{
public:
    ExtendedDrawData(const VertexArray* const vao, const Material& material, const glm::mat4& placement, const size_t nVertices)
        : vao(vao),
          m_material(material),
          placement(placement),
          nVertices(nVertices)
    {
    }

    const VertexArray* vao;
    const Material m_material;
    const glm::mat4 placement;
    const size_t nVertices;
};
}
