/************************************************************************************//*!
\file           Input.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 24, 2021
\brief          Implementation of the general abstracted input system that will be used
                by the client.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/Core/MouseCode.h"
#include "Engine/Core/KeyCode.h"

namespace engine
{
    // forward declaration
    class WindowsWindow;

    /****************************************************************************//*!
     @brief     Interface of the Engine's Input system that will be used by 
                by the client.
    *//*****************************************************************************/
    class Input
    {
    public:
        /*-----------------------------------------------------------------------------*/
        /* Interface Functions                                                         */
        /*-----------------------------------------------------------------------------*/
        /****************************************************************************//*!
         @brief     Determine if a key by the given keycode is currently held down

         @note      This function will return true even when keypressed is true too.

         @return    Whether the key is currently down.
        *//*****************************************************************************/
        static bool IsKeyDown(KeyCode keycode) { return s_instance->IsKeyDownImpl(keycode); }
        /****************************************************************************//*!
         @brief     Determine if a key by the given keycode has been triggered within
                    this frame.

         @return    Whether the key is triggered this frame .
        *//*****************************************************************************/
        static bool IsKeyPressed(KeyCode keycode) { return s_instance->IsKeyPressedImpl(keycode); }
        /****************************************************************************//*!
         @brief     Determine if a key by the given keycode has been triggered within
                    this frame.
         
         @note      KeyRelease returns true once when a key that was pressed is released.
                    To check if a key is not pressed all the time 
                    use !(IsKeyDown(keycode)) instead.

         @return    Whether the key is Released this frame.
        *//*****************************************************************************/
        static bool IsKeyReleased(KeyCode keycode) { return s_instance->IsKeyReleasedImpl(keycode); }

        /****************************************************************************//*!
         @brief     Determine if a mouse button by the given mouse code is 
                    currently held down

         @note      This function will return true even when mousepressed is true too.

         @return    Whether the mouse button is currently down.
        *//*****************************************************************************/
        static bool IsMouseButtonDown(MouseCode button) { return s_instance->IsMouseButtonDownImpl(button); }
        /****************************************************************************//*!
         @brief     Determine if a mouse button by the given mouse code is
                    has been triggered this frame.

         @return    Whether the mouse button is being clicked this frame.
        *//*****************************************************************************/
        static bool IsMouseButtonPressed(MouseCode button) { return s_instance->IsMouseButtonPressedImpl(button); }
        /****************************************************************************//*!
         @brief     Determine if a mouse button by the given mouse code is
                    has been released this frame.

         @return    Whether the mouse button is being released this frame.
        *//*****************************************************************************/
        static bool IsMouseButtonReleased(MouseCode button) { return s_instance->IsMouseButtonReleasedImpl(button); }


        /****************************************************************************//*!
         @brief     Retrieve the current mouse position in screen coordinates

         @return    a pair containing the x and y position of the mouse
        *//*****************************************************************************/
        static std::pair<int, int> GetMousePosition() { return s_instance->GetMousePositionImpl(); }
        /****************************************************************************//*!
         @brief     Retrieve the current X-axis mouse position in screen coordinates

         @return    X-axis position of where the mouse current is at
        *//*****************************************************************************/
        static int GetMouseX() { return s_instance->GetMouseXImpl(); }
        /****************************************************************************//*!
         @brief     Retrieve the current Y-axis mouse position in screen coordinates

         @return    Y-axis position of where the mouse current is at
        *//*****************************************************************************/
        static int GetMouseY() { return s_instance->GetMouseYImpl(); }
    
    protected:

        Input() = default;
        virtual ~Input() = default;
        
        virtual void Update() = 0;

        virtual bool IsKeyDownImpl(KeyCode keycode) = 0;
        virtual bool IsKeyPressedImpl(KeyCode keycode) = 0;
        virtual bool IsKeyReleasedImpl(KeyCode keycode) = 0;

        virtual bool IsMouseButtonDownImpl(MouseCode button) = 0;
        virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
        virtual bool IsMouseButtonReleasedImpl(MouseCode button) = 0;
    
        virtual std::pair<int, int> GetMousePositionImpl() = 0;
        virtual int GetMouseXImpl() = 0;
        virtual int GetMouseYImpl() = 0;

    private:

        //friend void Windowswindow::OnUpdate(Timestep);
        friend class WindowsWindow;

        static Input* s_instance;
    };
}