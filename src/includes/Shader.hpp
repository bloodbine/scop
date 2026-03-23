#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader {
    private:
        std::string m_FilePath;
        unsigned int m_RendererID;
        std::unordered_map<std::string, unsigned int> m_UniformLocationCache;

        unsigned int GetUniformLocation(const std::string& name);
        const std::string ParseShader(std::string filepath);
        unsigned int CreateShader(const std::string& vertexShader, const std::string fragmentShader);
        unsigned int CompileShader(unsigned int type, const std::string& source);
    public:
        Shader(const std::string& filepath);
        ~Shader();

        void Bind() const;
        void UnBind() const;

        // Set Uniforms
        void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
};
