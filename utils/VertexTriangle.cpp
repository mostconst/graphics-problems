#include "glm/geometric.hpp"

#include "VertexTriangle.h"
#include "comparison.h"

namespace nsk_cg
{
VertexTriangle::VertexTriangle(const Vertex& first, const Vertex& second, const Vertex& third) : first(first),
    second(second),
    third(third)
{
}

Vertex VertexTriangle::First() const
{
    return first;
}

Vertex VertexTriangle::Second() const
{
    return second;
}

Vertex VertexTriangle::Third() const
{
    return third;
}

bool isCorrect(const VertexTriangle& triangle)
{
    const float firstSide = distance(triangle.First(), triangle.Second());
    const float secondSide = distance(triangle.Second(), triangle.Third());
    const float thirdSide = distance(triangle.First(), triangle.Third());
    const bool correct = lessThan(firstSide, secondSide + thirdSide) &&
        lessThan(secondSide, firstSide + thirdSide) &&
        lessThan(thirdSide, firstSide + secondSide);

    return correct;
}

glm::vec3 getNormalVector(const VertexTriangle& triangle)
{
    assert(isCorrect(triangle));
    const glm::vec3 side1 = triangle.Second().Subtract(triangle.First());
    const glm::vec3 side2 = triangle.Third().Subtract(triangle.First());
    const glm::vec3 cross = glm::cross(side1, side2);

    return glm::normalize(cross);
}
}
