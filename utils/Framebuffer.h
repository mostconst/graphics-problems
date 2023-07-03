#pragma once

#include "MovableHandle.h"

namespace nsk_cg
{
class Texture;

class Framebuffer final
{
public:
    Framebuffer();
    ~Framebuffer();
    unsigned GetRaw() const { return handle.GetRaw(); }
    Framebuffer(const Framebuffer& other) = delete;
    Framebuffer(Framebuffer&& other) noexcept = default;
    Framebuffer& operator=(const Framebuffer& other) = delete;
    Framebuffer& operator=(Framebuffer&& other) noexcept = default;

    void Bind() const;
    void Attach(const Texture& textureColorBuffer, const Texture& textureDepthBuffer) const;

private:
    MovableHandle handle;
};
}