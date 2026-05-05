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
#include "includes/GUI.hpp"
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
    static_assert(sizeof(vec3) == 3 * sizeof(float));
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 

    unsigned int positionsBytes = obj.getPositions().size() * (3 * sizeof(float));
    unsigned int normalsBytes = obj.getNormals().size() * (3 * sizeof(float));
    unsigned int UVsBytes = obj.getUVs().size() * (2 * sizeof(float));
    VertexArray va;
    VertexBuffer vb((positionsBytes + normalsBytes + UVsBytes));
    vb.AddSubData(&obj.getPositions()[0], positionsBytes, 0);
    vb.AddSubData(&obj.getNormals()[0], normalsBytes, positionsBytes);
    vb.AddSubData(&obj.getUVs()[0], UVsBytes, positionsBytes + normalsBytes);
    vb.Bind();

    VertexBufferLayout layout;
    layout.Push<float>(3, 0);
    layout.Push<float>(3, positionsBytes);
    layout.Push<float>(2, (positionsBytes + normalsBytes));
    va.AddBuffer(vb, layout);
    va.Bind();

    IndexBuffer ib(obj.getIndices().data(), obj.getIndices().size());
    Shader shader("shaders/shader");
    shader.Bind();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    Texture texture("bl_tx225_co.png"); // default texture
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    Camera camera;
    const vec3 cameraTarget = obj.getCenter();
    std::cout << "Center of Gravity:" << cameraTarget.v1 << " " << cameraTarget.v2 << " " << cameraTarget.v3 << std::endl;
    camera.setTarget(cameraTarget.v1, cameraTarget.v2, cameraTarget.v3);
    camera.setRadius(5.0);
    camera.setAspect((float)width / (float)height);
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
    glLineWidth(2.0f);
    glPointSize(2.5f);

    GUI gui(window, texture, modelMatrix, vb, obj);
    float *lightRGB = gui.getLightRGB();
    float *lightAmbientStrength = gui.getLightAmbientStrength();

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
            invMv3x3[0] = 1.0f; invMv3x3[1] = 0.0f; invMv3x3[2] = 0.0f;
            invMv3x3[3] = 0.0f; invMv3x3[4] = 1.0f; invMv3x3[5] = 0.0f;
            invMv3x3[6] = 0.0f; invMv3x3[7] = 0.0f; invMv3x3[8] = 1.0f;
        }

        float normalMatrix[9];
        mat3Transpose(invMv3x3, normalMatrix);
        vb.Bind();
        va.Bind();
        shader.Bind();
        texture.Bind(); 

        shader.SetUniformMatrix4fv("view", viewF);
        shader.SetUniformMatrix4fv("projection", camera.getProjectionMatrix());
        shader.SetUniformMatrix4fv("model", modelMatrix);
        shader.SetUniformMatrix3fv("normalMatrix", normalMatrix);

        shader.SetUniform1i("u_Texture", 0);
        shader.SetUniform3f("u_LightDirection", 0.0f, -1.0f, 0.0f);
        shader.SetUniform3f("u_LightColour", lightRGB[0], lightRGB[1], lightRGB[2]);
        shader.SetUniform3f("u_AmbientStrength", lightAmbientStrength[0], lightAmbientStrength[1], lightAmbientStrength[2]);

        renderer.Draw(va, ib, shader);
        gui.RenderWindow();

        glfwSwapBuffers(window);

        glfwPollEvents();
    };

    glfwTerminate();
    return (0);
};
