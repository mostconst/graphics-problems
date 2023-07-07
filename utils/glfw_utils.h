#pragma once
#include <string_view>

struct GLFWwindow;

namespace nsk_cg
{
class ScreenSize;
class UserContext;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double screenX, double screenY);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
GLFWwindow* makeWindow(nsk_cg::UserContext& userContext, const std::string_view& title);
GLFWwindow* makeMinimalWindow(const ScreenSize& userContext, const std::string_view& title);
}