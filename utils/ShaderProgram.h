#pragma once

#include <glm/glm.hpp>

#include <string>

#include "ShaderProgramGuard.h"

namespace nsk_cg
{
// HW_ITEM 1
class ShaderProgram final
{
public:
    ShaderProgram() = default;
    ShaderProgram(const std::string& vertexCode, const std::string& fragmentCode);

    void Use() const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec3(const std::string& name, const glm::vec3& vector) const;
    static void SetVec3(int location, const glm::vec3& vector);
    void SetMat3(const std::string& name, const glm::mat3& matrix) const;
    void SetInt(const std::string& name, int value) const;
    static void SetInt(int location, int value);

private:
    ShaderProgramGuard guard;
};

ShaderProgram createShader(const char* vertexPath, const char* fragmentPath);
}