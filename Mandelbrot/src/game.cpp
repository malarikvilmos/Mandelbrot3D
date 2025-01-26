#include "game.h"
#include "datatypes/appstate.h"
#include <iostream>
#include "tools/benchmark.h"
#include "tools/math.h"
#include "tools/config.h"
#include <glad4_3/glad.h>
#include "rendering/gui.h"
#include "rendering/statemanager.h"
#include "datatypes/precision.h"
#include "rendering/camera.h"

Game::Game(const char* title) : Application(title)
{
    states[AppState::DebugState] = std::bind(&Game::DebugState, this);

    this->terrain = new Terrain(Config::width/2, Config::height/2);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    Camera::Init(this->window, Config::width, Config::height, 80.0f);
    Camera::Rotate(this->mouse.x, this->mouse.y);
    
}

Game::~Game()
{
    delete mandelbrotShaderFast;
    delete mandelbrotShaderPrecise;
}

void Game::DebugState()
{
    if (shouldResize) OnResize(Config::width, Config::height);

    SetHardwareCursor();
    Benchmark::MeasureFrametimeBegin();
    Benchmark::DoBenchmark();
    ProcessEvents();
    DebugStateInput();

    
    
    if (Config::precision == Precision::Precise) {
        mandelbrotShaderPrecise->Bind();
        mandelbrotShaderPrecise->SetUniformDouble(1 / zoomFactor, "scaleFactor");
        mandelbrotShaderPrecise->SetUniformDVec2(translate, "translation");
        mandelbrotShaderPrecise->Dispatch();
        mandelbrotShaderPrecise->Await();
    }
    else if (Config::precision == Precision::Fast) {
        mandelbrotShaderFast->Bind();
        mandelbrotShaderFast->SetUniformFloat(1 / (float)zoomFactor, "scaleFactor");
        glm::vec2 trans = glm::vec2((float)translate.x, (float)translate.y);
        mandelbrotShaderFast->SetUniformVec2(trans, "translation");
        mandelbrotShaderFast->Dispatch();
        mandelbrotShaderFast->Await();
    }
    else {
        constexpr const double switchingPoint = 20'000.0;
        if (zoomFactor > switchingPoint) {
            mandelbrotShaderPrecise->Bind();
            mandelbrotShaderPrecise->SetUniformDouble(1 / zoomFactor, "scaleFactor");
            mandelbrotShaderPrecise->SetUniformDVec2(translate, "translation");
            mandelbrotShaderPrecise->Dispatch();
            mandelbrotShaderPrecise->Await();
        } 
        else {
            mandelbrotShaderFast->Bind();
            mandelbrotShaderFast->SetUniformFloat(1 / (float)zoomFactor, "scaleFactor");
            glm::vec2 trans = glm::vec2((float)translate.x, (float)translate.y);
            mandelbrotShaderFast->SetUniformVec2(trans, "translation");
            mandelbrotShaderFast->Dispatch();
            mandelbrotShaderFast->Await();
        }
    }

    if (this->zooming == Zooming::Smooth) {
        perFrameZoom = (pow(1.25f, mouse.scroll) - zoomFactor) * Benchmark::deltaTime;
        zoomFactor += perFrameZoom;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (dimension == Dimension::D_3D)
    {
        glm::mat4 mvp(Camera::projection * Camera::view * terrain->model);
        Terrain::shader->Bind();
        this->terrain->shader->SetUniformMat4(mvp, "mvp");
        terrain->Draw();
        Camera::Update();
    }
    else {
        Quad::shader->Bind();
        Quad::Draw();
    }

    if (inTransition) {
        if (transitionFactor <= 0.0f) {
            dimension = (Dimension)((int32_t)dimension * -1);
            isTransitionHalfway = true;
            transitionFactor = 0.00000000000001f;
        }
        else if (transitionFactor > 1.0f) {
            inTransition = false;
            isTransitionHalfway = false;
        }
        if (isTransitionHalfway) {
            transitionFactor += Benchmark::deltaTime * 10;
        }
        else {
            transitionFactor -= Benchmark::deltaTime * 10;
        }
        Quad::shader->SetUniformFloat(transitionFactor, "transitionFactor");
        Terrain::shader->SetUniformFloat(transitionFactor, "transitionFactor");
    }
    else {
        transitionFactor = 1.0f;
    }
    
    
    GUI::Begin();
    if (StateManager::drawFrameStats)
    {
        GUI::DrawMenuBar();
        GUI::DrawFPSCounter();
    }
    GUI::End();
    
    SDL_GL_SwapWindow(window);
    
    Benchmark::MeasureFrametimeEnd();
}

void Game::DebugStateInput()
{
    if (!StateManager::minimized || mouse.heldRight)
    {
        if (this->keysHeld.F1) {
            keysHeld.F1 = false;
            StateManager::isWireframe = !StateManager::isWireframe;
            StateManager::Wireframe(StateManager::isWireframe);
        }
        if (this->keysHeld.F2) {
            keysHeld.F2 = false;
            this->Screenshot();
        }
        if (this->keysHeld.F3) {
            keysHeld.F3 = false;
            StateManager::FrameStats(StateManager::drawFrameStats = !StateManager::drawFrameStats);
        }
        if (this->keysHeld.Left || this->keysHeld.A) {
            translate.x -= Benchmark::deltaTime / zoomFactor;
        }
        if (this->keysHeld.Right || this->keysHeld.D) {
            translate.x += Benchmark::deltaTime / zoomFactor;
        }
        if (this->keysHeld.Up || this->keysHeld.W) {
            translate.y += Benchmark::deltaTime / zoomFactor;
        }
        if (this->keysHeld.Down || this->keysHeld.S) {
            translate.y -= Benchmark::deltaTime / zoomFactor;
        }
    }
}
