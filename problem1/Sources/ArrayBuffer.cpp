#include "ArrayBuffer.h"

#include "VertexArray.h"
#include "glad/glad.h"

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

void ArrayBuffer::bufferData(GLsizeiptr size, const void* data, GLenum usage) const
{
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
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

void ElementBuffer::bufferData(const VertexArray& vertexArray, GLsizeiptr size, const void* data, GLenum usage) const
{
    vertexArray.Bind();
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}

void ElementBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle.GetRaw());
}
