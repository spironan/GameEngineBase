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
#include <GL/gl3w.h>

#include "Engine/Platform/Windows/WindowsWindow.h"
#include "Engine/Core/Base.h"
#include "Engine/Core/Application.h"

#if defined(GRAPHICS_CONTEXT_VULKAN)
    #include "Engine/Platform/Vulkan/VulkanContext.h"
#elif defined(GRAPHICS_CONTEXT_OPENGL)
    #include "Engine/Platform/OpenGL/OpenGLContext.h"
#endif

#include <imgui_impl_sdl.h>
#include <imgui.h>

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

#ifdef GRAPHICS_CONTEXT_OPENGL
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
#endif
        SDL_WindowFlags window_flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

#ifdef GRAPHICS_CONTEXT_VULKAN
        window_flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_VULKAN | window_flags);
#elif  defined(GRAPHICS_CONTEXT_OPENGL)
        window_flags= static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL |  window_flags);
#endif
        m_window = SDL_CreateWindow(m_data.Title.c_str()
                        , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED
                        , m_data.Width, m_data.Height
                        , window_flags);

        ENGINE_ASSERT_MSG(m_window, "Failed to create SDL Window: {0}", SDL_GetError());
        

        ENGINE_PROFILE_SCOPE("SDL_CreateRenderer");
        // create graphics context
#ifdef GRAPHICS_CONTEXT_VULKAN
        m_context = new VulkanContext(m_window);
#elif  defined(GRAPHICS_CONTEXT_OPENGL)
        m_context = new OpenGLContext(m_window);
#endif
        m_context->Init();
        
        // Set VSync Status
        SetVSync(properties.VSync);
    }

    void WindowsWindow::Shutdown()
    {
        ENGINE_PROFILE_FUNCTION();

        /* delete the current graphics context */
        delete m_context;

        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
    
    double WindowsWindow::CalcDeltaTime()
    {
        double time = static_cast<double>(SDL_GetPerformanceCounter());
        double dt =  (time - m_lastFrameTime) * 1000.0 / SDL_GetPerformanceFrequency();
        m_lastFrameTime = time;
        return dt;
    }

    void WindowsWindow::ProcessEvents()
    {
        ENGINE_PROFILE_FUNCTION();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // this should only run if there's imgui on
            ImGui_ImplSDL2_ProcessEvent(&event);
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
                KeyPressedEvent keyPressEvent((KeyCode)event.key.keysym.scancode, event.key.repeat ? 1 : 0);
                m_data.EventCallback(keyPressEvent);

                break;
            }
            case SDL_KEYUP:
            {
                KeyReleasedEvent keyPressEvent((KeyCode)event.key.keysym.scancode);
                m_data.EventCallback(keyPressEvent);

                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                MouseButtonReleasedEvent mouseButtonReleasedEvent(event.key.keysym.scancode);
                m_data.EventCallback(mouseButtonReleasedEvent);

                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                MouseButtonPressedEvent mouseButtonPressedEvent(event.key.keysym.scancode);
                m_data.EventCallback(mouseButtonPressedEvent);

                break;
            }
            case SDL_MOUSEWHEEL:
            {
                MouseScrolledEvent mouseScrolledEvent(static_cast<float>(event.wheel.x), static_cast<float>(event.wheel.y));
                m_data.EventCallback(mouseScrolledEvent);

                break;
            }
            case SDL_MOUSEMOTION:
            {
                MouseMovedEvent mouseMovedEvent(static_cast<float>(event.motion.x), static_cast<float>(event.motion.y));
                m_data.EventCallback(mouseMovedEvent);

                break;
            }
            default:
                break;
            }
        }
    }

    void WindowsWindow::SwapBuffers()
    {
        ENGINE_PROFILE_FUNCTION();

        // swap rendering buffers
        m_context->SwapBuffers();
    }

    void WindowsWindow::Maximize()
    {
        SDL_MaximizeWindow(m_window);
    }

    void WindowsWindow::Minimize()
    {
        SDL_MinimizeWindow(m_window);
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        LOG_ENGINE_INFO("Set Vsync : {0}", enabled);

        m_data.VSync = m_context->SetVSync(enabled);
    }

    void WindowsWindow::SetTitle(const std::string& title)
    {
        m_data.Title = title;
        SDL_SetWindowTitle(m_window, m_data.Title.c_str());
    }

    std::pair<int, int> WindowsWindow::GetWindowPos() const
    {
        int x, y;
        SDL_GetWindowPosition(m_window, &x, &y);
        return { x, y };
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_data.VSync;
    }

}