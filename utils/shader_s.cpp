#include "shader_s.h"

#include <fstream>
#include <sstream>

#include "Shader.h"

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

    return { vShaderStream.str(), fShaderStream.str()};
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

void ShaderProgram::use() const
{
    glUseProgram(guard.GetRaw());
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(guard.GetRaw(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

