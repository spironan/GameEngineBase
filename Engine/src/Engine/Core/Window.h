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
#include "Engine/Core/Events/Event.h"
#include "Engine/Core/Timestep.h"

namespace engine
{
    /********************************************************************************//*!
     @brief     Properties that make up a window
    *//*********************************************************************************/
    struct WindowProperties
    {
        std::string Title = "Core Engine";
        unsigned int Width = 1600;
        unsigned int Height = 900;
        bool Fullscreen = false;
        bool VSync = true;

        ///****************************************************************************//*!
        // @brief     Default constructor determining the title, width and height.
        //*//*****************************************************************************/
        //WindowProperties(const std::string& title = "Core Engine",
        //    unsigned int width = 1600, unsigned int height = 900)
        //    : Title(title), Width(width), Height(height)
        //{
        //}
    };

    /********************************************************************************//*!
     @brief     Interface representing a desktop system based window
    *//*********************************************************************************/
    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        virtual ~Window() = default;

        /*-----------------------------------------------------------------------------*/
        /* Functions                                                                   */
        /*-----------------------------------------------------------------------------*/
        virtual double CalcDeltaTime() = 0;

        virtual void ProcessEvents() = 0;
        virtual void SwapBuffers() = 0;
        //virtual void OnUpdate(Timestep dt) = 0;

        virtual void Maximize() = 0;
        virtual void Minimize() = 0;

        /****************************************************************************//*!
         @brief     Static function to create a Window of the current platform

         @return    the pointer to the created windows. Will result in nullptr if 
                    the platform is not supported.
        *//*****************************************************************************/
        static Window* Create(const WindowProperties& props = WindowProperties{});

        /*-----------------------------------------------------------------------------*/
        /* Getters                                                                     */
        /*-----------------------------------------------------------------------------*/
        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;
        virtual std::pair<unsigned int, unsigned int> GetSize() const = 0;
        virtual std::pair<int, int> GetWindowPos() const = 0;
        /****************************************************************************//*!
         @brief     Retrieve the Native Window class of the backend system.
                    Currently uses SDL_Window thus static cast it to SDL_Window*
                    to make use of it.

         @return    Pointer to the Actual Window implementation, requires the user to
                    know the backend system and cast it to the appropriate window
        *//*****************************************************************************/
        virtual void* GetNativeWindow() const = 0;

        virtual void* GetRenderingContext() const = 0;

        virtual bool IsVSync() const = 0;

        /*-----------------------------------------------------------------------------*/
        /* Setters                                                                     */
        /*-----------------------------------------------------------------------------*/
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual void SetTitle(const std::string& title) = 0;

    protected:
        double m_lastFrameTime = 0;
    };
}