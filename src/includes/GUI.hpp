#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "Texture.hpp"
#include <filesystem>
#include <iostream>
#include <map>
#include <vector>

#pragma once

class GUI {
    private:
        float lightRGB[3];
        float lightAmbientStrength[3];
        Texture &texture;
        std::vector<std::string> textureList;
        int textureIDx;
        float (&modelMatrix)[16];
        float scale[3];
        std::map<std::string, GLenum> renderModeList;
        std::string renderMode;

    public:
        GUI(GLFWwindow *window, Texture &texture, float (&modelMatrix)[16]);
        ~GUI();

        void RenderWindow(void);

        float *getLightRGB(void);
        float *getLightAmbientStrength(void);
};