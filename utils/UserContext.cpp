#include "UserContext.h"

#include <glad/glad.h>

#include "ScreenSize.h"

namespace nsk_cg
{
UserContext::UserContext(const glm::vec3& lookAt, const float cameraDistance)
    : m_camera{ cameraDistance, lookAt }
{
}

UserContext::UserContext(const glm::vec3& lookAt, const float cameraDistance, const ScreenSize& screenSize)
    : screenWidth{screenSize.GetWidth()}
    , screenHeight{screenSize.GetHeight()}
    , m_camera{cameraDistance, lookAt}
{
}

void UserContext::AddScreenSizeListener(IScreenSizeListener* const listener)
{
    m_screenSizeListeners.push_back(listener);
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
    return m_camera;
}

const glm::mat4& UserContext::GetProjection() const
{
    return m_projection;
}

glm::mat4 UserContext::GetViewMatrix() const
{
    return m_camera.GetViewMatrix();
}

void UserContext::OnWindowSizeChange(const int width, const int height)
{
    glViewport(0, 0, width, height);
    m_projection = math_utils::perspectiveFov(glm::radians(fovDeg),
        static_cast<float>(width) / static_cast<float>(height), zNear,
        zFar);

    for(const auto listener : m_screenSizeListeners)
    {
        listener->OnWindowSizeChange({width, height});
    }
}

void UserContext::OnMouseMove(double screenX, double screenY)
{
    if (m_buttonPressed)
    {
        m_camera.RotateHorizontal(static_cast<float>((screenY - lastCursorPos.second) / -cameraSensitivity));
        m_camera.RotateVertical(static_cast<float>((screenX - lastCursorPos.first) / -cameraSensitivity));
    }
    lastCursorPos = { screenX, screenY };
}

void UserContext::OnLeftMouseButtonAction(const bool pressed)
{
    m_buttonPressed = pressed;
}
}
