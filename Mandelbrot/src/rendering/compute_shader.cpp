#include "compute_shader.h"
#include <glad4_3/glad.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "../tools/config.h"

ComputeShader::ComputeShader(const char* shader, uint32_t local_size_x, uint32_t local_size_y) noexcept
{
	texture_width = Config::width;
	texture_height = Config::height;
	this->local_size_x = local_size_x;
	this->local_size_y = local_size_y;

	uint32_t computeShader;
	computeShader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShader, 1, &shader, NULL);
	glCompileShader(computeShader);

	int success;
	char infoLog[512];
	glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(computeShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << shader << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, computeShader);
	glLinkProgram(ID);

	glDeleteShader(computeShader);

	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, texture_width, texture_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindImageTexture(0, texture0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

ComputeShader::~ComputeShader() noexcept
{
	glDeleteTextures(1, &texture0);
	glDeleteProgram(ID);
}

void ComputeShader::Bind() noexcept
{
	glUseProgram(ID);
}

void ComputeShader::Dispatch() noexcept
{
	glDispatchCompute(texture_width / local_size_x, texture_height / local_size_y, 1);
}

void ComputeShader::Await() noexcept
{
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void ComputeShader::SetUniformFloat(float value, const char* name) noexcept
{
	glUniform1f(glGetUniformLocation(ID, name), value);
}

void ComputeShader::SetUniformDouble(double value, const char* name) noexcept
{
	glUniform1d(glGetUniformLocation(ID, name), value);
}

void ComputeShader::SetUniformVec2(glm::vec2& uniform, const char* name) noexcept
{
	int location = glGetUniformLocation(ID, name);
	glUniform2fv(location, 1, glm::value_ptr(uniform));
}

void ComputeShader::SetUniformDVec2(glm::dvec2& uniform, const char* name) noexcept
{
	int location = glGetUniformLocation(ID, name);
	glUniform2dv(location, 1, glm::value_ptr(uniform));
}

void ComputeShader::SetUniformU32(uint32_t value, const char* name) noexcept
{
	glUniform1ui(glGetUniformLocation(ID, name), value);
}
