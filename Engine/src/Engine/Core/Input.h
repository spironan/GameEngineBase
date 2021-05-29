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
    class WindowsWindow;

    class Input
    {
    public:

        static bool IsKeyDown(KeyCode keycode) { return s_instance->IsKeyDownImpl(keycode); }
        static bool IsKeyPressed(KeyCode keycode) { return s_instance->IsKeyPressedImpl(keycode); }
        static bool IsKeyReleased(KeyCode keycode) { return s_instance->IsKeyReleasedImpl(keycode); }

        static bool IsMouseButtonDown(MouseCode button) { return s_instance->IsMouseButtonDownImpl(button); }
        static bool IsMouseButtonPressed(MouseCode button) { return s_instance->IsMouseButtonPressedImpl(button); }
        static bool IsMouseButtonReleased(MouseCode button) { return s_instance->IsMouseButtonReleasedImpl(button); }

        static std::pair<int, int> GetMousePosition() { return s_instance->GetMousePositionImpl(); }
        static int GetMouseX() { return s_instance->GetMouseXImpl(); }
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