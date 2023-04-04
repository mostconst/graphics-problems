#pragma once

namespace nsk_cg
{
constexpr float TOLERANCE = 1E-4f;

inline bool lessThan(const float lhs, const float rhs)
{
    return rhs - lhs > TOLERANCE;
}

inline bool floatsEqual(const float lhs, const float rhs)
{
    return lessThan(lhs, rhs) == false && lessThan(rhs, lhs) == false;  // NOLINT(readability-suspicious-call-argument)
}

inline bool vectorsEqual(const glm::vec3& lhs, const glm::vec3& rhs)
{
    return floatsEqual(lhs.x, rhs.x) &&
        floatsEqual(lhs.y, rhs.y) &&
        floatsEqual(lhs.z, rhs.z);
}
}