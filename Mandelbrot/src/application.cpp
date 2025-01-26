#include "application.h"
#include <iostream>
// #include <SDL2/SDL_syswm.h>
#include <SDL2/SDL.h>
#include "tools/benchmark.h"
#include "tools/config.h"
#include "tools/math.h"
#include <glad4_3/glad.h>
#include "rendering/gui.h"
#include "rendering/quad.h"
#include "rendering/statemanager.h"
#include "tools/asset.h"

Application::Application(const char* title)
{
    Config::Read();
    Config::Save();

    AssetManager::Load("assets.dat");

    this->title = title;
    this->width = Config::width;
    this->height = Config::height;
    this->vsync = Config::vsync;

    SDL_Init(SDL_INIT_EVENTS);

    ConstructWindow();

    CreateHardwareCursor();
    SetWindowIcon();

    SDL_SetRelativeMouseMode(SDL_FALSE);
    isMouseGrabbed = false;

    gpuWarpSize = QueryWarpSize();
    switch(gpuWarpSize) {
        case  8: local_x = 4; local_y = 2; break;
        case 16: local_x = 4; local_y = 4; break;
        case 32: local_x = 8; local_y = 4; break;
        case 64: local_x = 8; local_y = 8; break;

        default: local_x = 8; local_y = 4; break;
    }

    OnResize(Config::width, Config::height);
    this->mouse.x = 90.0f / Config::sensitivity;
}

Application::~Application()
{
    Config::Save();

    if (Terrain::shader != nullptr) delete Terrain::shader;
    GUI::Shutdown();
    Quad::CleanUp();
    SDL_FreeCursor(cursor);
    this->Quit();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Application::Run()
{
    if (states.empty()) {
        std::cerr << "No gamestates defined. Exiting.\n";
        return;
    }
    while (!quit) states[(int32_t)appstate]();
}

void Application::ConstructWindow()
{
    if (glContext != nullptr) {
        GUI::Shutdown();
        Quad::CleanUp();
        SDL_GL_DeleteContext(glContext);
        glContext = nullptr;
        SDL_DestroyWindow(window);
        window = nullptr;
        Config::Save();
    }

    uint32_t wflags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;

    StateManager::Init();
    window = SDL_CreateWindow((const char*)title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        Config::width, Config::height, wflags);

    // SDL_SysWMinfo wmInfo;
    // SDL_VERSION(&wmInfo.version);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    glContext = SDL_GL_CreateContext(window);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, this->vsync);
    SDL_GL_SetSwapInterval(this->vsync);

    SDL_GetDesktopDisplayMode(0, &displayMode);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    if (Config::fullscreen) {
        SDL_SetWindowSize(window, displayMode.w, displayMode.h);
        SDL_SetWindowFullscreen(window, Config::fullscreen);
        SDL_SetWindowPosition(window, displayMode.w / 4, displayMode.h / 4);
    }

    StateManager::Multisampling(false);
    GUI::Init(this);
    Quad::Init();
    StateManager::Wireframe(StateManager::isWireframe);
    StateManager::DepthTest(true);
    StateManager::FaceCulling(false);
    StateManager::Blending(false);
}

void Application::ProcessEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        GUI::ProcessSdlEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            std::cout << "I should quit." << std::endl;
            this->Quit();
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                OnResize(event.window.data1, event.window.data2);
            }
            break;
        case SDL_KEYDOWN:
            ProcessDownInput(event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            ProcessUpInput(event.key.keysym.sym);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (!isMouseGrabbed) {
                std::cout << "Mouse: " << event.button.x << ":" << event.button.y
                    << " | Type: " << (int32_t)event.button.button << std::endl;
            }
            if ((int32_t)event.button.button == 1) isMouseJustPressed = true;

            if (event.button.button == 1) mouse.heldLeft = true;
            if (event.button.button == 2) mouse.heldMiddle = true;
            if (event.button.button == 3) mouse.heldRight = true;

            std::cout << mouse.heldLeft << " | " << mouse.heldMiddle << " | " << mouse.heldRight << "\n";
            break;
        case SDL_MOUSEBUTTONUP:
            isMouseJustPressed = false;
            if (event.button.button == 1) mouse.heldLeft = false;
            if (event.button.button == 2) mouse.heldMiddle = false;
            if (event.button.button == 3) mouse.heldRight = false;
            break;
        case SDL_MOUSEMOTION:
            if (isMouseGrabbed) {
                mouse.x += event.motion.xrel;
                mouse.y -= event.motion.yrel;
                if (mouse.y > 300) mouse.y = 300;
                else if (mouse.y < -300) mouse.y = -300;
            }
            // else {
            //     mouse.x = event.motion.x;
            //     mouse.y = event.motion.y;
            // }
            break;
        case SDL_MOUSEWHEEL:
            constexpr const double MAX_ZOOM = 6196147063758.0;
            constexpr const uint32_t MAX_SCROLL = 131;
            static int32_t prevScroll = mouse.scroll;
            prevScroll = mouse.scroll;
            mouse.scroll += event.wheel.y;
            zoomdir = mouse.scroll > prevScroll ? ZoomDirection::Inwards : ZoomDirection::Outwards;
            if (mouse.scroll <= 0) mouse.scroll = 0;
            if (this->zooming == Zooming::Rough) {
                zoomFactor = std::min(pow(1.25f, mouse.scroll), MAX_ZOOM);
            }
            if (zoomFactor >= MAX_ZOOM) mouse.scroll -= event.wheel.y;
            if (mouse.scroll > MAX_SCROLL) mouse.scroll = MAX_SCROLL;
            break;
        }
    }
}

void Application::ProcessDownInput(int32_t keycode) {
    switch (keycode)
    {
    case SDLK_F11:
        if (!Config::fullscreen) {
            Config::width = displayMode.w;
            Config::height = displayMode.h;
            SDL_SetWindowSize(window, displayMode.w, displayMode.h);
        }
        else {
            Config::width = this->width;
            Config::height = this->height;
            SDL_SetWindowSize(window, width, height);
        }
        OnResize(Config::width, Config::height);
        SDL_SetWindowFullscreen(window, Config::fullscreen = (int32_t)!(Config::fullscreen));
        break;
    case SDLK_ESCAPE:
        Quit();
        break;
    case SDLK_w:
        keysHeld.W = true;
        break;
    case SDLK_s:
        keysHeld.S = true;
        break;
    case SDLK_a:
        keysHeld.A = true;
        break;
    case SDLK_d:
        keysHeld.D = true;
        break;
    case SDLK_b:
        keysHeld.B = true;
        break;
    case SDLK_SPACE:
        keysHeld.Space = true;
        break;
    case SDLK_LCTRL:
        keysHeld.Ctrl = true;
        break;
    case SDLK_LSHIFT:
        keysHeld.Shift = true;
        break;
    case SDLK_F1:
        keysHeld.F1 = true;
        break;
    case SDLK_F2:
        keysHeld.F2 = true;
        break;
    case SDLK_F3:
        keysHeld.F3 = true;
        break;
    case SDLK_F4:
        keysHeld.F4 = true;
        break;
    case SDLK_F5:
        keysHeld.F5 = true;
        break;
    case SDLK_LEFT:
        keysHeld.Left = true;
        break;
    case SDLK_RIGHT:
        keysHeld.Right = true;
        break;
    case SDLK_UP:
        keysHeld.Up = true;
        break;
    case SDLK_DOWN:
        keysHeld.Down = true;
        break;
    }
}

void Application::ProcessUpInput(int32_t keycode) {
    switch (keycode)
    {
    case SDLK_LALT:
        keysHeld.Alt = false;
        break;
    case SDLK_w:
        keysHeld.W = false;
        break;
    case SDLK_s:
        keysHeld.S = false;
        break;
    case SDLK_a:
        keysHeld.A = false;
        break;
    case SDLK_d:
        keysHeld.D = false;
        break;
    case SDLK_b:
        keysHeld.B = false;
        break;
    case SDLK_SPACE:
        keysHeld.Space = false;
        break;
    case SDLK_LCTRL:
        keysHeld.Ctrl = false;
        break;
    case SDLK_LSHIFT:
        keysHeld.Shift = false;
        break;
    case SDLK_F1:
        keysHeld.F1 = false;
        break;
    case SDLK_F2:
        keysHeld.F2 = false;
        break;
    case SDLK_F3:
        keysHeld.F3 = false;
        break;
    case SDLK_F4:
        keysHeld.F4 = false;
        break;
    case SDLK_F5:
        keysHeld.F5 = false;
        break;
    case SDLK_LEFT:
        keysHeld.Left = false;
        break;
    case SDLK_RIGHT:
        keysHeld.Right = false;
        break;
    case SDLK_UP:
        keysHeld.Up = false;
        break;
    case SDLK_DOWN:
        keysHeld.Down = false;
        break;
    }
}

void Application::OnResize(uint32_t width, uint32_t height)
{
    shouldResize = false;
    delete mandelbrotShaderFast;
    delete mandelbrotShaderPrecise;

    Config::width = width;
    Config::height = height;
    glViewport(0, 0, Config::width, Config::height);
    GUI::OnResize();
    if (terrain != nullptr) delete terrain;
    this->terrain = new Terrain(Config::width / 2, Config::height / 2);

    std::stringstream ssFast;
    ssFast << "assets/shaders/mandelbrot_fast_" << GetColorName(colorSheme) << '_' << gpuWarpSize << ".comp";
    std::cout << ssFast.str() << std::endl;
    Asset& file0 = AssetManager::files[ssFast.str().c_str()];
    char* mbShaderF = (char*)malloc(file0.end - file0.begin);
    if (mbShaderF != 0) strcpy(mbShaderF, (const char*)file0.content);
    mandelbrotShaderFast = new ComputeShader(mbShaderF, local_x, local_y);
    free(mbShaderF);

    float aspectRatio = (float)Config::width / Config::height;
    mandelbrotShaderFast->Bind();
    glm::vec2 invResF = glm::vec2(1.0 / Config::height * -2, 1.0 / Config::height * 2);
    mandelbrotShaderFast->SetUniformVec2(invResF, "invRes");
    mandelbrotShaderFast->SetUniformFloat(aspectRatio, "aspectRatio");
    mandelbrotShaderFast->SetUniformU32(current_iter, "maxIter");

    std::stringstream ssPrecise;
    ssPrecise << "assets/shaders/mandelbrot_precise_" << GetColorName(colorSheme) << '_' << gpuWarpSize << ".comp";
    std::cout << ssPrecise.str() << std::endl;
    Asset& file1 = AssetManager::files[ssPrecise.str().c_str()];
    char* mbShaderP = (char*)malloc(file1.end - file1.begin);
    if (mbShaderP != 0) strcpy(mbShaderP, (const char*)file1.content);
    mandelbrotShaderPrecise = new ComputeShader(mbShaderP, local_x, local_y);
    free(mbShaderP);
    this->aspectRatio = aspectRatio;

    mandelbrotShaderPrecise->Bind();
    glm::dvec2 invResP = glm::dvec2(1.0 / Config::height * -2, 1.0 / Config::height * 2);
    mandelbrotShaderPrecise->SetUniformDVec2(invResP, "invRes");
    mandelbrotShaderPrecise->SetUniformFloat(aspectRatio, "aspectRatio");
    mandelbrotShaderPrecise->SetUniformU32(current_iter, "maxIter");

}

void Application::Quit()
{
    quit = true;
}

void Application::CreateHardwareCursor()
{
    const Asset& file = AssetManager::files["assets/img/cursor.bmp"];
    SDL_RWops* rwops = SDL_RWFromMem(file.content, file.end - file.begin);
    cursorSurface = SDL_LoadBMP_RW(rwops, 1);
    cursor = SDL_CreateColorCursor(cursorSurface, 0, 0);
    SDL_FreeSurface(cursorSurface);
}

void Application::SetHardwareCursor()
{
    SDL_SetCursor(cursor);
}

void Application::SetWindowIcon()
{
    const Asset& file = AssetManager::files["assets/img/icon.bmp"];
    SDL_RWops* rwops = SDL_RWFromMem(file.content, file.end - file.begin);
    SDL_Surface* iconSurface = SDL_LoadBMP_RW(rwops, 1);
    SDL_SetWindowIcon(window, iconSurface);
    SDL_FreeSurface(iconSurface);
}

void Application::SetState(int32_t state)
{
    this->appstate = state;
}

int32_t Application::GetState()
{
    return this->appstate;
}

void Application::Screenshot()
{
    uint8_t* arr = new uint8_t[Config::width * Config::height * 4];
    glReadPixels(0, 0, Config::width, Config::height, GL_RGBA, GL_UNSIGNED_BYTE, arr);

    std::thread ioThread([arr]() {
        Image image(Config::width, Config::height, 4);
        memcpy(image.data, arr, Config::width * Config::height * 4 * sizeof(uint8_t));
        delete arr;

        time_t timestamp = time(&timestamp);
        struct tm dttm = *localtime(&timestamp);

        std::stringstream ss;
        ss << "saved/";
        ss << dttm.tm_year + 1900;  ss << "-";
        ss << dttm.tm_mon  + 1;     ss << "-";
        ss << dttm.tm_mday;         ss << "-";
        ss << dttm.tm_hour;         ss << "-";
        ss << dttm.tm_min;          ss << "-";
        ss << dttm.tm_sec;
        ss << ".png";

        if (std::filesystem::exists("saved/") && std::filesystem::is_directory("saved/")) {
            image.Save(ss.str().c_str());
        }
        else {
            std::filesystem::create_directory("saved/");
            image.Save(ss.str().c_str());
        }
    });
    ioThread.detach();
}

int32_t Application::QueryWarpSize()
{
    int32_t subgroupSize = 32;
    if (SDL_GL_ExtensionSupported("GL_KHR_shader_subgroup")) {
        glGetIntegerv(GL_SUBGROUP_SIZE_KHR, &subgroupSize);
    }
    return subgroupSize;
}

