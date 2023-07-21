#include <fstream>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "utils.h"

int main()
{
    constexpr int windowWidth = 800;
    constexpr int windowHeight = 600;
    MalevichCore app(windowWidth, windowHeight);
    app.SetColor(0.0f, 1.0f, 0.0f);

    while (!glfwWindowShouldClose(app.GetWindow()))
    {
        app.Draw();

        glfwSwapBuffers(app.GetWindow());
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
