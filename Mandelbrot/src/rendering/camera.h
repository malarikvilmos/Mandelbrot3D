#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <SDL2/SDL.h>

enum class CameraState
{
	FreeLook
};

enum class Direction 
{ 
	FORWARD, 
	BACKWARDS, 
	LEFT, 
	RIGHT, 
	UP, 
	DOWN 
};

class Camera
{
public:
	inline static SDL_Window* window = nullptr;
	inline static CameraState state = CameraState::FreeLook;
	inline static glm::mat4 view = glm::mat4(1.0f);
	inline static glm::mat4 projection = glm::mat4(1.0f);
	inline static float fov = 90.0f;
	
	inline static float yaw = -90.0f;
	inline static float pitch = 0.0f;

	static void Init(SDL_Window* window, uint32_t width, uint32_t height, float fov) noexcept;
	static void Update() noexcept;
	static void ZoomIn() noexcept;
	static void ZoomOut() noexcept;
	static void Rotate(int32_t relX, int32_t relY) noexcept;
	static void MoveFreeLook(Direction direction, bool isShiftHeld) noexcept;
	static void EnableCursor() noexcept;
	static void DisableCursor() noexcept;
	static void ToggleCursor() noexcept;

private:
	inline static float nearPlane = 0.001f;
	inline static float farPlane = 500.0f;
	inline static float accumulatedX = 0.0f;
	inline static float accumulatedY = 0.0f;
	inline static float cameraSpeed = 3.0f;
	inline static float width = 0;
	inline static float height = 0;
	inline static float lastX = width / 2;
	inline static float lastY = height / 2;
	inline static float sensitivity = 0.5f;
	inline static bool firstMouse = true;
	inline static bool setToMiddle = true;
	inline static bool mouseIsLocked = true;
	inline static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -3.0f);
	inline static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	inline static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

};
