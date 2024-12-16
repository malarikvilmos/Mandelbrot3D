#pragma once
#include <cstdint>
#include <glm/vec2.hpp>

class ComputeShader
{
public:
	uint32_t local_size_x, local_size_y;
	uint32_t ID;
	uint32_t texture0;
	uint32_t texture_width, texture_height;

	ComputeShader(const char* shader, uint32_t local_size_x, uint32_t local_size_y) noexcept;
	~ComputeShader() noexcept;
	void Bind() noexcept;
	void Dispatch() noexcept;
	void Await() noexcept;

	void SetUniformFloat(float value, const char* name) noexcept;
	void SetUniformDouble(double value, const char* name) noexcept;
	void SetUniformVec2(glm::vec2& uniform, const char* name) noexcept;
	void SetUniformDVec2(glm::dvec2& uniform, const char* name) noexcept;
	void SetUniformU32(uint32_t value, const char* name) noexcept;
};
