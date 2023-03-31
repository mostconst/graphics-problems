#pragma once
#include <string>

#include "glad/glad.h"

namespace nsk_cg
{
class Shader final
{
public:
    enum class Type : GLenum
    {
        Vertex = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER
    };

    Shader(Type type);
    ~Shader();
    void Compile(const std::string& code) const;
    unsigned int GetId() const;
    Shader(const Shader& other) = delete;
    Shader(Shader&& other) noexcept = delete;
    Shader& operator=(const Shader& other) = delete;
    Shader& operator=(Shader&& other) noexcept = delete;

private:
    unsigned int id;
};
}
