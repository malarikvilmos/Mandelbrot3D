#include "shader.h"
#include <iostream>
#include <fstream>
#include <glad4_3/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vshader, const char* fshader) noexcept
{
    uint32_t vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vshader, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    uint32_t fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fshader, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() noexcept
{
    glDeleteShader(ID);
}

void Shader::Bind() noexcept
{
    glUseProgram(ID);
    currentlyBoundID = ID;
}

void Shader::SetUniformMat4(glm::mat4& uniform, const char* name) noexcept
{
    if (ID != currentlyBoundID) Bind();
    int location = glGetUniformLocation(ID, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniform));
}

void Shader::SetUniformMat4Array(glm::mat4& uniform, uint32_t count, const char* name) noexcept
{
    if (ID != currentlyBoundID) Bind();
    int location = glGetUniformLocation(ID, name);
    glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(uniform));
}

void Shader::SetUniformU32(uint32_t value, const char* name) noexcept
{
    if (ID != currentlyBoundID) Bind();
    glUniform1ui(glGetUniformLocation(ID, name), value);
}

void Shader::SetUniformI32(int32_t value, const char* name) noexcept
{
    if (ID != currentlyBoundID) Bind();
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::SetUniformFloat(float value, const char* name) noexcept
{
    if (ID != currentlyBoundID) Bind();
    glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::SetUniformVec3(glm::vec3& uniform, const char* name) noexcept
{
    if (ID != currentlyBoundID) Bind();
    int location = glGetUniformLocation(ID, name);
    glUniform3fv(location, 1, glm::value_ptr(uniform));
}

void Shader::SetUniformVec3Array(glm::vec3& uniform, uint32_t count, const char* name) noexcept
{
    if (ID != currentlyBoundID) Bind();
    int location = glGetUniformLocation(ID, name);
    glUniform3fv(location, count, glm::value_ptr(uniform));
}

void ShaderLoader::Load() noexcept
{
    constexpr const char* const path = "shaders";
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        Read(entry.path());
    }
}

void ShaderLoader::Read(const std::filesystem::path& path) noexcept
{
    std::ifstream file(path.string());
    std::stringstream ss;
    ss << file.rdbuf();
    shaders[path.filename().string()] = ss.str();
}
