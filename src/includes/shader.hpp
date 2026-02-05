#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader {
    private:
        // unsigned int shader;
        unsigned int program;
        unsigned int vertexShader;
        unsigned int fragmentShader;

        const std::string   ParseShader(const std::string& shaderPath);
        static unsigned int CompileShader(unsigned int type, const std::string& source);

    public:
        Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        Shader(const Shader& obj);
        Shader& operator=(const Shader& obj);
        ~Shader();

        void Use();
};