#pragma once

#include "glm/vec3.hpp"

namespace nsk_cg
{
enum class LightType
{
    Directional = 1,
    Positional
};

struct Light
{
    Light(const LightType type, const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient,
        const glm::vec3& diffuse, const glm::vec3& specular)
        : type(type), position(position), direction(direction),
        ambient(ambient),
        diffuse(diffuse),
        specular(specular)
    {
    }

    LightType type;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};
}