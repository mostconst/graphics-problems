#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "ShaderProgramGuard.h"

class ShaderProgram
{
public:
    ShaderProgram() = default;
    ShaderProgram(const std::string& vertexCode, const std::string& fragmentCode);

    void use() const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    ShaderProgramGuard guard;
};

ShaderProgram createShader(const char* vertexPath, const char* fragmentPath);