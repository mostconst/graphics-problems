#include "MeshBuilding.h"

#include <algorithm>
#include <iterator>

#include "tesselate.h"
#include "VertexTriangle.h"

namespace nsk_cg
{
IndexedVertices makeCubeWithUniqueVertices()
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
    // TODO remove duplication
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
    std::vector<IndexTriangle> indexTriangles;
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

        indexTriangles.emplace_back(vertOffset + 0, vertOffset + 1, vertOffset + 2);
        indexTriangles.emplace_back(vertOffset + 0, vertOffset + 2, vertOffset + 3);
    }

    std::vector<glm::vec3> normals(resultVertices.size(), glm::vec3(0.0f));
    for(const auto& triangle : indexTriangles)
    {
        const glm::vec3 normal = getNormalVector(VertexTriangle(resultVertices[triangle.First()], resultVertices[triangle.Second()], resultVertices[triangle.Third()]));
        normals[triangle.First()] = normal;
        normals[triangle.Second()] = normal;
        normals[triangle.Third()] = normal;
    }
    return { std::move(resultVertices), indexTriangles, std::move(normals) };
}

Mesh makeSphere(const int tesselationLevel)
{
    const auto tessResult = tesselateIterative(tesselationLevel);
    const auto& pyramidVertices = tessResult.GetVertices();
    std::vector<nsk_cg::Vertex> morphedVertices;
    morphedVertices.reserve(pyramidVertices.size());
    std::transform(pyramidVertices.cbegin(), pyramidVertices.cend(), std::back_inserter(morphedVertices),
                   mapToUnitSphere);

    std::vector<glm::vec3> normals;
    normals.reserve(morphedVertices.size());
    for(const auto& vertex : morphedVertices)
    {
        normals.push_back(vertex.ToVector());
    }

    return {morphedVertices, tessResult.GetTriangles(), normals};
}
}
