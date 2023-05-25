#pragma once
#include "MovableHandle.h"
#include "glad/glad.h"

enum class TextureFormat
{
    Color = GL_RGBA,
    Depth = GL_DEPTH_COMPONENT
};

namespace nsk_cg
{
struct ScreenSize;

class Texture final
{
public:
    Texture(const TextureFormat& type, int width, int height);
    ~Texture();
    Texture(const Texture& other) = delete;
    Texture(Texture&& other) noexcept = default;
    Texture& operator=(const Texture& other) = delete;
    Texture& operator=(Texture&& other) noexcept = default;

    unsigned GetRaw() const { return handle.GetRaw(); }
    void SetSize(const ScreenSize& size) const;

private:
    MovableHandle handle;
    const TextureFormat m_type;
};
}
