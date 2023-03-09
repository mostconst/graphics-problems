#include "GeometryObject.h"

#include <cassert>

GeometryObject::GeometryObject(std::vector<utils::Vertex> vertices, std::vector<utils::Color> colors,
                               std::vector<unsigned> indices)
    : m_vertices(std::move(vertices)),
      m_colors(std::move(colors)),
      m_indices(std::move(indices))
{
    assert(m_vertices.size() == m_colors.size());
}

const std::vector<utils::Vertex>& GeometryObject::GetVertices() const
{
    return m_vertices;
}

const std::vector<utils::Color>& GeometryObject::GetColors() const
{
    return m_colors;
}

const std::vector<unsigned>& GeometryObject::GetIndices() const
{
    return m_indices;
}

GeometryObject makeRainbowCube()
{
    std::vector<utils::Vertex> vertices = {
        {-0.5f, -0.5f, 0.5f},
        {0.5f, -0.5f, 0.5f},
        {-0.5f, 0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},
        {-0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        {-0.5f, 0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},
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
    std::vector<unsigned int> indices = {
        0, 1, 3, 0, 3, 2,
        5, 4, 6, 5, 6, 7,
        1, 5, 7, 1, 7, 3,
        4, 0, 2, 4, 2, 6,
        4, 5, 1, 4, 1, 0,
        2, 3, 7, 2, 7, 6,
    };
    return {std::move(vertices), std::move(colors), std::move(indices)};
}

GeometryObject makePlatform(float size)
{
    std::vector<utils::Vertex> vertices = {
        {-size, -size, 0.0f},
        {size, -size, 0.0f},
        {size, size, 0.0f},
        {-size, size, 0.0f},
    };
    std::vector<utils::Color> colors(4, {0.5f, 0.5f, 0.5f});
    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
    };

    return {std::move(vertices), std::move(colors), std::move(indices)};
}
