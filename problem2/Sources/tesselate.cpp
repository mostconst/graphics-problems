#include "tesselate.h"

#include <cassert>
#include <random>

#include "glm/vec3.hpp"

VertInd tesselate(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices)
{
    auto out_vertices = vertices;
    std::vector<unsigned int> out_indices;
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        const auto ind0 = indices[i];
        const auto ind1 = indices[i + 1];
        const auto ind2 = indices[i + 2];
        const auto ind3 = out_vertices.size() + 0;
        const auto ind4 = out_vertices.size() + 1;
        const auto ind5 = out_vertices.size() + 2;
        out_vertices.push_back((vertices[ind1] + vertices[ind2]) / 2.0f);
        out_vertices.push_back((vertices[ind0] + vertices[ind2]) / 2.0f);
        out_vertices.push_back((vertices[ind0] + vertices[ind1]) / 2.0f);

        out_indices.push_back(ind0);
        out_indices.push_back(ind5);
        out_indices.push_back(ind4);

        out_indices.push_back(ind5);
        out_indices.push_back(ind1);
        out_indices.push_back(ind3);

        out_indices.push_back(ind3);
        out_indices.push_back(ind4);
        out_indices.push_back(ind5);

        out_indices.push_back(ind4);
        out_indices.push_back(ind3);
        out_indices.push_back(ind2);
    }

    return { out_vertices, out_indices };
}

VertInd getInitialPyramid()
{
    std::vector<glm::vec3> vertices = {
        {0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, 0.0f, -1.0f},
    };
    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,
        5, 1, 4,
        5, 4, 3,
        5, 3, 2,
        5, 2, 1,
    };

    return { std::move(vertices), std::move(indices) };
}

std::vector<utils::Color> generateColors(size_t nColors)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::vector<utils::Color> colors;
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 0xFFFFFF);
    for (size_t i = 0; i != nColors; ++i)
    {
        colors.emplace_back(dist6(rng));
    }

    return colors;
}

GeometryObject makePyramid(const int tesselationLevel)
{
    assert(tesselationLevel >= 0);

    auto tessResult = getInitialPyramid();

    for (int i = 0; i < tesselationLevel; ++i)
    {
        tessResult = tesselate(tessResult.first, tessResult.second);
    }
    auto vertices = tessResult.first;
    auto indices = tessResult.second;

    std::vector<utils::Vertex> verticesRes;
    verticesRes.reserve(vertices.size());
    for (const auto& v : vertices)
    {
        verticesRes.emplace_back(v.x, v.y, v.z);
    }

    std::vector<utils::Color> colors = generateColors(vertices.size());

    return { std::move(verticesRes), std::move(colors), std::move(indices) };
}

VertInd tesselateIter(const VertInd& input, int tesselationLevel)
{
    assert(tesselationLevel > 0);
    assert(input.second.size() % 3 == 0);
    const auto& indices = input.second;
    std::vector<glm::vec3> outVertices = input.first;
    std::vector<unsigned int> outIndices;
    for (size_t inputTriangle = 0; inputTriangle < indices.size(); inputTriangle += 3)
    {
        const int nRows = std::pow(2, tesselationLevel);
        const glm::vec3& vertex0 = input.first[inputTriangle + 0];
        const glm::vec3& vertex1 = input.first[inputTriangle + 1];
        const glm::vec3& vertex2 = input.first[inputTriangle + 2];
        const glm::vec3 downUnit = (vertex1 - vertex0) / static_cast<float>(nRows);
        const glm::vec3 rightUnit = (vertex2 - vertex1) / static_cast<float>(nRows);
        for (int row = 0; row < nRows; ++row)
        {
            for (int col = 0; col < 2 * row + 1; ++col)
            {
                if (col % 2 == 0)
                {
                    const unsigned index0 = 0;
                }
            }
        }
    }

    return { outVertices, outIndices };
}
