#pragma once
#include "helper_enums.h"

namespace utils
{
    struct Vertex
    {
        Vertex(float x, float y, float z)
            : x(x), y(y), z(z)
        {}

        const float x;
        const float y;
        const float z;

        static constexpr size_t nComponents = 3;
        static constexpr AttribType componentType = AttribType::Float;
    };

    inline Vertex middle(const Vertex& v1, const Vertex& v2)
    {
        return { (v1.x + v2.x) / 2.0f, (v1.y + v2.y) / 2.0f, (v1.z + v2.z) / 2.0f };
    }
}