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

namespace engine
{
    //forward declaration
    class GraphicsContext;

    /********************************************************************************//*!
     @brief     Describes a Windows(Platform) specific windows that implements 
                the generic window interface. 
                Currently using SDL as the backend abstraction.

     @note      This class Should not be directly accessed but instead through the Window
                class and its interface functions in application.
    *//*********************************************************************************/
    class WindowsWindow final : public Window
    {
    public:
        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        WindowsWindow(const WindowProperties& props);
        virtual ~WindowsWindow();

        /*-----------------------------------------------------------------------------*/
        /* Functions                                                                   */
        /*-----------------------------------------------------------------------------*/
        double CalcDeltaTime() override;

        void ProcessEvents() override;
        void SwapBuffers() override;
        void Maximize() override;
        void Minimize() override;

        //void OnUpdate(Timestep dt) override;

        /*-----------------------------------------------------------------------------*/
        /* Getters                                                                     */
        /*-----------------------------------------------------------------------------*/
        unsigned int GetWidth() const override { return m_data.Width; }
        unsigned int GetHeight() const override { return m_data.Height; }
        std::pair<unsigned int, unsigned int> GetSize() const override { return { m_data.Width, m_data.Height }; }
        std::pair<int, int> GetWindowPos() const override;

        void* GetNativeWindow() const override { return m_window; }
        void* GetRenderingContext() const override { return m_context; }

        bool IsVSync() const override;

        /*-----------------------------------------------------------------------------*/
        /* Setters                                                                     */
        /*-----------------------------------------------------------------------------*/
        void SetEventCallback(const EventCallbackFn & callback) override 
        {
            m_data.EventCallback = callback;
        }
        void SetVSync(bool enabled) override;
        void SetTitle(const std::string& title) override;

    private:
        void Init(const WindowProperties& properties);
        void Shutdown();

    private:
        SDL_Window* m_window;
        GraphicsContext* m_context;
    private:
        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_data;
    };
}