#include "MeshBuilding.h"

namespace nsk_cg
{
Mesh makeCubeWithUniqueVertices()
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

    std::vector<IndexTriangle> indices = {
        {0, 1, 3},
        {0, 3, 2},
        {5, 4, 6},
        {5, 6, 7},
        {1, 5, 7},
        {1, 7, 3},
        {4, 0, 2},
        {4, 2, 6},
        {4, 5, 1},
        {4, 1, 0},
        {2, 3, 7},
        {2, 7, 6},
    };
    return { std::move(vertices), std::move(indices)};
}

Mesh makeCubeForLighting()
{
    const std::vector<Vertex> vertices = {
    {-0.5f, -0.5f, 0.5f},
    {0.5f, -0.5f, 0.5f},
    {-0.5f, 0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f},
    {-0.5f, -0.5f, -0.5f},
    {0.5f, -0.5f, -0.5f},
    {-0.5f, 0.5f, -0.5f},
    {0.5f, 0.5f, -0.5f},
    };
    const std::vector<unsigned int> indices = {
        0, 1, 3, 0, 3, 2,
        5, 4, 6, 5, 6, 7,
        1, 5, 7, 1, 7, 3,
        4, 0, 2, 4, 2, 6,
        4, 5, 1, 4, 1, 0,
        2, 3, 7, 2, 7, 6,
    };
    std::vector<Vertex> resultVertices;
    std::vector<IndexTriangle> resultIndices;
    constexpr size_t cubeFaces = 6;
    for (size_t face = 0; face < cubeFaces; ++face)
    {
        const size_t faceStart = face * 6;
        //Sassert(faceStart == resultVertices.size());
        resultVertices.push_back(vertices[indices[faceStart]]);
        resultVertices.push_back(vertices[indices[faceStart + 1]]);
        resultVertices.push_back(vertices[indices[faceStart + 2]]);
        resultVertices.push_back(vertices[indices[faceStart + 5]]);

        const size_t vertOffset = face * 4;

        resultIndices.emplace_back(vertOffset + 0, vertOffset + 1, vertOffset + 2);
        resultIndices.emplace_back(vertOffset + 0, vertOffset + 2, vertOffset + 3);
    }
    return { std::move(resultVertices), std::move(resultIndices) };
}
}
