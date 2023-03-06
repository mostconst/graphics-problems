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
}