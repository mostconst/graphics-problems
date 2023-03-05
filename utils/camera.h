#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(const float distance, const glm::vec3& origin)
        : distance(distance),
          origin(origin)
    {
    }

    glm::mat4 GetViewMatrix() const;
    void RotateHorizontal(const float angle);
    void RotateVertical(const float angle);

private:
    const float distance;
    const glm::vec3 origin;
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
};
