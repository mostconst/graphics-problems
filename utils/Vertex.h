#pragma once
#include "helper_enums.h"
#include "glm/vec3.hpp"

namespace nsk_cg
{
struct Vertex
{
    Vertex(const float x, const float y, const float z)
        : x(x), y(y), z(z)
    {}

    glm::vec3 ToVector() const
    {
        return { x, y, z };
    }

    // TODO operator
    Vertex AddVector(const glm::vec3& vector) const
    {
        return { x + vector.x, y + vector.y, z + vector.z };
    }

    // TODO operator
    glm::vec3 Subtract(const Vertex& vertex) const
    {
        return ToVector() - vertex.ToVector();
    }

    const float x;
    const float y;
    const float z;

    static constexpr size_t nComponents = 3;
    static constexpr AttributeType componentType = AttributeType::Float;
};

inline Vertex middle(const Vertex& v1, const Vertex& v2)
{
    return { (v1.x + v2.x) / 2.0f, (v1.y + v2.y) / 2.0f, (v1.z + v2.z) / 2.0f };
}

float distance(const Vertex& v1, const Vertex& v2);
bool equal(const Vertex& v1, const Vertex& v2);
Vertex mapToUnitSphere(const nsk_cg::Vertex& v);
}
