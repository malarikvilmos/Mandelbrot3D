#include "gui.h"
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>
#include "../tools/benchmark.h"
#include <glad4_3/glad.h>
#include "statemanager.h"
#include "../tools/asset.h"
#include "../tools/math.h"
#include <ctime>
#include "../datatypes/precision.h"
#include "camera.h"

void GUI::Init(Application* app)
{
    GUI::fontConfig.FontDataOwnedByAtlas = false;
    fontSize = Config::height / 30;
    GUI::app = app;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    const Asset& file = AssetManager::files["assets/fonts/unispace.ttf"];
    font1 = io.Fonts->AddFontFromMemoryTTF(file.content, file.end - file.begin, GUI::fontSize, &GUI::fontConfig);
    font2 = io.Fonts->AddFontFromMemoryTTF(file.content, file.end - file.begin, GUI::fontSize * 0.65f, &GUI::fontConfig);
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(GUI::app->window, app->glContext);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::SetMouseCursor(ImGuiMouseCursor_None);
}

void GUI::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void GUI::ProcessSdlEvent(SDL_Event* event)
{
    ImGui_ImplSDL2_ProcessEvent(event);
}

void GUI::Begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void GUI::End()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::EndFrame();
}

void GUI::DrawFPSCounter()
{
    if (StateManager::minimized) return;
    ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight));
    ImGui::SetNextWindowSize(ImVec2(Config::width / 2, Config::height / 2));
    ImGui::Begin("Frame stats", NULL,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_AlwaysAutoResize);

    ImVec2 shadowOffset(0.0f, 3.0f);
    ImVec4 shadowColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImGui::PushFont((ImFont*)font1);

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::SetCursorScreenPos(ImVec2(pos.x + shadowOffset.x, pos.y + shadowOffset.y));
    ImGui::TextColored(shadowColor, "%d FPS", Benchmark::framerate);
    ImGui::SetCursorScreenPos(pos);
    ImGui::TextColored(textColor, "%d FPS", Benchmark::framerate);

    pos = ImGui::GetCursorScreenPos();
    ImGui::SetCursorScreenPos(ImVec2(pos.x + shadowOffset.x, pos.y + shadowOffset.y));
    ImGui::TextColored(shadowColor, "Frametime: %.3f ms", Benchmark::frametime);
    ImGui::SetCursorScreenPos(pos);
    ImGui::TextColored(textColor, "Frametime: %.3f ms", Benchmark::frametime);

    pos = ImGui::GetCursorScreenPos();
    ImGui::SetCursorScreenPos(ImVec2(pos.x + shadowOffset.x, pos.y + shadowOffset.y));
    ImGui::TextColored(shadowColor, "Hide UI: F3", Benchmark::frametime);
    ImGui::SetCursorScreenPos(pos);
    ImGui::TextColored(textColor, "Hide UI: F3", Benchmark::frametime);

    pos = ImGui::GetCursorScreenPos();
    ImGui::SetCursorScreenPos(ImVec2(pos.x + shadowOffset.x, pos.y + shadowOffset.y));
    ImGui::TextColored(shadowColor, "Screenshot: F2", app->zoomFactor);
    ImGui::SetCursorScreenPos(pos);
    ImGui::TextColored(textColor, "Screenshot: F2", app->zoomFactor);

    pos = ImGui::GetCursorScreenPos();
    ImGui::SetCursorScreenPos(ImVec2(pos.x + shadowOffset.x, pos.y + shadowOffset.y));
    ImGui::TextColored(shadowColor, "Fullscreen: F11", app->zoomFactor);
    ImGui::SetCursorScreenPos(pos);
    ImGui::TextColored(textColor, "Fullscreen: F11", app->zoomFactor);

    ImGui::PopFont();

    ImGui::End();
}

void GUI::OnResize()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    GUI::fontSize = Config::height / 30;
    
    const Asset& file = AssetManager::files["assets/fonts/unispace.ttf"];
    font1 = io.Fonts->AddFontFromMemoryTTF(file.content, file.end - file.begin, GUI::fontSize, &GUI::fontConfig);

    GUI::Shutdown();
    GUI::Init(GUI::app);
}

void GUI::DrawEditorGUI()
{
    ImGui::PushFont((ImFont*)font2);
    DrawMenuBar();
    ImGui::PopFont();
}

void GUI::DrawMenuBar()
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.30f, 0.30f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.50f, 0.50f, 0.50f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.4f, 0.4f, 0.4f, 0.55f));

    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.5f, 0.5f, 0.5f, 0.05f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.5f, 0.5f, 0.5f, 0.05f));
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.4f, 0.4f, 0.05f));
    ImGui::PushStyleColor(ImGuiCol_SeparatorHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));

    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.4f, 0.4f, 0.4f, 0.05f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 0.05f));

    ImGui::PushStyleColor(ImGuiCol_ResizeGripHovered, ImVec4(0.7f, 0.7f, 0.7f, 0.1f));
    ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, ImVec4(0.7f, 0.7f, 0.7f, 0.1f));
    ImGui::PushStyleColor(ImGuiCol_ResizeGrip, ImVec4(0.5f, 0.5f, 0.5f, 0.1f));

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save Image"))
            {
                app->Screenshot();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit"))
            {
                app->Quit();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Precision"))
        {
            if (ImGui::MenuItem("Automatic"))
            {
                Config::precision = Precision::Automatic;
                Config::Save();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Fast"))
            {
                Config::precision = Precision::Fast;
                Config::Save();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Precise"))
            {
                Config::precision = Precision::Precise;
                Config::Save();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Iterations"))
        {
            if (ImGui::MenuItem("200")) {
                app->current_iter = 200;
                app->mandelbrotShaderFast->Bind();
                app->mandelbrotShaderFast->SetUniformU32(app->current_iter, "maxIter");
                app->mandelbrotShaderPrecise->Bind();
                app->mandelbrotShaderPrecise->SetUniformU32(app->current_iter, "maxIter");
            } ImGui::Separator();
            if (ImGui::MenuItem("500")) {
                app->current_iter = 500;
                app->mandelbrotShaderFast->Bind();
                app->mandelbrotShaderFast->SetUniformU32(app->current_iter, "maxIter");
                app->mandelbrotShaderPrecise->Bind();
                app->mandelbrotShaderPrecise->SetUniformU32(app->current_iter, "maxIter");
            } ImGui::Separator();
            if (ImGui::MenuItem("1000")) {
                app->current_iter = 1000;
                app->mandelbrotShaderFast->Bind();
                app->mandelbrotShaderFast->SetUniformU32(app->current_iter, "maxIter");
                app->mandelbrotShaderPrecise->Bind();
                app->mandelbrotShaderPrecise->SetUniformU32(app->current_iter, "maxIter");
            } ImGui::Separator();
            if (ImGui::MenuItem("2000")) {
                app->current_iter = 2000;
                app->mandelbrotShaderFast->Bind();
                app->mandelbrotShaderFast->SetUniformU32(app->current_iter, "maxIter");
                app->mandelbrotShaderPrecise->Bind();
                app->mandelbrotShaderPrecise->SetUniformU32(app->current_iter, "maxIter");
            } ImGui::Separator();
            if (ImGui::MenuItem("5000")) {
                app->current_iter = 5000;
                app->mandelbrotShaderFast->Bind();
                app->mandelbrotShaderFast->SetUniformU32(app->current_iter, "maxIter");
                app->mandelbrotShaderPrecise->Bind();
                app->mandelbrotShaderPrecise->SetUniformU32(app->current_iter, "maxIter");
            } ImGui::Separator();
            if (ImGui::MenuItem("10000")) {
                app->current_iter = 10000;
                app->mandelbrotShaderFast->Bind();
                app->mandelbrotShaderFast->SetUniformU32(app->current_iter, "maxIter");
                app->mandelbrotShaderPrecise->Bind();
                app->mandelbrotShaderPrecise->SetUniformU32(app->current_iter, "maxIter");
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Vsync"))
        {
            if (ImGui::MenuItem("On"))
            {
                Config::vsync = 1;
                Config::Save();
                SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, Config::vsync);
                SDL_GL_SetSwapInterval(Config::vsync);
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Off"))
            {
                Config::vsync = 0;
                Config::Save();
                SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, Config::vsync);
                SDL_GL_SetSwapInterval(Config::vsync);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Theme"))
        {
            if (ImGui::MenuItem("Hue"))
            {
                app->colorSheme = ColorScheme::Hue;
                app->shouldResize = true;
            } ImGui::Separator();
            if (ImGui::MenuItem("Grayscale"))
            {
                app->colorSheme = ColorScheme::Grayscale;
                app->shouldResize = true;
            } ImGui::Separator();
            if (ImGui::MenuItem("Scary"))
            {
                app->colorSheme = ColorScheme::Scary;
                app->shouldResize = true;
            } ImGui::Separator();
            if (ImGui::MenuItem("Fourth"))
            {
                app->colorSheme = ColorScheme::Fourth;
                app->shouldResize = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Zooming"))
        {
            if (ImGui::MenuItem("Rough"))
            {
                app->zooming = Zooming::Rough;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Smooth"))
            {
                app->zooming = Zooming::Smooth;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Dimensions"))
        {
            if (ImGui::MenuItem("2D"))
            {
                if (app->dimension == Dimension::D_3D) {
                    app->inTransition = true;
                    app->isTransitionHalfway = false;
                }
            } ImGui::Separator();
            if (ImGui::MenuItem("3D"))
            {
                if (app->dimension == Dimension::D_2D) {
                    app->inTransition = true;
                    app->isTransitionHalfway = false;
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    ImGui::PopStyleColor(16);
    GUI::menuBarHeight = ImGui::GetFrameHeight();
}
