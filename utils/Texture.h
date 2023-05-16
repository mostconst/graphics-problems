#pragma once
#include "MovableHandle.h"

namespace nsk_cg
{
class Texture final
{
public:
    Texture();
    ~Texture();
    unsigned GetRaw() const { return handle.GetRaw(); }
    Texture(const Texture& other) = delete;
    Texture(Texture&& other) noexcept = default;
    Texture& operator=(const Texture& other) = delete;
    Texture& operator=(Texture&& other) noexcept = default;

    void Bind() const;

private:
    MovableHandle handle;
};
}
