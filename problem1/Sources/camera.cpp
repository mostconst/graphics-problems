#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::GetViewMatrix()
{
    const glm::vec3 towards = glm::cross(right, up);
    const glm::mat4 res(
        glm::vec4(right[0], up[0], towards[0], 0.0f),
        glm::vec4(right[1], up[1], towards[1], 0.0f),
        glm::vec4(right[2], up[2], towards[2], 0.0f),
        glm::vec4(0.0f, 0.0f, -distance, 1.0f)
    );
    return res;
}

void Camera::RotateHorizontal(const float angle)
{
    glm::mat4 rotation(1.0f);
    rotation = glm::rotate(rotation, angle, right);
    up = glm::vec3(rotation * glm::vec4(up, 0.0f));
}

void Camera::RotateVertical(const float angle)
{
    glm::mat4 rotation(1.0f);
    rotation = glm::rotate(rotation, angle, up);
    right = glm::vec3(rotation * glm::vec4(right, 0.0f));
}