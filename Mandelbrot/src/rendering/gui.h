#pragma once
#include "../application.h"
#include "../rendering/image.h"
#include <imgui/imgui.h>

enum class GuiState
{
	None,
	Debug
};

class GUI
{
public:
	inline static Application* app = nullptr;
	inline static Image* image = nullptr;
	inline static uint32_t fontSize = 1;
	inline static ImFontConfig fontConfig = {};
	inline static void* font1 = nullptr;
	inline static void* font2 = nullptr;
	inline static float menuBarHeight = 0;

	static void Init(Application* app);
	static void Shutdown();
	static void ProcessSdlEvent(SDL_Event* event);
	static void Begin();
	static void End();
	static void DrawFPSCounter();
	static void OnResize();
	static void DrawEditorGUI();
	static void DrawMenuBar();

private:
	inline static GuiState state = GuiState::None;
};
