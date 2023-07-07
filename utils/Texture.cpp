#include "Texture.h"

#include "ScreenSize.h"
#include "glad/glad.h"

namespace nsk_cg
{
unsigned int createTexture()
{
    unsigned int res;
    glGenTextures(1, &res);
    return res;
}

Texture::Texture(const TextureFormat& type, const int width, const int height)
    : handle(createTexture()),
      m_type(type),
      m_size{width, height}
{
    glBindTexture(GL_TEXTURE_2D, GetRaw());
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(type), width, height, 0, static_cast<int>(type), GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &handle.GetRaw());
}

void Texture::SetSize(const ScreenSize& size)
{
    m_size = size;
    glBindTexture(GL_TEXTURE_2D, GetRaw());
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(m_type), size.GetWidth(), size.GetHeight(), 0, static_cast<int>(m_type), GL_UNSIGNED_BYTE, nullptr);
}


Image Texture::MakeImage() const
{
    glBindTexture(GL_TEXTURE_2D, GetRaw());
    const int nComponents = 4;
    std::vector<unsigned char> buffer(static_cast<size_t>(m_size.GetWidth() * m_size.GetHeight() * nComponents), 0);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
    return Image{ std::move(buffer), m_size, nComponents };
}
}
