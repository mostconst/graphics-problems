#include "Vertex.h"

#include "glm/geometric.hpp"
#include "comparison.h"

namespace nsk_cg
{
float distance(const Vertex& v1, const nsk_cg::Vertex& v2)
{
    return glm::length(v1.Subtract(v2));
}

bool equal(const Vertex& v1, const Vertex& v2)
{
    return distance(v1, v2) < TOLERANCE;
}
}
