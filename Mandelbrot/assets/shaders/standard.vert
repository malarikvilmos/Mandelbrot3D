#version 430 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 mvp;

out vec2 texCoords;

void main()
{
	texCoords = aTexCoords;
	gl_Position = mvp * vec4(aPos, 0.0, 1.0);
}

