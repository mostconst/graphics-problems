#pragma once
#include "GeometryObject.h"
#include "glm/vec3.hpp"

using VertInd = std::pair<std::vector<glm::vec3>, std::vector<unsigned int>>;

GeometryObject makePyramid(int tesselationLevel);
VertInd tesselateIter(const VertInd& input, int tesselationLevel);