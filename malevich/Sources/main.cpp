#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils.h"

int main()
{
    constexpr int windowWidth = 800;
    MalevichCore app(windowWidth);
    //app.SetBackground(Color{ 0.0f, 1.0f, 0.0f });

    while (!glfwWindowShouldClose(app.GetWindow()))
    {
        app.Draw();

        glfwSwapBuffers(app.GetWindow());
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
