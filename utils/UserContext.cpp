#include "UserContext.h"

#include "glad/glad.h"

namespace nsk_cg
{
UserContext::UserContext(const glm::vec3& lookAt, const float cameraDistance)
    : camera{ cameraDistance, lookAt }
{
}

int UserContext::GetScreenWidth() const
{
    return screenWidth;
}

int UserContext::GetScreenHeight() const
{
    return screenHeight;
}

double UserContext::GetCameraSensitivity() const
{
    return cameraSensitivity;
}

const Camera& UserContext::GetCamera() const
{
    return camera;
}

const glm::mat4& UserContext::GetProjection() const
{
    return projection;
}

glm::mat4 UserContext::GetViewMatrix() const
{
    return camera.GetViewMatrix();
}

void UserContext::OnWindowSizeChange(const int width, const int height)
{
    glViewport(0, 0, width, height);
    projection = math_utils::perspectiveFov(glm::radians(fovDeg),
        static_cast<float>(width) / static_cast<float>(height), zNear,
        zFar);
}

void UserContext::OnMouseMove(double screenX, double screenY)
{
    if (buttonPressed)
    {
        camera.RotateHorizontal(static_cast<float>((screenY - lastCursorPos.second) / -cameraSensitivity));
        camera.RotateVertical(static_cast<float>((screenX - lastCursorPos.first) / -cameraSensitivity));
    }
    lastCursorPos = { screenX, screenY };
}

void UserContext::OnLeftMouseButtonAction(const bool pressed)
{
    buttonPressed = pressed;
}
}
