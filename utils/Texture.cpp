#include "Texture.h"

#include "glad/glad.h"

namespace nsk_cg
{
unsigned int createTexture()
{
    unsigned int res;
    glGenTextures(1, &res);
    return res;
}

Texture::Texture()
    : handle(createTexture())
{
}

Texture::~Texture()
{
    glDeleteTextures(1, &handle.GetRaw());
}
}