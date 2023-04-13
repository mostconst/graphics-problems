#pragma once

#include <glm/glm.hpp>

namespace nsk_cg
{
class Camera final
{
public:
    Camera(const float distance, const glm::vec3& origin)
        : distance(distance),
        origin(origin)
    {
    }

    glm::mat4 GetViewMatrix() const;
    glm::vec3 GetViewPosition() const;
    void RotateHorizontal(float angle);
    void RotateVertical(float angle);

private:
    const float distance;
    const glm::vec3 origin;
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
};
}
