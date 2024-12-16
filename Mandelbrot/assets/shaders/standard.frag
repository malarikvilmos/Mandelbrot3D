#version 430 core

in vec2 texCoords;
uniform sampler2D tex;
uniform float transitionFactor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(texture(tex, texCoords).rgb * transitionFactor, 1.0);
}

