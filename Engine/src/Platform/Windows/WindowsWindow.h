#pragma once

#include "Engine/Core/Window.h"

#include <sdl2/SDL.h>

namespace Engine
{
    class Windowswindow final : public Window
    {
    public:
        Windowswindow(const WindowProperties& props);

        virtual ~Windowswindow();

        void OnUpdate() override;

        unsigned int GetWidth() const override { return m_Data.Width; };
        unsigned int GetHeight() const override { return m_Data.Height; };

        void SetEventCallback(const EventCallbackFn & callback) override 
        {
            m_Data.EventCallback = callback;
        }

        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        void* GetNativeWindow() const override { return m_Window; };

    private:
        void Init(const WindowProperties& properties);
        void Shutdown();

    private:
        SDL_Window* m_Window;
        SDL_Renderer* m_Renderer;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };
}