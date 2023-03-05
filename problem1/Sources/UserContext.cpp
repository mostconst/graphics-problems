#include "UserContext.h"

#include "glad/glad.h"

void UserContext::OnWindowSizeChange(int width, int height)
{
    glViewport(0, 0, width, height);
    projection = glm::perspective(glm::radians(fovDeg),
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

void UserContext::OnLeftMouseButtonAction(bool pressed)
{
    buttonPressed = pressed;
}
