#pragma once

#include <vector>

#include "MovableHandle.h"
#include "ScreenSize.h"
#include "glad/glad.h"

namespace nsk_cg
{
enum class TextureFormat {
    Color = GL_RGBA,
    Depth = GL_DEPTH_COMPONENT
};

int textureFormatSize(TextureFormat format);

class Texture final
{
public:
    Texture(GLint internalFormat, GLenum format, int width, int height, const void* data);
    Texture(const TextureFormat& type, int width, int height);
    ~Texture();
    Texture(const Texture& other) = delete;
    Texture(Texture&& other) noexcept = default;
    Texture& operator=(const Texture& other) = delete;
    Texture& operator=(Texture&& other) noexcept = default;

    unsigned GetRaw() const { return handle.GetRaw(); }
    void SetSize(const ScreenSize& size);
    const ScreenSize& GetSize() const { return m_size; }
    void SetWrap(GLint param);

    std::vector<unsigned char> GetData() const;

private:
    MovableHandle handle;
    GLint m_internalFormat;
    GLenum m_format;
    ScreenSize m_size;
};
}
