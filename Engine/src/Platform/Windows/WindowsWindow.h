#pragma once

#include "Engine/Core/Window.h"

#include <sdl2/SDL.h>

namespace engine
{
    class Windowswindow final : public Window
    {
    public:
        Windowswindow(const WindowProperties& props);

        virtual ~Windowswindow();

        void OnUpdate() override;

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