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
      m_type(type)
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

void Texture::SetSize(const ScreenSize& size) const
{
    glBindTexture(GL_TEXTURE_2D, GetRaw());
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(m_type), size.m_width, size.m_height, 0, static_cast<int>(m_type), GL_UNSIGNED_BYTE, nullptr);
}
}
