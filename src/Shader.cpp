#include "includes/Shader.hpp"

Shader::Shader(const std::string& filepath) : m_FilePath(filepath), m_RendererID(0) {
    std::string vertexSource = ParseShader(m_FilePath + ".vert");
    std::string fragmentSource = ParseShader(m_FilePath + ".frag");

    m_RendererID = CreateShader(vertexSource, fragmentSource);
};

Shader::~Shader() {
    glDeleteProgram(this->m_RendererID);
};

const std::string Shader::ParseShader(std::string filepath) {
    std::ifstream shaderFile(filepath);
    std::string shader;
    if (shaderFile.is_open()) {
        std::string line;
        while (std::getline(shaderFile, line))
            shader.append(line + '\n');
    } else {
        std::cerr << "Unable to open shader file: " << filepath << std::endl;
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

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
};

void Shader::Bind() const {
    glUseProgram(m_RendererID);
};

void Shader::UnBind() const {
    glUseProgram(0);
};

void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
};

void Shader::SetUniform1f(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    glUniform3f(GetUniformLocation(name), v0, v1, v2);
};

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
};

void Shader::SetUniformMatrix4fv(const std::string& name, const float* matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix);
}

void Shader::SetUniformMatrix3fv(const std::string& name, const float* matrix)
{
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, matrix);
};

void Shader::SetUniformMatrix4dv(const std::string& name, const double* matrix) {
    glUniformMatrix4dv(GetUniformLocation(name), 1, GL_FALSE, matrix);
};

int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    m_UniformLocationCache[name] = location;
    return location;
};