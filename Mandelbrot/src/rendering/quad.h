#pragma once
#include "glad4_3/glad.h"
#include "shader.h"
#include "../tools/asset.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <vector>
#include "../datatypes/vertex.h"

class Quad
{
public:
	inline static Shader* shader = nullptr;
	inline static glm::mat4 ortho_projection = glm::ortho(0.5f, -0.5f, -0.5f, 0.5f);

	static void Init();
	static void Draw();
	static void CleanUp();

private:
	inline static uint32_t VBO, VAO;
	inline static Vertex vertices[4]{
		Vertex { 0.5f,  0.5f, 1.0f, 1.0f},
		Vertex { 0.5f, -0.5f, 1.0f, 0.0f},
		Vertex {-0.5f, -0.5f, 0.0f, 0.0f},
		Vertex {-0.5f,  0.5f, 0.0f, 1.0f},
	};
	inline static uint32_t indices[6]{
		0, 1, 3,
		1, 2, 3
	};
};
