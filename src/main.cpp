#include "includes/Renderer.hpp"
#include "includes/object.hpp"
#include "includes/VertexBuffer.hpp"
#include "includes/VertexBufferLayout.hpp"
#include "includes/VertexArray.hpp"
#include "includes/IndexBuffer.hpp"
#include "includes/Shader.hpp"
#include "includes/Texture.hpp"
#include "includes/stb_image.h"
#include "includes/Camera.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>


int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_obj_file>" << std::endl;
        return 1;
    };

    GLFWwindow* window;
    
    if (glfwInit() != GLFW_TRUE) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    };

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1920, 1080, "Scop", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    };

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    Object obj(argv[1]);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    VertexArray va;
    VertexBuffer vb(&obj.getVertexes()[0], obj.getVertexes().size() * sizeof(float));
    vb.Bind();
    VertexBufferLayout layout;
    layout.Push<float>(3);
    va.AddBuffer(vb, layout);
    va.Bind();

    IndexBuffer ib(&obj.getFaces()[0], obj.getFaces().size());

    Shader shader("shaders/shader");
    shader.Bind();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    float colorTimer = 0.0f;
    const float colorInterval = 0.5f; // seconds
    float currentColor[4];
    currentColor[0] = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    currentColor[1] = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    currentColor[2] = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    currentColor[3] = 1.0f;
    shader.SetUniform4f("u_Colour", currentColor[0], currentColor[1], currentColor[2], currentColor[3]);

    // Texture texture("42.png");
    // texture.Bind();
    // shader.SetUniform1i("u_Texture", 0);

    // va.UnBind();
    // vb.UnBind();
    // ib.UnBind();
    // shader.UnBind();

    Camera camera;
    const std::array<float, 3> cameraTarget = obj.getCenter();
    std::cout << "Center of Gravity:" << cameraTarget[0] << " " << cameraTarget[1] << " " << cameraTarget[2] << std::endl;
    camera.setTarget(cameraTarget[0], cameraTarget[1], cameraTarget[2]);
    camera.setRadius(5.0);
    camera.setAspect(800.0f / 600.0f);
    camera.setFOV(45.0f);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    float modelMatrix[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    Renderer renderer;
    while (!glfwWindowShouldClose(window)) {
        renderer.Clear();
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.processKeyboard(window, deltaTime);

        colorTimer += deltaTime;
        if (colorTimer >= colorInterval) {
            currentColor[0] = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
            currentColor[1] = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
            currentColor[2] = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
            shader.SetUniform4f("u_Colour", currentColor[0], currentColor[1], currentColor[2], currentColor[3]);
            colorTimer = 0.0f;
        }
        const double* viewD = camera.getViewMatrix();
        float viewF[16];
        for (int i = 0; i < 16; ++i) viewF[i] = static_cast<float>(viewD[i]);
        shader.SetUnformMatrix4fv("view", viewF);
        shader.SetUnformMatrix4fv("projection", camera.getProjectionMatrix());
        shader.SetUnformMatrix4fv("model", modelMatrix);

        renderer.Draw(va, ib, shader);

        glfwSwapBuffers(window);

        glfwPollEvents();
    };

    glfwTerminate();
    return (0);
};
