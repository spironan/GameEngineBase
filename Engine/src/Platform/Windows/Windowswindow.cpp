#include "pch.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Engine/Core/Base.h"
#include "Engine/Core/Application.h"

namespace Engine
{
    static bool s_SDLInitialized = false;

    Windowswindow::Windowswindow(const WindowProperties& props)
    {
        Init(props);
    }

    Windowswindow::~Windowswindow()
    {
        Shutdown();
    }

    void Windowswindow::Init(const WindowProperties& properties)
    {
        m_Data.Title = properties.Title;
        m_Data.Width = properties.Width;
        m_Data.Height = properties.Height;

        LOG_ENGINE_INFO("Creating Windows window using SDL: [{0} {1}x{2}]", properties.Title, properties.Width, properties.Height);

        // windows creation
        if (!s_SDLInitialized)
        {
            int success = SDL_Init(SDL_INIT_VIDEO);
            ENGINE_ASSERT_MSG((success == 0), "Failed to initialize SDL {0}", SDL_GetError());
            
            s_SDLInitialized = true;
        }

        m_Window = SDL_CreateWindow(m_Data.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Data.Width, m_Data.Height, SDL_WINDOW_SHOWN);
        ENGINE_ASSERT_MSG(m_Window, "Failed to create SDL Window: {0}", SDL_GetError());

        // -1 means use whatever available card
        // SDL_RENDERER_ACCELERATED tells the system to use gpu if possible
        m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
        ENGINE_ASSERT_MSG(m_Renderer, "Failed to create SDL Rendere: {0}", SDL_GetError());

        SetVSync(true);
    }

    void Windowswindow::Shutdown()
    {
        SDL_DestroyWindow(m_Window);
    }

    void Windowswindow::OnUpdate()
    {
        //Clear color and clear color buffer bit equivalent
        SDL_SetRenderDrawColor(m_Renderer, 96, 128, 255, 255);
        SDL_RenderClear(m_Renderer);

        //SDL_AddEventWatch();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                Application::Get().Close();
                break;

            default:
                break;
            }
        }

        // render : must be called
        SDL_RenderPresent(m_Renderer);
    }

    void Windowswindow::SetVSync(bool enabled)
    {
        LOG_ENGINE_INFO("Set Vsync : {0}", enabled);
        enabled 
            ? SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1") 
            : SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
        m_Data.VSync = enabled;
    }

    bool Windowswindow::IsVSync() const
    {
        return m_Data.VSync;
    }
}