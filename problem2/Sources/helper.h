#pragma once
#include <vector>

#include "VertexArray.h"

namespace utils
{
    struct Vertex;
}

class GeometryObject;
VertexArray LoadBuffers(const GeometryObject& object, const std::vector<utils::Vertex>& morphedVertices, std::vector<ArrayBuffer>& arrayBuffers,
                        std::vector<ElementBuffer>& elementBuffers);

GeometryObject makePyramid(int tesselationLevel);
