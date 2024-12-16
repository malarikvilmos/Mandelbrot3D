#pragma once
#include <SDL2/SDL.h>
#include <thread>
#include "datatypes/mouse.h"
#include <unordered_map>
#include <functional>
#include "rendering/compute_shader.h"
#include "rendering/quad.h"
#include "datatypes/zooming.h"
#include "datatypes/zoom_direction.h"
#include "datatypes/color_scheme.h"
#include "rendering/terrain.h"
#include "datatypes/dimension.h"

struct KeysHeld
{
	bool W		: 1 = false;
	bool A		: 1 = false;
	bool S		: 1 = false;
	bool D		: 1 = false;
	bool B		: 1 = false;
	bool Space	: 1 = false;
	bool Ctrl	: 1 = false;
	bool Shift	: 1 = false;
	bool Alt	: 1 = false;
	bool F1		: 1 = false;
	bool F2		: 1 = false;
	bool F3		: 1 = false;
	bool F4		: 1 = false;
	bool F5		: 1 = false;
	bool Left	: 1 = false;
	bool Right	: 1 = false;
	bool Up		: 1 = false;
	bool Down	: 1 = false;
};

class Application
{
public:
	SDL_Window* window = nullptr;
	SDL_GLContext glContext = nullptr;
	SDL_DisplayMode displayMode = {};
	int32_t width, height;
	int32_t vsync = 1;
	bool isMouseGrabbed = false;
	bool isMouseJustPressed = false;
	ComputeShader* mandelbrotShaderFast = nullptr;
	ComputeShader* mandelbrotShaderPrecise = nullptr;
	double zoomFactor = 1.0f;
	double perFrameZoom;
	Zooming zooming = Zooming::Smooth;
	ZoomDirection zoomdir = ZoomDirection::Inwards;
	uint32_t gpuWarpSize;
	ColorScheme colorSheme = ColorScheme::Hue;
	Dimension dimension = Dimension::D_2D;
	bool inTransition = false;
	float transitionFactor = 1.0;
	bool isTransitionHalfway = false;
	float aspectRatio;

	uint32_t local_x, local_y;
	uint32_t current_iter = 200;
	bool shouldResize = false;

	Application(const char* title = (const char*)u8"Engine");
	~Application();

	void ConstructWindow();
	void ProcessEvents();
	void Run();
	void Quit();
	void CreateHardwareCursor();
	void SetHardwareCursor();
	void SetWindowIcon();
	void SetState(int32_t state);
	int GetState();
	void Screenshot();
	int32_t QueryWarpSize();

protected:
	const char* title;
	bool ipc = false;
	bool quit = false;
	KeysHeld keysHeld = {};
	Mouse mouse = {};
	std::unordered_map<int32_t, std::function<void()>> states;
	int32_t appstate = 0;
	SDL_Surface* cursorSurface = nullptr;
	SDL_Cursor* cursor = nullptr;
	glm::dvec2 translate = {};
	Terrain* terrain = nullptr;

	void ProcessDownInput(int32_t keycode);
	void ProcessUpInput(int32_t keycode);
	void OnResize(uint32_t width, uint32_t height);
};
