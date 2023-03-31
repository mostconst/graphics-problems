#pragma once
#include "GeometryObject.h"
#include "IndexTriangle.h"
#include "glm/vec3.hpp"

namespace nsk_cg
{
using IndexedGeometry = std::pair<std::vector<glm::vec3>, std::vector<nsk_cg::IndexTriangle>>;
IndexedGeometry tesselateIterative(int tesselationLevel);
}