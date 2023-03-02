#include "camera.h"
#include "math_util.h"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::GetViewMatrix()
{
    const glm::vec3 towards = glm::cross(right, up);
    const auto eye = origin + towards * distance;
    const glm::mat4 res = rowMajorMatrix(
        glm::vec4(right, -glm::dot(right, eye)),
        glm::vec4(up, -glm::dot(up, eye)),
        glm::vec4(towards, -glm::dot(towards, eye)),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
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
