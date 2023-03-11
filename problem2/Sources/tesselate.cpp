#include "tesselate.h"

#include <cassert>
#include <random>

using VertInd = std::pair<std::vector<utils::Vertex>, std::vector<unsigned int>>;

VertInd tesselate(const std::vector<utils::Vertex>& vertices, const std::vector<unsigned int>& indices)
{
    auto out_vertices = vertices;
    auto out_indices = indices;
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        const auto ind0 = indices[i];
        const auto ind1 = indices[i + 1];
        const auto ind2 = indices[i + 2];
        const auto ind3 = out_vertices.size() + 0;
        const auto ind4 = out_vertices.size() + 1;
        const auto ind5 = out_vertices.size() + 2;
        out_vertices.push_back(middle(vertices[ind0], vertices[ind1]));
        out_vertices.push_back(middle(vertices[ind1], vertices[ind2]));
        out_vertices.push_back(middle(vertices[ind2], vertices[ind0]));

        out_indices.push_back(ind0);
        out_indices.push_back(ind3);
        out_indices.push_back(ind5);

        out_indices.push_back(ind3);
        out_indices.push_back(ind1);
        out_indices.push_back(ind4);

        out_indices.push_back(ind5);
        out_indices.push_back(ind4);
        out_indices.push_back(ind2);

        out_indices.push_back(ind3);
        out_indices.push_back(ind4);
        out_indices.push_back(ind5);
    }

    return { out_vertices, out_indices };
}

GeometryObject makePyramid(const int tesselationLevel)
{
    assert(tesselationLevel >= 0);

    std::vector<utils::Vertex> _vertices = {
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, -1.0f},
    };
    std::vector<unsigned int> _indices = {
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4,
        0, 1, 5,
        1, 2, 5,
        2, 3, 5,
        3, 0, 5,
    };

    auto tessResult = std::pair{_vertices, _indices};

    for (int i = 0; i < tesselationLevel; ++i)
    {
        tessResult = tesselate(tessResult.first, tessResult.second);
    }
    auto vertices = tessResult.first;
    auto indices = tessResult.second;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 0xFFFFFF);
    std::vector<utils::Color> colors;
    for (size_t i = 0; i != vertices.size(); ++i)
    {
        colors.emplace_back(dist6(rng));
    }

    return { std::move(vertices), std::move(colors), std::move(indices) };
}
