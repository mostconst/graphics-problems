#pragma once
#include "Vertex.h"

namespace nsk_cg
{
class VertexTriangle
{
public:
    VertexTriangle(const Vertex& first, const Vertex& second, const Vertex& third);
    Vertex First() const;
    Vertex Second() const;
    Vertex Third() const;

private:
    Vertex first;
    Vertex second;
    Vertex third;
};

bool isCorrect(const VertexTriangle& triangle);
glm::vec3 getNormalVector(const VertexTriangle& triangle);
}
