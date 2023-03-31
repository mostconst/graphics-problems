#include "ArrayBuffer.h"

#include "VertexArray.h"
#include "glad/glad.h"

namespace nsk_cg
{
unsigned int createBuffer()
{
    unsigned int res;
    glGenBuffers(1, &res);
    return res;
}

ArrayBuffer::ArrayBuffer()
    : handle(createBuffer())
{
}

ArrayBuffer::~ArrayBuffer()
{
    glDeleteBuffers(1, &handle.GetRaw());
}

void ArrayBuffer::bufferData(const size_t size, const void* data, const GLenum usage) const
{
    Bind();
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, usage);
}

void ArrayBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, handle.GetRaw());
}

ElementBuffer::ElementBuffer()
    : handle(createBuffer())
{
}

ElementBuffer::~ElementBuffer()
{
    glDeleteBuffers(1, &handle.GetRaw());
}

void ElementBuffer::bufferData(const nsk_cg::VertexArray& vertexArray, const size_t size, const void* data, const GLenum usage) const
{
    vertexArray.Bind();
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, usage);
}

void ElementBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle.GetRaw());
}
}
