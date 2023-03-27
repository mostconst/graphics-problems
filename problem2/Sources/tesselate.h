#pragma once
#include "GeometryObject.h"
#include "glm/vec3.hpp"

struct IndexTriangle
{
    // TODO cleanup
    const unsigned int first;
    const unsigned int second;
    const unsigned int third;
};

using VertInd = std::pair<std::vector<glm::vec3>, std::vector<IndexTriangle>>;

VertInd tesselateIter(int tesselationLevel);