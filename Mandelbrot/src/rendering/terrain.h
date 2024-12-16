#pragma once
#include <cstdint>
#include <vector>
#include "shader.h"
#include "../datatypes/vertex.h"

class Terrain
{
public:
	inline static Shader* shader = nullptr;
	uint32_t width, height;

	Terrain(uint32_t width, uint32_t height);
	~Terrain();
	void Draw();

	uint32_t VBO, VAO;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	glm::mat4 model;
};
