#pragma once
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <memory>
#include <cstdint>
#include <chrono>
#include <thread>

#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer.h"
#include "imgui.h"

#include <yadl/context.hpp>
#include <yadl/canvas.hpp>
#include <yadl/debug.hpp>

class SDLCore
{
private:
    constexpr static int32_t m_width{1280};
    constexpr static int32_t m_height{720};

    int32_t m_canvasWidth{640};
    int32_t m_canvasHeight{480};

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window{nullptr, SDL_DestroyWindow};
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_renderer{nullptr, SDL_DestroyRenderer};
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_texture{nullptr, SDL_DestroyTexture};

    void Run()
    {
        bool isRunning = true;
        SDL_Event event;

        double deltaTime = 0.0;
        std::chrono::high_resolution_clock::time_point lastFrame = std::chrono::high_resolution_clock::now();
        
        OnStart();

        while (isRunning)
        {
            std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
            deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastFrame).count() / 1000000.0;
            lastFrame = now;

            while (SDL_PollEvent(&event))
            {
                ImGui_ImplSDL2_ProcessEvent(&event);
                if (event.type == SDL_QUIT)
                {
                    isRunning = false;
                }
            }

            SDL_RenderClear(m_renderer.get());

            ImGui_ImplSDLRenderer_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
            OnImGuiRender(deltaTime);
            ImGui::Render();

            OnUpdate(deltaTime);

            OnRender(deltaTime);
            SDL_RenderSetLogicalSize(m_renderer.get(), m_canvasWidth, m_canvasHeight);
            SDL_UpdateTexture(m_texture.get(), nullptr, m_canvas.GetBytes(), m_canvas.GetWidth() * sizeof(uint32_t));
            SDL_RenderCopy(m_renderer.get(), m_texture.get(), nullptr, nullptr);
            SDL_RenderSetLogicalSize(m_renderer.get(), m_width, m_height);

            ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

            SDL_RenderPresent(m_renderer.get());

        }
    }

protected:
    yadl::Canvas m_canvas;
    yadl::Context m_ctx{m_canvas};

public:
    SDLCore(int32_t canvasWidth = 640, int32_t canvasHeight = 480) 
    : m_canvasWidth{canvasWidth}
    , m_canvasHeight{canvasHeight}
    , m_canvas{canvasWidth, canvasHeight}
    , m_ctx{m_canvas}
    {
        YADL_LOG_FATAL_IF(SDL_Init(SDL_INIT_EVERYTHING) != 0, "SDL_Init Error: %s", SDL_GetError());

        m_window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(SDL_CreateWindow("YADL Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE), SDL_DestroyWindow);
        YADL_LOG_FATAL_IF(m_window == nullptr, "SDL_CreateWindow Error: %s", SDL_GetError());

        m_renderer = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>(SDL_CreateRenderer(m_window.get(), -1, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer);
        YADL_LOG_FATAL_IF(m_renderer == nullptr, "SDL_CreateRenderer Error: %s", SDL_GetError());

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
        SDL_RenderSetLogicalSize(m_renderer.get(), m_canvasWidth, m_canvasHeight);

        m_texture = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(SDL_CreateTexture(m_renderer.get(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, m_canvasWidth, m_canvasHeight), SDL_DestroyTexture);

        SDL_SetRenderDrawColor(m_renderer.get(), 0, 0, 0, 255);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io{ImGui::GetIO()};

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui_ImplSDL2_InitForSDLRenderer(m_window.get(), m_renderer.get());
        ImGui_ImplSDLRenderer_Init(m_renderer.get());

    }
    ~SDLCore()
    {
        ImGui_ImplSDLRenderer_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_Quit();
    }
    SDLCore(const SDLCore&) = delete;
    SDLCore(SDLCore&&) = delete;
    SDLCore& operator=(const SDLCore&) = delete;
    SDLCore& operator=(SDLCore&&) = delete;

    void Start()
    {
        Run();
    }

    virtual void OnUpdate(double deltaTime) 
    {
        (void) deltaTime;
    };
    virtual void OnRender(double deltaTime) 
    {
        (void) deltaTime;
    };
    virtual void OnImGuiRender(double deltaTime) 
    {
        (void) deltaTime;
    };
    virtual void OnStart() {};
};
