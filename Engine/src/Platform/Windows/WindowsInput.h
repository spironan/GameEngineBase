/************************************************************************************//*!
\file           WindowsInput.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 15, 2021
\brief          Describes a Windows(Platform) specific input that implements 
                the generic Input interface. 
                Currently using SDL as the backend abstraction.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/Core/Input.h"

#include <sdl2/SDL.h>

#include "Platform/Windows/WindowsWindow.h"

namespace engine
{
    /********************************************************************************//*!
     @brief     Describes a Windows(Platform) specific input that implements 
                the generic Input interface. 
                Currently using SDL as the backend abstraction.
     
     @note      This class Should not be directly accessed but instead through the Input
                class and its interface functions.
    *//*********************************************************************************/
    class WindowsInput final : public Input
    {
    public:
        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        WindowsInput();
        virtual ~WindowsInput();

    protected:
        void Update() override;

        bool IsKeyDownImpl(KeyCode keycode) override;
        bool IsKeyPressedImpl(KeyCode keycode) override;
        bool IsKeyReleasedImpl(KeyCode keycode) override;
        
        bool IsMouseButtonDownImpl(MouseCode button) override;
        bool IsMouseButtonPressedImpl(MouseCode button) override;
        bool IsMouseButtonReleasedImpl(MouseCode button) override;

        std::pair<int, int> GetMousePositionImpl() override;
        int GetMouseXImpl() override;
        int GetMouseYImpl() override;

    private :
        int m_keyLength;
        const Uint8* m_keyboardState;
        Uint8* m_prevKeyboardState;

        Uint32 m_prevMouseState;
        Uint32 m_mouseState;

        int m_mouseXPos;
        int m_mouseYPos;
    };
}