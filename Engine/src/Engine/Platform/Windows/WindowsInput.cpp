/************************************************************************************//*!
\file           WindowsInput.cpp
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 15, 2021
\brief          Describes a Windows(Platform) specific input that ements 
                the generic Input interface. 
                Currently using SDL as the backend abstraction.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/

#include "pch.h"
#include <sdl2/SDL.h>

#include "Engine/Core/Input.h"

#include "Engine/Core/Application.h"

namespace engine
{
    // Windows Platform currently using SDL
    // SDL Specific Input Requirements

    int m_keyLength;
    const Uint8* m_keyboardState;
    Uint8* m_prevKeyboardState;
    
    Uint32 m_prevMouseState;
    Uint32 m_mouseState;

    int m_mouseXPos;
    int m_mouseYPos;

    int m_mouseXDelta;
    int m_mouseYDelta;

    void Input::Init()
    {
        m_prevMouseState = m_mouseState = m_mouseXPos = m_mouseYPos = 0;

        m_keyboardState = SDL_GetKeyboardState(&m_keyLength);
        m_prevKeyboardState = new Uint8[m_keyLength];
        memcpy(m_prevKeyboardState, m_keyboardState, m_keyLength);
    }

    void Input::Update()
    {
        memcpy(m_prevKeyboardState, m_keyboardState, m_keyLength);
        m_prevMouseState = m_mouseState;

        m_mouseState = SDL_GetMouseState(&m_mouseXPos, &m_mouseYPos);

        SDL_GetRelativeMouseState(&m_mouseXDelta, &m_mouseYDelta);
    }

    void Input::ShutDown()
    {
        delete[] m_prevKeyboardState;
        m_prevKeyboardState = nullptr;
    }


    bool Input::IsKeyDown(const KeyCode keycode)
    {
        return m_keyboardState[static_cast<int>(keycode)];
    }

    bool Input::IsKeyPressed(const KeyCode keycode)
    {
        return !m_prevKeyboardState[static_cast<int>(keycode)] && m_keyboardState[static_cast<int>(keycode)];
    }

    bool Input::IsKeyReleased(const KeyCode keycode)
    {
        return m_prevKeyboardState[static_cast<int>(keycode)] && !m_keyboardState[static_cast<int>(keycode)];
    }
    
    bool Input::IsAnyKeyDown()
    {
        for (KeyCode keycode{ 0 }; keycode < m_keyLength; ++keycode)
        {
            if (IsKeyDown(keycode)) return true;
        }

        return false;
    }
    
    bool Input::IsAnyKeyPressed()
    {
        for (KeyCode keycode{ 0 }; keycode < m_keyLength; ++keycode)
        {
            if (IsKeyPressed(keycode)) return true;
        }

        return false;
    }

    bool Input::IsAnyKeyReleased()
    {
        for (KeyCode keycode{ 0 }; keycode < m_keyLength; ++keycode)
        {
            if (IsKeyReleased(keycode)) return true;
        }

        return false;
    }

    std::vector<KeyCode> Input::GetKeysDown()
    {
        std::vector<KeyCode> keys;
        
        for (KeyCode keycode{ 0 }; keycode < m_keyLength; ++keycode)
        {
            if (IsKeyDown(keycode)) keys.emplace_back(keycode);
        }

        return keys;
    }

    std::vector<KeyCode> Input::GetKeysPressed()
    {
        std::vector<KeyCode> keys;
        
        for (KeyCode keycode{ 0 }; keycode < m_keyLength; ++keycode)
        {
            if (IsKeyPressed(keycode)) keys.emplace_back(keycode);
        }

        return keys;
    }

    std::vector<KeyCode> Input::GetKeysReleased()
    {
        std::vector<KeyCode> keys;
        
        for (KeyCode keycode{ 0 }; keycode < m_keyLength; ++keycode)
        {
            if (IsKeyReleased(keycode)) keys.emplace_back(keycode);
        }

        return keys;
    }


    bool Input::IsMouseButtonDown(const MouseCode button)
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

    bool Input::IsMouseButtonPressed(const MouseCode button)
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

    bool Input::IsMouseButtonReleased(const MouseCode button)
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

    bool Input::IsAnyMouseButtonDown()
    {
        for (MouseCode mousecode{ 0 }; mousecode <= Mouse::ButtonLast; ++mousecode)
        {
            if (IsMouseButtonDown(mousecode)) return true;
        }

        return false;
    }

    bool Input::IsAnyMouseButtonPressed()
    {
        
        for (MouseCode mousecode{ 0 }; mousecode <= Mouse::ButtonLast; ++mousecode)
        {
            if (IsMouseButtonPressed(mousecode)) return true;
        }

        return false;
    }

    bool Input::IsAnyMouseButtonReleased()
    {

        for (MouseCode mousecode{ 0 }; mousecode <= Mouse::ButtonLast; ++mousecode)
        {
            if (IsMouseButtonReleased(mousecode)) return true;
        }

        return false;
    }

    std::vector<MouseCode> Input::GetMouseButtonsDown()
    {
        std::vector<MouseCode> mouseButtons;

        for (MouseCode mousecode{ 0 }; mousecode <= Mouse::ButtonLast; ++mousecode)
        {
            if (IsMouseButtonDown(mousecode)) mouseButtons.emplace_back(mousecode);
        }

        return mouseButtons;
    }

    std::vector<MouseCode> Input::GetMouseButtonsPressed()
    {
        std::vector<MouseCode> mouseButtons;

        for (MouseCode mousecode{ 0 }; mousecode <= Mouse::ButtonLast; ++mousecode)
        {
            if (IsMouseButtonPressed(mousecode)) mouseButtons.emplace_back(mousecode);
        }

        return mouseButtons;
    }

    std::vector<MouseCode> Input::GetMouseButtonsReleased()
    {
        std::vector<MouseCode> mouseButtons;

        for (MouseCode mousecode{ 0 }; mousecode <= Mouse::ButtonLast; ++mousecode)
        {
            if (IsMouseButtonReleased(mousecode)) mouseButtons.emplace_back(mousecode);
        }

        return mouseButtons;
    }


    std::pair<int, int> Input::GetMousePosition()
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return { x, y };
    }

    std::pair<int, int> Input::GetMouseDelta()
    {
        return { m_mouseXDelta, m_mouseYDelta };   // invert y so that you get bottom left as 0,0 instead of top left as 0,0 (default is window space)
    }
    
    int Input::GetMouseX()
    {
        return GetMousePosition().first;
    }

    int Input::GetMouseY()
    {
        return GetMousePosition().second;
    }
}