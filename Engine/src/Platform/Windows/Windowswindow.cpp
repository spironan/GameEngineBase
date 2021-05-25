/************************************************************************************//*!
\file           WindowsWindow.cpp
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 15, 2021
\brief          Describes a Windows(Platform) specific windows that implements 
                the generic window interface. 
                Currently using SDL as the backend abstraction.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"

#include <sdl2/SDL.h>

#include "Platform/Windows/WindowsWindow.h"
#include "Engine/Core/Base.h"
#include "Engine/Core/Application.h"

#include "Engine/Core/Input.h"

namespace engine
{
    static bool s_SDLInitialized = false;

    WindowsWindow::WindowsWindow(const WindowProperties& props)
    {
        ENGINE_PROFILE_FUNCTION();

        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        ENGINE_PROFILE_FUNCTION();

        Shutdown();
    }

    void WindowsWindow::Init(const WindowProperties& properties)
    {
        ENGINE_PROFILE_FUNCTION();

        m_data.Title = properties.Title;
        m_data.Width = properties.Width;
        m_data.Height = properties.Height;

        LOG_ENGINE_INFO("Creating Windows window using SDL: [{0} {1}x{2}]", properties.Title, properties.Width, properties.Height);

        // windows creation
        if (!s_SDLInitialized)
        {
            ENGINE_PROFILE_SCOPE("SDL_INIT");

            int success = SDL_Init(SDL_INIT_VIDEO);
            ENGINE_ASSERT_MSG((success == 0), "Failed to initialize SDL {0}", SDL_GetError());

            s_SDLInitialized = true;
        }


        ENGINE_PROFILE_SCOPE("SDL_CreateWindows");
        m_window = SDL_CreateWindow(m_data.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_data.Width, m_data.Height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        ENGINE_ASSERT_MSG(m_window, "Failed to create SDL Window: {0}", SDL_GetError());

        // Set VSync Status
        SetVSync(true);

        // -1 means use whatever available card
        // SDL_RENDERER_ACCELERATED tells the system to use gpu if possible
        ENGINE_PROFILE_SCOPE("SDL_CreateRenderer");
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/);
        ENGINE_ASSERT_MSG(m_renderer, "Failed to create SDL Rendere: {0}", SDL_GetError());

    }

    void WindowsWindow::Shutdown()
    {
        ENGINE_PROFILE_FUNCTION();

        /*Delete Input Management here*/
        delete Input::s_instance;

        SDL_DestroyWindow(m_window);
    }
    
    void WindowsWindow::OnUpdate(Timestep dt)
    {
        ENGINE_PROFILE_FUNCTION();

        /*Update Input Management here*/
        Input::s_instance->Update();

        //LOG_ENGINE_TRACE("Delta Time : {0}s ({1}ms) ", dt.GetSeconds(), dt.GetMilliSeconds());
        
        //Clear color and clear color buffer bit equivalent
        SDL_SetRenderDrawColor(m_renderer, 96, 128, 255, 255);
        SDL_RenderClear(m_renderer);
        
        //SDL_AddEventWatch(FunctionCallback, (void*)&m_data);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            // WINDOWS EVENT
            case SDL_WINDOWEVENT:
            {
                switch (event.window.event)
                {
                    //Windows resize event
                    case SDL_WINDOWEVENT_RESIZED:
                    {
                        m_data.Width = event.window.data1;
                        m_data.Height = event.window.data2;

                        WindowResizeEvent resizeEvent(m_data.Width, m_data.Height);
                        m_data.EventCallback(resizeEvent);
                        break;
                    }
                    //Windows close event
                    case SDL_WINDOWEVENT_CLOSE:
                    {
                        WindowCloseEvent closeEvent;
                        m_data.EventCallback(closeEvent);
                        break;
                    }
                    case SDL_WINDOWEVENT_MAXIMIZED:
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                    {
                        WindowFocusEvent windowFocusEvent;
                        m_data.EventCallback(windowFocusEvent);
                        break;
                    }
                    case SDL_WINDOWEVENT_MINIMIZED:
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                    {
                        WindowLoseFocusEvent windowLoseFocusEvent;
                        m_data.EventCallback(windowLoseFocusEvent);
                        break;
                    }
                    case SDL_WINDOWEVENT_MOVED:
                    {
                        WindowMovedEvent windowMovedEvent;
                        m_data.EventCallback(windowMovedEvent);
                        break;
                    }
                default:
                    break;
                }

                break;
            }

            case SDL_KEYDOWN:
            {
                KeyPressedEvent keyPressEvent(event.key.keysym.sym, event.key.repeat ? 1 : 0);
                m_data.EventCallback(keyPressEvent);
            }
                break;

            case SDL_KEYUP:
            {
                KeyReleasedEvent keyPressEvent(event.key.keysym.sym);
                m_data.EventCallback(keyPressEvent);
            }
                break;

            case SDL_MOUSEBUTTONUP:
            {
                MouseButtonReleasedEvent mouseButtonReleasedEvent(event.key.keysym.sym);
                m_data.EventCallback(mouseButtonReleasedEvent);
            }
                break;

            case SDL_MOUSEBUTTONDOWN:
            {
                MouseButtonPressedEvent mouseButtonPressedEvent(event.key.keysym.sym);
                m_data.EventCallback(mouseButtonPressedEvent);
            }
                break;
            case SDL_MOUSEWHEEL:
            {
                MouseScrolledEvent mouseScrolledEvent(static_cast<float>(event.wheel.x), static_cast<float>(event.wheel.y));
                m_data.EventCallback(mouseScrolledEvent);
            }
                break;
            case SDL_MOUSEMOTION:
            {
                MouseMovedEvent mouseMovedEvent(static_cast<float>(event.motion.x), static_cast<float>(event.motion.y));
                m_data.EventCallback(mouseMovedEvent);
            }
                break;

            default:
                break;
            }
        }

        // render : must be called
        SDL_RenderPresent(m_renderer);
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        LOG_ENGINE_INFO("Set Vsync : {0}", enabled);
        
        enabled 
            ? SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1") 
            : SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");

        m_data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_data.VSync;
    }
}