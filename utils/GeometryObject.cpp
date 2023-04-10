#include "GeometryObject.h"

#include <cassert>

namespace nsk_cg
{
GeometryObject::GeometryObject(std::vector<Vertex> vertices, std::vector<utils::Color> colors,
    std::vector<unsigned int> indices)
    : m_vertices(std::move(vertices)),
    m_colors(std::move(colors)),
    m_indices(std::move(indices))
{
    assert(m_vertices.size() == m_colors.size());
}

const std::vector<Vertex>& GeometryObject::GetVertices() const
{
    return m_vertices;
}

const std::vector<utils::Color>& GeometryObject::GetColors() const
{
    return m_colors;
}

const std::vector<unsigned int>& GeometryObject::GetIndices() const
{
    return m_indices;
}

GeometryObject makeRainbowCube()
{
    std::vector<Vertex> vertices = {
        {-0.5f, -0.5f, 0.5f},
        {0.5f, -0.5f, 0.5f},
        {-0.5f, 0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},
        {-0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        {-0.5f, 0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},
    };

    std::vector<unsigned int> indices = {
        0, 1, 3, 0, 3, 2,
        5, 4, 6, 5, 6, 7,
        1, 5, 7, 1, 7, 3,
        4, 0, 2, 4, 2, 6,
        4, 5, 1, 4, 1, 0,
        2, 3, 7, 2, 7, 6,
    };
    std::vector<utils::Color> colors = {
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.5f, 0.0f},
        {1.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {0.3f, 0.0f, 0.5f},
        {0.6f, 0.0f, 0.8f},
        {1.0f, 0.4f, 0.7f},
    };
    return { std::move(vertices), std::move(colors), std::move(indices) };
}

GeometryObject makePlatform(float size)
{
    std::vector<Vertex> vertices = {
        {-size, -size, 0.0f},
        {size, -size, 0.0f},
        {size, size, 0.0f},
        {-size, size, 0.0f},
    };
    std::vector<utils::Color> colors(4, { 0.5f, 0.5f, 0.5f });
    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
    };

    return { std::move(vertices), std::move(colors), std::move(indices) };
}
}
