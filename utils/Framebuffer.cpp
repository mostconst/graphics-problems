#include "Framebuffer.h"

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
}