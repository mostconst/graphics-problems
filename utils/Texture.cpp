#include "Texture.h"

#include <cassert>

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

int textureFormatSize(const TextureFormat format)
{
	switch (format)
	{
		case TextureFormat::Color:
            return 4;
		case TextureFormat::Depth:
	        return 1;
	}
    assert(false);

    return 0;
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


std::vector<unsigned char> Texture::GetData() const
{
    glBindTexture(GL_TEXTURE_2D, GetRaw());
    const int nComponents = textureFormatSize(m_type);
    std::vector<unsigned char> buffer(static_cast<size_t>(imagePixelCount(m_size, nComponents)), 0);
    glGetTexImage(GL_TEXTURE_2D, 0, static_cast<int>(m_type), GL_UNSIGNED_BYTE, buffer.data());
    return buffer;
}
}
