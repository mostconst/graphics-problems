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

int textureFormatSize(const GLint format)
{
	switch (format)
	{
		case GL_RGBA:
            return 4;
		case GL_DEPTH_COMPONENT:
	        return 1;
        default:
            assert(false);
            return 0;
	}
}

Texture::Texture(const GLint internalFormat, const GLenum format, const int width, const int height, const void* data)
    : handle(createTexture()),
      m_internalFormat(internalFormat),
      m_format(format),
      m_size{ width, height }
{
    glBindTexture(GL_TEXTURE_2D, GetRaw());
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const TextureFormat& type, const int width, const int height)
    : Texture{static_cast<GLint>(type), static_cast<GLenum>(type) , width, height, nullptr}
{
}

Texture::~Texture()
{
    glDeleteTextures(1, &handle.GetRaw());
}

void Texture::SetSize(const ScreenSize& size)
{
    m_size = size;
    glBindTexture(GL_TEXTURE_2D, GetRaw());
    glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, size.GetWidth(), size.GetHeight(), 0, m_format, GL_UNSIGNED_BYTE, nullptr);
}


void Texture::SetWrap(GLint param)
{
    glBindTexture(GL_TEXTURE_2D, GetRaw());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
}

std::vector<unsigned char> Texture::GetData() const
{
    glBindTexture(GL_TEXTURE_2D, GetRaw());
    const int nComponents = textureFormatSize(m_internalFormat);
    std::vector<unsigned char> buffer(static_cast<size_t>(m_size.GetWidth() * m_size.GetHeight() * nComponents), 0);
    glGetTexImage(GL_TEXTURE_2D, 0, static_cast<int>(m_internalFormat), GL_UNSIGNED_BYTE, buffer.data());
    return buffer;
}
}
