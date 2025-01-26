#include "terrain.h"
#include "../tools/asset.h"
#include <glad4_3/glad.h>
#include <iostream>
#include "../tools/math.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "camera.h"
#include "../application.h"
#include "../tools/config.h"
#include <cstring>

Terrain::Terrain(uint32_t width, uint32_t height)
{
	if (shader == nullptr) {
		Asset& file = AssetManager::files["assets/shaders/terrain.vert"];
		char* vertexShader = (char*)malloc(file.end - file.begin);
		if (vertexShader != 0) strcpy(vertexShader, (const char*)file.content);

		Asset& file2 = AssetManager::files["assets/shaders/standard.frag"];
		char* fragmentShader = (char*)malloc(file2.end - file2.begin);
		if (fragmentShader != 0) strcpy(fragmentShader, (const char*)file2.content);

		Terrain::shader = new Shader(vertexShader, fragmentShader);
		Terrain::shader->Bind();

		free(vertexShader);
		free(fragmentShader);
	}

	this->width = width;
	this->height = height;
	vertices = std::vector<Vertex>(width * height);

	for (int32_t i = 0; i < width; ++i) {
		for (int32_t j = 0; j < height; ++j) {
			vertices[j * width + i].x = Math::LinearMapToRange((float)i / width, 0.0f, 1.0f, -0.5f, 0.5f);
			vertices[j * width + i].y = Math::LinearMapToRange((float)j / height, 0.0f, 1.0f, -0.5f, 0.5f);
			vertices[j * width + i].s = (float)i / width;
			vertices[j * width + i].t = (float)j / height;
		}
	}
	
	for (int32_t i = 0; i < width - 1; ++i) {
		for (int32_t j = 0; j < height - 1; ++j) {
			uint32_t topLeft = j * width + i;
			uint32_t topRight = j * width + (i + 1);
			uint32_t bottomLeft = (j + 1) * width + i;
			uint32_t bottomRight = (j + 1) * width + (i + 1);

			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
			indices.push_back(topRight);

			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(topLeft);
		}
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	shader->Bind();
	shader->SetUniformFloat(1.0f, "transitionFactor");
	model = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10 / ((float)Config::width / Config::height), 1.0f));
}

Terrain::~Terrain()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Terrain::Draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
}
