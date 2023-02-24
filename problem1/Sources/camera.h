#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    explicit Camera(const float distance)
        : distance(distance)
    {}

    glm::mat4 GetViewMatrix();
    void RotateHorizontal(const float angle);
    void RotateVertical(const float angle);

private:
    const float distance;
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
};