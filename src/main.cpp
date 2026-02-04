#include "includes/object.hpp"
#include <GLFW/glfw3.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_obj_file>" << std::endl;
        return 1;
    };

    GLFWwindow* window;

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    };

    window = glfwCreateWindow(500, 500, "Scop", NULL, NULL);
    glfwMakeContextCurrent(window);

    Object obj(argv[1]);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        glfwPollEvents();
    };

    glfwTerminate();
    return (0);
};
