#pragma once
#include "ArrayBuffer.h"
#include "glad/glad.h"

class VertexArray final
{
public:
    VertexArray();
    ~VertexArray();
    VertexArray(const VertexArray& other) = delete;
    VertexArray(VertexArray&& other) noexcept = default;
    VertexArray& operator=(const VertexArray& other) = delete;
    VertexArray& operator=(VertexArray&& other) noexcept = default;

    void vertexAttribPointer(const ArrayBuffer& arrayBuffer,
                             GLuint index,
                             GLint size,
                             GLenum type,
                             GLboolean normalized,
                             GLsizei stride,
                             const void* pointer);
    void enableVertexAttribArray(GLuint index);
    void drawElements(GLenum mode,
                      GLsizei count,
                      GLenum type,
                      const void* indices);
    void Bind() const { glBindVertexArray(handle.GetRaw()); }

    unsigned int GetRaw() const { return handle.GetRaw(); }

private:
    MovableHandle handle;
};

void loadElementBuffer(const VertexArray& vertexArray, const ElementBuffer& elementBuffer, GLsizeiptr size, const void* data, GLenum usage);