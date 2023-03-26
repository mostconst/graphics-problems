#pragma once
#include "GeometryObject.h"
#include "glm/vec3.hpp"

struct IndexTriangle
{
    // TODO cleanup
    const size_t first;
    const size_t second;
    const size_t third;
};

using VertInd = std::pair<std::vector<glm::vec3>, std::vector<IndexTriangle>>;

VertInd tesselateIter(int tesselationLevel);