﻿#pragma once
#include "ArrayBuffer.h"
#include "helper_enums.h"
#include "glad/glad.h"

namespace nsk_cg
{
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
        AttributeType type,
        GLboolean normalized,
        GLsizei stride,
        size_t offset) const;
    void enableVertexAttribArray(GLuint index) const;
    void drawElements(GLenum mode,
        size_t count,
        GLenum type,
        const void* indices) const;
    void Bind() const { glBindVertexArray(handle.GetRaw()); }

    unsigned int GetRaw() const { return handle.GetRaw(); }

private:
    MovableHandle handle;
};
}