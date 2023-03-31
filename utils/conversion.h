#pragma once

namespace nsk_cg
{
template<typename T>
unsigned int glSize(const std::vector<T>& vector)
{
    // TODO size check
    return static_cast<unsigned int>(vector.size());
}

inline int sizeToInt(const size_t value)
{
    // TODO size check
    return static_cast<int>(value);
}
}