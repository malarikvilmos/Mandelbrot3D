#pragma once
#include "application.h"
#include "rendering/compute_shader.h"
#include <glm/vec2.hpp>
#include "rendering/quad.h"


class Game : public Application
{
public:
	Game(const char* title = "Mandelbrot");
	~Game();

private:
	void DebugState();
	void DebugStateInput();
};
