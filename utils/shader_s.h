#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ShaderProgram
{
public:
    ShaderProgram() = default;
    ShaderProgram(const std::string& vertexCode, const std::string& fragmentCode);
    ~ShaderProgram();
    ShaderProgram(const ShaderProgram& other) = delete;
    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(const ShaderProgram& other) = delete;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    void use() const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    unsigned int id = 0;
};

ShaderProgram createShader(const char* vertexPath, const char* fragmentPath);