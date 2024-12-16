#include "camera.h"
#include <glad4_3/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "../tools/benchmark.h"
#include "../tools/config.h"
#include "statemanager.h"

void Camera::Init(SDL_Window* window, uint32_t width, uint32_t height, float fov) noexcept
{
    Camera::window = window;
    Camera::width = width;
    Camera::height = height;
    Camera::fov = fov;
    projection = glm::perspective(glm::radians(fov), (float)width / height, nearPlane, farPlane);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::Update() noexcept
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::ZoomIn() noexcept
{
    projection = glm::perspective(glm::radians(fov * 0.55f), width / height, nearPlane, farPlane);
}

void Camera::ZoomOut() noexcept
{
    projection = glm::perspective(glm::radians(fov), width / height, nearPlane, farPlane);
}

void Camera::Rotate(int32_t relX, int32_t relY) noexcept
{
    static float sensitivity = Config::sensitivity;
    relX *= sensitivity;
    relY *= sensitivity;

    yaw = relX;
    pitch = relY;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void Camera::MoveFreeLook(Direction direction, bool isShiftHeld) noexcept
{
    float speed = cameraSpeed * Benchmark::deltaTime;
    speed += isShiftHeld * 2 * speed;

    switch (direction) {
        case Direction::FORWARD:      cameraPos += cameraFront * speed; break;
        case Direction::BACKWARDS:    cameraPos -= cameraFront * speed; break;
        case Direction::LEFT:         cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed; break;
        case Direction::RIGHT:        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed; break;
        case Direction::UP:           cameraPos += cameraUp * speed; break;
        case Direction::DOWN:         cameraPos -= cameraUp * speed; break;
    }
}

void Camera::EnableCursor() noexcept
{
    SDL_SetRelativeMouseMode(SDL_FALSE);
    if (setToMiddle) SDL_WarpMouseInWindow(window, width / 2, height / 2);
    firstMouse = true;
    setToMiddle = false;
}

void Camera::DisableCursor() noexcept
{
    SDL_SetRelativeMouseMode(SDL_TRUE);
    setToMiddle = true;
}

void Camera::ToggleCursor() noexcept
{
    mouseIsLocked ? EnableCursor() : DisableCursor();
}
