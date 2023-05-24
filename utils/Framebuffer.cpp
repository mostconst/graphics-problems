#include "Framebuffer.h"

#include <cassert>
#include <iostream>
#include <ostream>

#include "Texture.h"
#include "glad/glad.h"

namespace nsk_cg
{
unsigned int createFramebuffer()
{
    unsigned int res;
    glGenFramebuffers(1, &res);
    return res;
}

Framebuffer::Framebuffer()
    : handle(createFramebuffer())
{
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &handle.GetRaw());
}

bool checkFramebuffer()
{
    const auto framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    return framebufferStatus == GL_FRAMEBUFFER_COMPLETE;
}

void Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, GetRaw());
}

void Framebuffer::Attach(const Texture& textureColorBuffer, const Texture& textureDepthBuffer) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, GetRaw());
    // create a color attachment texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer.GetRaw(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureDepthBuffer.GetRaw(), 0);

    assert(checkFramebuffer());
}
}
