#include "pch.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Engine/Core/Base.h"
#include "Engine/Core/Application.h"

namespace engine
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
        m_data.Title = properties.Title;
        m_data.Width = properties.Width;
        m_data.Height = properties.Height;

        LOG_ENGINE_INFO("Creating Windows window using SDL: [{0} {1}x{2}]", properties.Title, properties.Width, properties.Height);

        // windows creation
        if (!s_SDLInitialized)
        {
            int success = SDL_Init(SDL_INIT_VIDEO);
            ENGINE_ASSERT_MSG((success == 0), "Failed to initialize SDL {0}", SDL_GetError());

            s_SDLInitialized = true;
        }

        m_window = SDL_CreateWindow(m_data.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_data.Width, m_data.Height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        ENGINE_ASSERT_MSG(m_window, "Failed to create SDL Window: {0}", SDL_GetError());

        // -1 means use whatever available card
        // SDL_RENDERER_ACCELERATED tells the system to use gpu if possible
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
        ENGINE_ASSERT_MSG(m_renderer, "Failed to create SDL Rendere: {0}", SDL_GetError());

        SetVSync(true);
    }

    void Windowswindow::Shutdown()
    {
        SDL_DestroyWindow(m_window);
    }
    
    // work around function
    int FunctionCallback(void* dat, SDL_Event* e)
    {
        engine::Windowswindow::WindowData *data = static_cast<engine::Windowswindow::WindowData*>(dat);
        switch (e->type)
        {
            case SDL_QUIT:
                Application::Get().Close();
                break;
            case SDL_WINDOWEVENT:
            {
                switch (e->window.event)
                {
                    //Windows resize event
                    case SDL_WINDOWEVENT_RESIZED:
                    {
                        data->Width = e->window.data1;
                        data->Height = e->window.data2;

                        WindowResizeEvent resizeEvent(data->Width, data->Height);
                        data->EventCallback(resizeEvent);
                        return 1;
                        break;
                    }
                    default:
                        break;
                }

                break;
            }
            default:
                break;
        }

        // consume away the event
        return 0;
    }

    void Windowswindow::OnUpdate()
    {
        //Clear color and clear color buffer bit equivalent
        SDL_SetRenderDrawColor(m_renderer, 96, 128, 255, 255);
        SDL_RenderClear(m_renderer);
        
        SDL_AddEventWatch(FunctionCallback, (void*)&m_data);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            /*switch (event.type)
            {
            case SDL_QUIT:
                Application::Get().Close();
                break;

            default:
                break;
            }*/
        }

        // render : must be called
        SDL_RenderPresent(m_renderer);
    }

    void Windowswindow::SetVSync(bool enabled)
    {
        LOG_ENGINE_INFO("Set Vsync : {0}", enabled);
        enabled 
            ? SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1") 
            : SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
        m_data.VSync = enabled;
    }

    bool Windowswindow::IsVSync() const
    {
        return m_data.VSync;
    }
}