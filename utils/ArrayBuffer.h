#pragma once

#include "MovableHandle.h"
#include "glad/glad.h"

namespace nsk_cg
{
class VertexArray;
class ArrayBuffer final
{
public:
    ArrayBuffer();
    ~ArrayBuffer();
    unsigned GetRaw() const { return handle.GetRaw(); }
    void bufferData(size_t size,
        const void* data,
        GLenum usage) const;
    ArrayBuffer(const ArrayBuffer& other) = delete;
    ArrayBuffer(ArrayBuffer&& other) noexcept = default;
    ArrayBuffer& operator=(const ArrayBuffer& other) = delete;
    ArrayBuffer& operator=(ArrayBuffer&& other) noexcept = default;

    void Bind() const;

private:
    MovableHandle handle;
};

class ElementBuffer final
{
public:
    ElementBuffer();
    ~ElementBuffer();
    ElementBuffer(const ElementBuffer& other) = delete;
    ElementBuffer(ElementBuffer&& other) noexcept = default;
    ElementBuffer& operator=(const ElementBuffer& other) = delete;
    ElementBuffer& operator=(ElementBuffer&& other) noexcept = default;

    unsigned int GetRaw() const { return handle.GetRaw(); }
    void bufferData(const nsk_cg::VertexArray&, size_t size,
        const void* data,
        GLenum usage) const;

    void Bind() const;

private:
    MovableHandle handle;
};
}
