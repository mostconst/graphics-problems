#pragma once
#include <utility>

#include "camera.h"
#include "math_util.h"
#include "glm/fwd.hpp"

namespace nsk_cg
{
class UserContext final
{
public:
    UserContext(const glm::vec3& lookAt, float cameraDistance);

    int GetScreenWidth() const;
    int GetScreenHeight() const;
    double GetCameraSensitivity() const;
    const Camera& GetCamera() const;
    const glm::mat4& GetProjection() const;
    glm::mat4 GetViewMatrix() const;

    void OnWindowSizeChange(int width, int height);
    void OnMouseMove(double screenX, double screenY);
    void OnLeftMouseButtonAction(bool pressed);

private:
    // settings
    const int screenWidth = 800;
    const int screenHeight = 600;
    const double cameraSensitivity = 100.0; // pixels per rad
    const float fovDeg = 45.0f;
    const float zNear = 0.1f;
    const float zFar = 100.0f;

    std::pair<double, double> lastCursorPos = { 0.0, 0.0 };
    bool buttonPressed = false;

    Camera camera;
    glm::mat4 projection = math_utils::perspectiveFov(glm::radians(fovDeg),
        static_cast<float>(screenWidth) / static_cast<float>(screenHeight), zNear,
        zFar);
};
}
