#include "Shader.h"

#include <iostream>
#include <string>

Shader::Shader(Type type)
    : id(glCreateShader(static_cast<GLenum>(type)))
{
}

Shader::~Shader()
{
    glDeleteShader(id);
}

void Shader::Compile(const std::string& code) const
{
    const char* codePtr = code.c_str();
    glShaderSource(id, 1, &codePtr, nullptr);
    glCompileShader(id);
    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        const int bufferSize = 1024;
        GLchar infoLog[bufferSize];
        glGetShaderInfoLog(id, bufferSize, nullptr, infoLog);
        throw std::invalid_argument(infoLog);
    }
}

unsigned Shader::GetId() const
{
    return id;
}
