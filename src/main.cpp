#include "renderer/renderManager.h"
#include <GLFW/glfw3.h>

int main(int argc, char** argv)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(1024, 768, "OpenGL Clustered", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    glewInit();

    RenderManager rm;
    rm.startUp();

    while (!glfwWindowShouldClose(window))
    {
        rm.Render();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}