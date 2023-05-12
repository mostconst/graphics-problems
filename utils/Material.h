#pragma once

namespace nsk_cg
{
struct Material
{
    Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float shininess,
        const float opacity)
        : ambient(ambient),
          diffuse(diffuse),
          specular(specular),
          shininess(shininess),
          opacity(opacity)
    {
    }

    Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float shininess)
        : Material(ambient, diffuse, specular, shininess, 1.0f)
    {
    }

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    float opacity;
};
}