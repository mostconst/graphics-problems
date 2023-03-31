#include "TriangleCalculator.h"

#include <cassert>

namespace nsk_cg
{
int TriangleCalculator::getTriangleIndex(const int face, const int row, const int column) const
{
    assert(face >= 0);
    assert(row >= 0);
    assert(column >= 0);
    return face * getTrianglesInFace() + row * row + column;
}

int power(const int base, const int exponent)
{
    assert(base == 2 || base == 4);
    assert(exponent <= 10);
    int res = 1;
    for (int i = 0; i < exponent; i++)
    {
        res *= base;
    }

    return res;
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

bool TriangleCalculator::isBottomFace(const int face)
{
    assert(0 <= face && face < totalSides);
    return face >= pyramidSides;
}

int TriangleCalculator::getTopAdjacentFace(const int face)
{
    assert(isBottomFace(face));
    return totalSides - 1 - face;
}
}
