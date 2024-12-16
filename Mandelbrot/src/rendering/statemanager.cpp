#include "statemanager.h"
#include <iostream>
#include <glad4_3/glad.h>
#include <SDL2/SDL_video.h>

void StateManager::Init()
{
    
}

void StateManager::Blending(bool value)
{
    if (value) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else {
        glDisable(GL_BLEND);
    }
}

void StateManager::DepthTest(bool value)
{
    glDepthFunc(GL_LESS);
    value ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void StateManager::FaceCulling(bool value)
{
    value ? glEnable(GL_CULL_FACE): glDisable(GL_CULL_FACE);
}

void StateManager::Multisampling(bool value)
{
    value ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);
}

void StateManager::MultisamplingCount(MSAA_Samples count)
{
    StateManager::msaa_count = count;
    //Config::msaa_samples = static_cast<int32_t>(count);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, (int)StateManager::msaa_count);
}


void StateManager::ClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void StateManager::Wireframe(bool value)
{
    glPolygonMode(GL_FRONT_AND_BACK, value ? GL_LINE : GL_FILL);
}

void StateManager::FrameStats(bool drawFrameStats)
{
    StateManager::drawFrameStats = drawFrameStats;
}

void StateManager::Minimize(bool value)
{
    StateManager::minimized = value;
}
