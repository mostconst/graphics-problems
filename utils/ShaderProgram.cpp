﻿#include "ShaderProgram.h"

#include <fstream>
#include <sstream>

#include "Shader.h"

namespace nsk_cg
{
ShaderProgram createShader(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::ifstream vShaderFile(vertexPath);
    std::ifstream fShaderFile(fragmentPath);
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    /*vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);*/
    std::stringstream vShaderStream, fShaderStream;
    // read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    return { vShaderStream.str(), fShaderStream.str() };
}

ShaderProgram::ShaderProgram(const std::string& vertexCode, const std::string& fragmentCode)
{
    const Shader vertex(Shader::Type::Vertex);
    vertex.Compile(vertexCode);
    const Shader fragment(Shader::Type::Fragment);
    fragment.Compile(fragmentCode);
    guard = ShaderProgramGuard(glCreateProgram());
    glAttachShader(guard.GetRaw(), vertex.GetId());
    glAttachShader(guard.GetRaw(), fragment.GetId());
    glLinkProgram(guard.GetRaw());
    GLint success;
    glGetProgramiv(guard.GetRaw(), GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1024];
        glGetProgramInfoLog(guard.GetRaw(), 1024, nullptr, infoLog);
        throw std::invalid_argument(infoLog);
    }
}

void ShaderProgram::Use() const
{
    glUseProgram(guard.GetRaw());
}

void ShaderProgram::SetMat4(const std::string& name, const glm::mat4& mat) const
{
    const GLint location = glGetUniformLocation(guard.GetRaw(), name.c_str());
    assert(location != -1);
    glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::SetFloat(const std::string& name, const float value) const
{
    const GLint location = glGetUniformLocation(guard.GetRaw(), name.c_str());
    assert(location != -1);
    glUniform1f(location, value);
}

void ShaderProgram::SetVec3(const std::string& name, const glm::vec3& vector) const
{
    const GLint location = glGetUniformLocation(guard.GetRaw(), name.c_str());
    assert(location != -1);
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void ShaderProgram::SetVec3(const int location, const glm::vec3& vector)
{
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void ShaderProgram::SetInt(const int location, const int value)
{
    glUniform1i(location, value);
}

void ShaderProgram::SetMat3(const std::string& name, const glm::mat3& matrix) const
{
    const GLint location = glGetUniformLocation(guard.GetRaw(), name.c_str());
    assert(location != -1);
    glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void ShaderProgram::SetInt(const std::string& name, const int value) const
{
    const GLint location = glGetUniformLocation(guard.GetRaw(), name.c_str());
    assert(location != -1);
    glUniform1i(location, value);
}
}
