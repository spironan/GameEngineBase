/************************************************************************************//*!
\file           WindowsInput.cpp
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
#include "pch.h"
#include <sdl2/SDL.h>

#include "Platform/Windows/WindowsInput.h"

#include "Engine/Core/Application.h"


namespace engine
{
    // Initialize input system as windows input
    // may need to shift this to the appropriate platform window file 
    // if more then one architecture is supported
    Input* Input::s_instance = new WindowsInput();

    WindowsInput::WindowsInput()
        : m_prevMouseState { 0 }
        , m_mouseState { 0 }
        , m_mouseXPos { 0 }
        , m_mouseYPos { 0 }

    {
        m_keyboardState = SDL_GetKeyboardState(&m_keyLength);
        m_prevKeyboardState = new Uint8[m_keyLength];
        memcpy(m_prevKeyboardState, m_keyboardState, m_keyLength);
    }
    
    WindowsInput::~WindowsInput()
    {
        delete[] m_prevKeyboardState;
        m_prevKeyboardState = nullptr;
    }

    void WindowsInput::Update()
    {
        memcpy(m_prevKeyboardState, m_keyboardState, m_keyLength);
        m_prevMouseState = m_mouseState;

        m_mouseState = SDL_GetMouseState(&m_mouseXPos, &m_mouseYPos);
    }

    bool WindowsInput::IsKeyDownImpl(const KeyCode keycode)
    {
        return m_keyboardState[keycode];
    }

    bool WindowsInput::IsKeyPressedImpl(const KeyCode keycode)
    {
        return !m_prevKeyboardState[keycode] && m_keyboardState[keycode];
    }

    bool WindowsInput::IsKeyReleasedImpl(const KeyCode keycode)
    {
        return m_prevKeyboardState[keycode] && !m_keyboardState[keycode];
    }


    bool WindowsInput::IsMouseButtonDownImpl(const MouseCode button)
    {
        Uint32 mask = 0;

        switch (button)
        {
        case Mouse::ButtonLeft:
            mask = SDL_BUTTON_LMASK;
            break;

        case Mouse::ButtonRight:
            mask = SDL_BUTTON_RMASK;
            break;

        case Mouse::ButtonMiddle:
            mask = SDL_BUTTON_MMASK;
            break;

        case Mouse::ButtonLast:
            mask = SDL_BUTTON_X1MASK;
            break;

        }

        return (m_mouseState & mask);
    }

    bool WindowsInput::IsMouseButtonPressedImpl(const MouseCode button)
    {
        Uint32 mask = 0;

        switch (button)
        {
        case Mouse::ButtonLeft:
            mask = SDL_BUTTON_LMASK;
            break;

        case Mouse::ButtonRight:
            mask = SDL_BUTTON_RMASK;
            break;

        case Mouse::ButtonMiddle:
            mask = SDL_BUTTON_MMASK;
            break;

        case Mouse::ButtonLast:
            mask = SDL_BUTTON_X1MASK;
            break;
        }

        return !(m_prevMouseState & mask) && (m_mouseState & mask);
    }

    bool WindowsInput::IsMouseButtonReleasedImpl(const MouseCode button)
    {
        Uint32 mask = 0;

        switch (button)
        {
        case Mouse::ButtonLeft:
            mask = SDL_BUTTON_LMASK;
            break;

        case Mouse::ButtonRight:
            mask = SDL_BUTTON_RMASK;
            break;

        case Mouse::ButtonMiddle:
            mask = SDL_BUTTON_MMASK;
            break;

        case Mouse::ButtonLast:
            mask = SDL_BUTTON_X1MASK;
            break;
        }

        return (m_prevMouseState & mask) && !(m_mouseState & mask);
    }



    std::pair<int, int> WindowsInput::GetMousePositionImpl()
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        return { x, y };
    }

    int WindowsInput::GetMouseXImpl()
    {
        return GetMousePosition().first;
    }

    int WindowsInput::GetMouseYImpl()
    {
        return GetMousePosition().second;
    }
}