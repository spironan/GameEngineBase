/************************************************************************************//*!
\file           WindowsWindow.h
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
#pragma once

#include "Engine/Core/Window.h"

//forward declaration
struct SDL_Window;
struct SDL_Renderer;

namespace engine
{
    class WindowsWindow final : public Window
    {
    public:
        WindowsWindow(const WindowProperties& props);

        virtual ~WindowsWindow();

        void OnUpdate(Timestep dt) override;

        unsigned int GetWidth() const override { return m_data.Width; };
        unsigned int GetHeight() const override { return m_data.Height; };

        void SetEventCallback(const EventCallbackFn & callback) override 
        {
            m_data.EventCallback = callback;
        }

        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        void* GetNativeWindow() const override { return m_window; };

    private:
        void Init(const WindowProperties& properties);
        void Shutdown();

    private:
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;

    public:
        //expose this for now
        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

    private:
        WindowData m_data;
    };
}