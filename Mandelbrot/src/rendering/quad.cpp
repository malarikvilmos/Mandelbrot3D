#include "quad.h"
#include <iostream>
#include "../tools/config.h"
#include "../tools/math.h"
#include <cstring>

void Quad::Init()
{
	Asset& file = AssetManager::files["assets/shaders/standard.vert"];
	char* vertexShader = (char*)malloc(file.end - file.begin);
	if (vertexShader != 0) strcpy(vertexShader, (const char*)file.content);
	
	Asset& file2 = AssetManager::files["assets/shaders/standard.frag"];
	char* fragmentShader = (char*)malloc(file2.end - file2.begin);
	if (fragmentShader != 0) strcpy(fragmentShader, (const char*)file2.content);

	Quad::shader = new Shader(vertexShader, fragmentShader);
	Quad::shader->Bind();

	free(vertexShader);
	free(fragmentShader);

	glGenVertexArrays(1, &Quad::VAO);
	glBindVertexArray(Quad::VAO);

	glGenBuffers(1, &Quad::VBO);
	glBindBuffer(GL_ARRAY_BUFFER, Quad::VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Quad::vertices), Quad::vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Quad::shader->SetUniformMat4(ortho_projection, "mvp");
	Quad::shader->SetUniformFloat(1.0f, "transitionFactor");
}

void Quad::Draw()
{
	glBindVertexArray(Quad::VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, Quad::indices);
}

void Quad::CleanUp()
{
	delete shader;
	glDeleteBuffers(1, &Quad::VBO);
	glDeleteVertexArrays(1, &Quad::VAO);
}
