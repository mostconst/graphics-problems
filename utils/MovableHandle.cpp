#include "MovableHandle.h"

namespace nsk_cg
{
MovableHandle::MovableHandle(const unsigned handle)
    : handle(handle)
{
}

const unsigned int& MovableHandle::GetRaw() const
{
    return handle;
}

MovableHandle::MovableHandle(MovableHandle&& other) noexcept
    : handle(other.handle)
{
    other.handle = 0;
}

MovableHandle& MovableHandle::operator=(MovableHandle&& other) noexcept
{
    if (this != &other)
    {
        handle = other.handle;
        other.handle = 0;
    }

    return *this;
}
}
