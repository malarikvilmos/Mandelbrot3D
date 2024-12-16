#version 430 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform sampler2D imgSampler;
uniform mat4 mvp;

out vec2 texCoords;

void main()
{
	float height = texture(imgSampler, aTexCoords).a;

	texCoords = aTexCoords;
	gl_Position = mvp * vec4(aPos, height, 1.0);
}

