#include "tesselate.h"

#include <cassert>
#include <random>

#include "glm/vec3.hpp"
#include "convertion.h"

VertInd tesselate(const std::vector<glm::vec3>& vertices, const std::vector<IndexTriangle>& triangles)
{
    auto out_vertices = vertices;
    std::vector<IndexTriangle> outTriangles;
    for (const auto& triangle : triangles)
    {
        const auto ind0 = triangle.first;
        const auto ind1 = triangle.second;
        const auto ind2 = triangle.third;
        const auto ind3 = glSize(out_vertices) + 0;
        const auto ind4 = glSize(out_vertices) + 1;
        const auto ind5 = glSize(out_vertices) + 2;
        out_vertices.push_back((vertices[ind1] + vertices[ind2]) / 2.0f);
        out_vertices.push_back((vertices[ind0] + vertices[ind2]) / 2.0f);
        out_vertices.push_back((vertices[ind0] + vertices[ind1]) / 2.0f);

        outTriangles.push_back({ind0, ind5, ind4});
        outTriangles.push_back({ind5, ind1, ind3});
        outTriangles.push_back({ind3, ind4, ind5});
        outTriangles.push_back({ind4, ind3, ind2});
    }

    return { out_vertices, outTriangles };
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
    std::vector<IndexTriangle> indices = {
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

VertInd tesselateRecursive(const VertInd& initial, const int tesselationLevel)
{
    auto tessResult = initial;

    for (int i = 0; i < tesselationLevel; ++i)
    {
        tessResult = tesselate(tessResult.first, tessResult.second);
    }

    return tessResult;
}

// TODO cleanup
int power(const int base, const int numberAbove)
{
    int res = 1;
    for (int i = 0; i < numberAbove; i++)
    {
        res *= base;
    }

    return res;
}

class TriangleCalculator
{
public:
    explicit TriangleCalculator(int tesselationLevel)
        : tesselationLevel(tesselationLevel)
    {
        assert(tesselationLevel > 0);
    }

    int getTriangleIndex(int face, int row, int column) const;
    int getRowsPerFace() const;
    int getTrianglesInFace() const;
    static int getTrianglesInRow(int row);

private:
    const int tesselationLevel;
};

int TriangleCalculator::getTriangleIndex(const int face, const int row, const int column) const
{
    assert(face >= 0);
    assert(row >= 0);
    assert(column >= 0);
    return face * getTrianglesInFace() + row * row + column;
}

int TriangleCalculator::getRowsPerFace() const
{
    return power(2, tesselationLevel);
}

int TriangleCalculator::getTrianglesInFace() const
{
    return power(4, tesselationLevel);
}

int TriangleCalculator::getTrianglesInRow(const int row)
{
    assert(row >= 0);
    return 2 * row + 1;
}

/*
                          0,  1,  2,
             3,  4,  5,   6,  7,  8,  9, 10, 11,
12, 13, 14, 15, 16, 17,  18, 19, 20
*/

constexpr int nPyramids = 2;
constexpr int pyramidSides = 4;
constexpr int totalSides = nPyramids * pyramidSides;

bool isBottomFace(const int face)
{
    assert(0 <= face && face < totalSides);
    return face >= pyramidSides;
}

int getTopAdjacentFace(const int face)
{
    assert(isBottomFace(face));
    return totalSides - 1 - face;
}

int indexFromEnd(const int index, const int total)
{
    return total - 1 - index;
}

 VertInd tesselateIter(const VertInd& input, const int tesselationLevel)
 {
     assert(tesselationLevel > 0);
     assert(input.second.size() == totalSides);
     const auto& triangles = input.second;
     std::vector<glm::vec3> outVertices = input.first;
     const TriangleCalculator triangleCalculator{ tesselationLevel };
     std::vector<IndexTriangle> outTriangles;
     outTriangles.reserve(triangles.size() * triangleCalculator.getTrianglesInFace());

     for (int face = 0; face < triangles.size(); ++face)
     {
         const auto& faceTriangle = triangles[face];
         const glm::vec3& vertex0 = input.first[faceTriangle.first];
         const glm::vec3& vertex1 = input.first[faceTriangle.second];
         const glm::vec3& vertex2 = input.first[faceTriangle.third];
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
                     unsigned int index0 = 0;
                     if (row == 0)
                     {
                         index0 = faceTriangle.first;
                     }
                     else if (col == 0)
                     {
                         index0 = outTriangles[triangleCalculator.getTriangleIndex(face, row - 1, 0)].second;
                     }
                     else
                     {
                         index0 = outTriangles.back().second;
                     }

                     unsigned int index1 = 0;
                     const int lastColumn = trianglesInRow - 1;
                     if (isBottomFace(face) && row == lastRow)
                     {
                         index1 = outTriangles[triangleCalculator.getTriangleIndex(getTopAdjacentFace(face), row, indexFromEnd(col, trianglesInRow))].third;
                     }
                     else if (col == 0)
                     {
                         if (face % 4 == 0)
                         {
                             // TODO last row
                             if (row == lastRow)
                             {
                                 index1 = faceTriangle.second;
                             }
                             else
                             {
                                 index1 = glSize(outVertices);
                                 outVertices.push_back(vertex0 + downUnit * static_cast<float>(row + 1));
                             }
                         }
                         else
                         {
                             // TODO last column
                             index1 = outTriangles[triangleCalculator.getTriangleIndex(face - 1, row, lastColumn)].third;
                         }
                     }
                     else
                     {
                         index1 = outTriangles.back().first;
                     }

                     unsigned int index2 = 0;
                     if (isBottomFace(face) && row == lastRow)
                     {
                         index2 = outTriangles[triangleCalculator.getTriangleIndex(getTopAdjacentFace(face), row, indexFromEnd(col, trianglesInRow))].second;
                     }
                     else if ((face == 3 || face == 7) && col == lastColumn)
                     {
                         index2 = outTriangles[triangleCalculator.getTriangleIndex(face - 3, row, 0)].second;
                     }
                     else
                     {
                         if (row == lastRow && col == lastColumn)
                         {
                             index2 = faceTriangle.third;
                         }
                         else
                         {
                             index2 = glSize(outVertices);
                             outVertices.push_back(vertex0 + downUnit * static_cast<float>(row + 1) + rightUnit * static_cast<float>(col / 2 + 1));
                         }
                     }
                     outTriangles.push_back({ index0, index1, index2 });
                 }
                 else
                 {
                     const auto triangleAbove = triangleCalculator.getTriangleIndex(face, row - 1, col - 1);
                     outTriangles.push_back({outTriangles.back().third, outTriangles[triangleAbove].third, outTriangles[triangleAbove].second});
                 }
             }
         }
     }
     assert(outTriangles.size() == triangles.size() * triangleCalculator.getTrianglesInFace());

     return { outVertices, outTriangles };
 }

VertInd tesselateIter(const int tesselationLevel)
{
    return tesselateIter(getInitialPyramid(), tesselationLevel);
}
