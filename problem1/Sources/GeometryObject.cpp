#include "GeometryObject.h"

#include <cassert>

GeometryObject::GeometryObject(std::vector<float> vertices, std::vector<float> colors, std::vector<unsigned> indices)
    : m_vertices(std::move(vertices)),
      m_colors(std::move(colors)),
      m_indices(std::move(indices))
{
    assert(m_vertices.size() == m_colors.size());
}

const std::vector<float>& GeometryObject::GetVertices() const
{
    return m_vertices;
}

const std::vector<float>& GeometryObject::GetColors() const
{
    return m_colors;
}

const std::vector<unsigned>& GeometryObject::GetIndices() const
{
    return m_indices;
}

GeometryObject makeRainbowCube()
{
    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
    };
    assert(vertices.size() == 8 * 3);
    std::vector<float> colors = {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.5f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.3f, 0.0f, 0.5f,
        0.6f, 0.0f, 0.8f,
        1.0f, 0.4f, 0.7f,
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

GeometryObject makePlatform()
{
    std::vector<float> vertices = {
        0.0f, 0.0f, 0.0f,
        20.0f, 0.0f, 0.0f,
        20.0f, 20.0f, 0.0f,
        0.0f, 20.0f, 0.0f,
    };
    std::vector<float> colors(4 * 3, 0.5f);
    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
    };

    return { std::move(vertices), std::move(colors), std::move(indices) };
}
