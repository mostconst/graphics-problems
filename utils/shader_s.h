#pragma once

#include <glm/glm.hpp>

#include <string>

#include "ShaderProgramGuard.h"

// HW_ITEM 1
class ShaderProgram
{
public:
    ShaderProgram() = default;
    ShaderProgram(const std::string& vertexCode, const std::string& fragmentCode);

    void Use() const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;
    void SetFloat(const std::string& name, float value) const;

private:
    ShaderProgramGuard guard;
};

ShaderProgram createShader(const char* vertexPath, const char* fragmentPath);