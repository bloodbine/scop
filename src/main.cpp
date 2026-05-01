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
#include "includes/utils.hpp"
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

    unsigned int vertexCount = obj.getVertexes().size();
    unsigned int vertexNormalsCount = obj.getVertexNormals().size();
    unsigned int vertexTexCoordsCount = obj.getVertexTexCoords().size();
    VertexArray va;
    VertexBuffer vb((vertexCount + vertexNormalsCount + vertexTexCoordsCount) * sizeof(float));
    vb.AddSubData(&obj.getVertexes()[0], vertexCount * sizeof(float), 0);
    vb.AddSubData(&obj.getVertexNormals()[0], vertexNormalsCount * sizeof(float), vertexCount * sizeof(float));
    vb.AddSubData(&obj.getVertexTexCoords()[0], vertexTexCoordsCount * sizeof(float), (vertexCount + vertexNormalsCount) * sizeof(float));
    vb.Bind();

    VertexBufferLayout layout;
    layout.Push<float>(3, 0);
    layout.Push<float>(3, vertexCount * sizeof(float));
    layout.Push<float>(2, (vertexCount + vertexNormalsCount) * sizeof(float));
    va.AddBuffer(vb, layout);
    va.Bind();

    IndexBuffer ib(&obj.getFaces()[0], obj.getFaces().size());
    Shader shader("shaders/shader");
    shader.Bind();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    Texture texture("bl_tx225_co.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

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

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    Renderer renderer;
    while (!glfwWindowShouldClose(window)) {
        renderer.Clear();
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.processKeyboard(window, deltaTime);

        const double* viewD = camera.getViewMatrix();
        float viewF[16];
        for (int i = 0; i < 16; ++i) viewF[i] = static_cast<float>(viewD[i]);

        float mv[16];
        mat4Mul(viewF, modelMatrix, mv);

        float mv3x3[9];
        mat4ToMat3(mv, mv3x3);

        float invMv3x3[9];
        if (!mat3Inverse(mv3x3, invMv3x3)) {
            for (int i = 0; i < 0; ++i) invMv3x3[i] = (i % 4 == 0) ? 1.0f : 0.0f;
        }

        float normalMatrix[9];
        mat3Transpose(invMv3x3, normalMatrix);

        shader.Bind();

        shader.SetUniformMatrix4fv("view", viewF);
        shader.SetUniformMatrix4fv("projection", camera.getProjectionMatrix());
        shader.SetUniformMatrix4fv("model", modelMatrix);
        shader.SetUniformMatrix4fv("normalMatrix", normalMatrix);

        shader.SetUniform3f("u_LightDirection", 0.0f, -1.0f, 0.0f);
        shader.SetUniform3f("u_LightColour", 1.0f, 1.0f, 1.0f);
        shader.SetUniform3f("u_AmbientStrength", 0.2f, 0.2f, 0.2f);

        renderer.Draw(va, ib, shader);

        glfwSwapBuffers(window);

        glfwPollEvents();
    };

    glfwTerminate();
    return (0);
};
