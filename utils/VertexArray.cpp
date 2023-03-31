#include "VertexArray.h"

namespace nsk_cg
{
unsigned int createArray()
{
    unsigned int res;
    glGenVertexArrays(1, &res);
    return res;
}

VertexArray::VertexArray()
    : handle(createArray())
{
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &handle.GetRaw());
}

void VertexArray::vertexAttribPointer(const ArrayBuffer& arrayBuffer, const GLuint index, const GLint size, const AttributeType type,
    const GLboolean normalized, const GLsizei stride, const size_t offset) const
{
    glBindVertexArray(handle.GetRaw());
    arrayBuffer.Bind();
    // TODO check size
    glVertexAttribPointer(index, size, static_cast<GLenum>(type), normalized, stride, reinterpret_cast<const void*>(offset));  // NOLINT(performance-no-int-to-ptr)
}

void VertexArray::enableVertexAttribArray(const GLuint index) const
{
    glBindVertexArray(handle.GetRaw());
    glEnableVertexAttribArray(index);
}

void VertexArray::drawElements(const GLenum mode, const size_t count, const GLenum type, const void* indices) const
{
    glBindVertexArray(handle.GetRaw());
    glDrawElements(mode, static_cast<GLsizei>(count), type, indices);
}
}
