#pragma once
#include "glad/glad.h"

class MovableHandle
{
public:
    MovableHandle(unsigned int handle);
    ~MovableHandle() = default;
    const unsigned int& GetRaw() const;
    MovableHandle(const MovableHandle& other) = delete;
    MovableHandle(MovableHandle&& other) noexcept;
    MovableHandle& operator=(const MovableHandle& other) = delete;
    MovableHandle& operator=(MovableHandle&& other) noexcept;

private:
    unsigned int handle;
};
