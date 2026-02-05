#include "includes/shader.hpp"

const std::string Shader::ParseShader(const std::string& shaderPath) {
    std::ifstream shaderFile(shaderPath);
    std::string shader;
    if (shaderFile.is_open()) {
        std::string line;
        while (std::getline(shaderFile, line))
            shader.append(line + '\n');
    } else {
        std::cerr << "Unable to open shader file: " << shaderPath << std::endl;
        return (NULL);
    };
    return (shader);
};

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cerr << "Failed to compile shader: " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cerr << message << std::endl;
        glDeleteShader(id);
        return (0);
    };

    return (id);
};

void Shader::Use() {
    glUseProgram(this->program);
};

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    this->program = glCreateProgram();
    this->vertexShader = this->CompileShader(GL_VERTEX_SHADER, this->ParseShader(vertexShaderPath));
    this->fragmentShader = this->CompileShader(GL_FRAGMENT_SHADER, this->ParseShader(fragmentShaderPath));

    glAttachShader(program, this->vertexShader);
    glAttachShader(program, this->fragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDetachShader(program, this->vertexShader);
    glDetachShader(program, this->fragmentShader);
};

Shader::Shader(const Shader& obj) {
    *this = obj;
};

Shader& Shader::operator=(const Shader& obj) {
    this->program = obj.program;
    this->vertexShader = obj.vertexShader;
    this->fragmentShader = obj.fragmentShader;

    return *this;
};

Shader::~Shader() {
    glDeleteProgram(this->program);
};
