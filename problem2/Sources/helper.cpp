﻿#include "helper.h"

#include <cassert>
#include <random>

#include "GeometryObject.h"
#include "tesselate.h"

VertexArray LoadBuffers(const GeometryObject& object, const std::vector<utils::Vertex>& morphedVertices, std::vector<ArrayBuffer>& arrayBuffers,
                        std::vector<ElementBuffer>& elementBuffers)
{
    assert(object.GetVertices().size() == morphedVertices.size());

    const auto& colors = object.GetColors();
    const auto& vertices = object.GetVertices();
    const auto& indices = object.GetIndices();
    using ColorType = std::remove_reference_t<decltype(colors)>::value_type;
    using VertexType = std::remove_reference_t<decltype(vertices)>::value_type;
    using IndexType = std::remove_reference_t<decltype(indices)>::value_type;

    ArrayBuffer positionsVBO;
    positionsVBO.bufferData(vertices.size() * sizeof(VertexType), vertices.data(),
                            GL_STATIC_DRAW);
    ArrayBuffer colorsVBO;
    colorsVBO.bufferData(colors.size() * sizeof(ColorType), colors.data(),
                         GL_STATIC_DRAW);

    ArrayBuffer positions2VBO;
    positions2VBO.bufferData(morphedVertices.size() * sizeof(VertexType), morphedVertices.data(),
                             GL_STATIC_DRAW);

    // HW_ITEM 7
    VertexArray vao;
    constexpr int positionIndex = 0;
    vao.vertexAttribPointer(positionsVBO, positionIndex, VertexType::nComponents, VertexType::componentType, GL_FALSE, 0, 0);
    vao.enableVertexAttribArray(positionIndex);

    constexpr int colorIndex = 1;
    vao.vertexAttribPointer(colorsVBO, colorIndex, ColorType::nComponents, ColorType::componentType, GL_FALSE, 0, 0);
    vao.enableVertexAttribArray(colorIndex);

    constexpr int positionIndex2 = 2;
    vao.vertexAttribPointer(positions2VBO, positionIndex2, VertexType::nComponents, VertexType::componentType, GL_FALSE, 0, 0);
    vao.enableVertexAttribArray(positionIndex2);

    ElementBuffer ebo;
    ebo.bufferData(vao, indices.size() * sizeof(IndexType),
                   object.GetIndices().data(),
                   GL_STATIC_DRAW);

    elementBuffers.push_back(std::move(ebo));
    arrayBuffers.push_back(std::move(positionsVBO));
    arrayBuffers.push_back(std::move(positions2VBO));
    arrayBuffers.push_back(std::move(colorsVBO));

    return vao;
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

    auto tessResult = tesselateIter(tesselationLevel);

    auto vertices = tessResult.first;
    auto triangles = tessResult.second;

    std::vector<utils::Vertex> verticesRes;
    verticesRes.reserve(vertices.size());
    for (const auto& v : vertices)
    {
        verticesRes.emplace_back(v.x, v.y, v.z);
    }

    std::vector<size_t> indicesRes;
    indicesRes.reserve(triangles.size() * 3);
    for (const auto& triangle : triangles)
    {
        indicesRes.push_back(triangle.first);
        indicesRes.push_back(triangle.second);
        indicesRes.push_back(triangle.third);
    }

    std::vector<utils::Color> colors = generateColors(vertices.size());

    return { std::move(verticesRes), std::move(colors), std::move(indicesRes) };
}