#include "tesselate.h"

#include <cassert>
#include <random>

#include "glm/vec3.hpp"
#include "conversion.h"
#include "IndexTriangle.h"
#include "TriangleCalculator.h"

namespace nsk_cg
{
IndexedGeometry tesselate(const std::vector<glm::vec3>& vertices, const std::vector<nsk_cg::IndexTriangle>& triangles)
{
    auto out_vertices = vertices;
    std::vector<nsk_cg::IndexTriangle> outTriangles;
    for (const auto& triangle : triangles)
    {
        const auto ind0 = triangle.First();
        const auto ind1 = triangle.Second();
        const auto ind2 = triangle.Third();
        const auto ind3 = nsk_cg::glSize(out_vertices) + 0;
        const auto ind4 = nsk_cg::glSize(out_vertices) + 1;
        const auto ind5 = nsk_cg::glSize(out_vertices) + 2;
        out_vertices.push_back((vertices[ind1] + vertices[ind2]) / 2.0f);
        out_vertices.push_back((vertices[ind0] + vertices[ind2]) / 2.0f);
        out_vertices.push_back((vertices[ind0] + vertices[ind1]) / 2.0f);

        outTriangles.emplace_back(ind0, ind5, ind4);
        outTriangles.emplace_back(ind5, ind1, ind3);
        outTriangles.emplace_back(ind3, ind4, ind5);
        outTriangles.emplace_back(ind4, ind3, ind2);
    }

    return { out_vertices, outTriangles };
}

IndexedGeometry getInitialPyramid()
{
    std::vector<glm::vec3> vertices = {
        {0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, 0.0f, -1.0f},
    };
    std::vector<nsk_cg::IndexTriangle> indices = {
        {0, 1, 2},
        {0, 2, 3},
        {0, 3, 4},
        {0, 4, 1},
        {5, 1, 4},
        {5, 4, 3},
        {5, 3, 2},
        {5, 2, 1},
    };

    return { std::move(vertices), std::move(indices) };
}

IndexedGeometry tesselateRecursive(const IndexedGeometry& initial, const int tesselationLevel)
{
    auto tessResult = initial;

    for (int i = 0; i < tesselationLevel; ++i)
    {
        tessResult = tesselate(tessResult.first, tessResult.second);
    }

    return tessResult;
}

/*
                          0,  1,  2,
             3,  4,  5,   6,  7,  8,  9, 10, 11,
12, 13, 14, 15, 16, 17,  18, 19, 20
*/

static int indexFromEnd(const int index, const int total)
{
    return total - 1 - index;
}

IndexedGeometry tesselateIterative(const IndexedGeometry& input, const int tesselationLevel)
{
    assert(tesselationLevel > 0);
    assert(sizeToInt(input.second.size()) == TriangleCalculator::TotalSides());
    const auto& triangles = input.second;
    std::vector<glm::vec3> outVertices = input.first;
    const TriangleCalculator triangleCalculator{ tesselationLevel };
    std::vector<nsk_cg::IndexTriangle> outTriangles;
    outTriangles.reserve(triangles.size() * triangleCalculator.getTrianglesInFace());

    for (int face = 0; face < nsk_cg::sizeToInt(triangles.size()); ++face)
    {
        const auto& faceTriangle = triangles[face];
        const glm::vec3& vertex0 = input.first[faceTriangle.First()];
        const glm::vec3& vertex1 = input.first[faceTriangle.Second()];
        const glm::vec3& vertex2 = input.first[faceTriangle.Third()];
        const int nRows = triangleCalculator.getRowsPerFace();
        const int lastRow = nRows - 1;
        const glm::vec3 downUnit = (vertex1 - vertex0) / static_cast<float>(nRows);
        const glm::vec3 rightUnit = (vertex2 - vertex1) / static_cast<float>(nRows);
        for (int row = 0; row < nRows; ++row)
        {
            const int trianglesInRow = TriangleCalculator::getTrianglesInRow(row);
            for (int col = 0; col < trianglesInRow; ++col)
            {
                if (col % 2 == 0)
                {
                    unsigned int index0;
                    if (row == 0)
                    {
                        index0 = faceTriangle.First();
                    }
                    else if (col == 0)
                    {
                        index0 = outTriangles[triangleCalculator.getTriangleIndex(face, row - 1, 0)].Second();
                    }
                    else
                    {
                        index0 = outTriangles.back().Second();
                    }

                    unsigned int index1;
                    const int lastColumn = trianglesInRow - 1;
                    if (TriangleCalculator::isBottomFace(face) && row == lastRow)
                    {
                        index1 = outTriangles[triangleCalculator.getTriangleIndex(TriangleCalculator::getTopAdjacentFace(face), row, indexFromEnd(col, trianglesInRow))].Third();
                    }
                    else if (col == 0)
                    {
                        if (face % 4 == 0)
                        {
                            if (row == lastRow)
                            {
                                index1 = faceTriangle.Second();
                            }
                            else
                            {
                                index1 = nsk_cg::glSize(outVertices);
                                outVertices.push_back(vertex0 + downUnit * static_cast<float>(row + 1));
                            }
                        }
                        else
                        {
                            index1 = outTriangles[triangleCalculator.getTriangleIndex(face - 1, row, lastColumn)].Third();
                        }
                    }
                    else
                    {
                        index1 = outTriangles.back().First();
                    }

                    unsigned int index2;
                    if (TriangleCalculator::isBottomFace(face) && row == lastRow)
                    {
                        index2 = outTriangles[triangleCalculator.getTriangleIndex(TriangleCalculator::getTopAdjacentFace(face), row, indexFromEnd(col, trianglesInRow))].Second();
                    }
                    else if ((face == 3 || face == 7) && col == lastColumn)
                    {
                        index2 = outTriangles[triangleCalculator.getTriangleIndex(face - 3, row, 0)].Second();
                    }
                    else
                    {
                        if (row == lastRow && col == lastColumn)
                        {
                            index2 = faceTriangle.Third();
                        }
                        else
                        {
                            index2 = nsk_cg::glSize(outVertices);
                            outVertices.push_back(vertex0 + downUnit * static_cast<float>(row + 1) + rightUnit * (static_cast<float>(col) / 2 + 1));
                        }
                    }
                    outTriangles.emplace_back(index0, index1, index2);
                }
                else
                {
                    const auto triangleAbove = triangleCalculator.getTriangleIndex(face, row - 1, col - 1);
                    outTriangles.emplace_back(outTriangles.back().Third(), outTriangles[triangleAbove].Third(), outTriangles[triangleAbove].Second());
                }
            }
        }
    }
    assert(outTriangles.size() == triangles.size() * triangleCalculator.getTrianglesInFace());

    return { outVertices, outTriangles };
}

IndexedGeometry tesselateIterative(const int tesselationLevel)
{
    return tesselateIterative(getInitialPyramid(), tesselationLevel);
}
}
