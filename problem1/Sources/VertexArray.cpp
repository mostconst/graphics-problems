#include "VertexArray.h"

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

void VertexArray::vertexAttribPointer(const ArrayBuffer& arrayBuffer, GLuint index, GLint size, GLenum type,
                                      GLboolean normalized, GLsizei stride, const void* pointer)
{
    glBindVertexArray(handle.GetRaw());
    arrayBuffer.Bind();
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void VertexArray::enableVertexAttribArray(GLuint index)
{
    glBindVertexArray(handle.GetRaw());
    glEnableVertexAttribArray(index);
}

void VertexArray::drawElements(GLenum mode, GLsizei count, GLenum type, const void* indices)
{
    glBindVertexArray(handle.GetRaw());
    glDrawElements(mode, count, type, indices);
}

void loadElementBuffer(const VertexArray& vertexArray, const ElementBuffer& elementBuffer, GLsizeiptr size,
                       const void* data, GLenum usage)
{
    vertexArray.Bind();
    elementBuffer.Bind();

}
