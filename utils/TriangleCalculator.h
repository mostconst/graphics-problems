#pragma once
#include <cassert>

namespace nsk_cg
{
class TriangleCalculator
{
public:
    explicit TriangleCalculator(const int tesselationLevel)
        : tesselationLevel(tesselationLevel)
    {
        assert(tesselationLevel > 0);
    }

    int getTriangleIndex(int face, int row, int column) const;
    int getRowsPerFace() const;
    int getTrianglesInFace() const;
    static int TotalSides() { return totalSides; }
    static int getTrianglesInRow(int row);
    static bool isBottomFace(int face);
    static int getTopAdjacentFace(int face);

private:
    const int tesselationLevel;
    static constexpr int nPyramids = 2;
    static constexpr int pyramidSides = 4;
    static constexpr int totalSides = nPyramids * pyramidSides;
};
}
