/************************************************************************************//*!
\file           Window.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 24, 2021
\brief          Describes a general abstracted window and its interface.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include <string>
#include "Engine/Events/Event.h"
#include "Engine/Core/Timestep.h"

namespace engine
{
    /********************************************************************************//*!
     @brief     Properties that make up a window
    *//*********************************************************************************/
    struct WindowProperties
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProperties(const std::string& title = "Core Engine",
            unsigned int width = 1600, unsigned int height = 900)
            : Title(title), Width(width), Height(height)
        {
        }
    };

    /********************************************************************************//*!
     @brief     Interface representing a desktop system based window
    *//*********************************************************************************/
    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;

        virtual void OnUpdate(Timestep dt) = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void* GetNativeWindow() const = 0;

        static Window* Create(const WindowProperties& props = WindowProperties{});
    };
}