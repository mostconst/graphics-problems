#pragma once

template<typename T>
unsigned int glSize(const std::vector<T>& vector)
{
    return static_cast<unsigned int>(vector.size());
}