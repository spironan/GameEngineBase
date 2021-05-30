/************************************************************************************//*!
\file           Input.cpp
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
    }

    void Input::ShutDown()
    {
        delete[] m_prevKeyboardState;
        m_prevKeyboardState = nullptr;
    }


    bool Input::IsKeyDown(const KeyCode keycode)
    {
        return m_keyboardState[keycode];
    }

    bool Input::IsKeyPressed(const KeyCode keycode)
    {
        return !m_prevKeyboardState[keycode] && m_keyboardState[keycode];
    }

    bool Input::IsKeyReleased(const KeyCode keycode)
    {
        return m_prevKeyboardState[keycode] && !m_keyboardState[keycode];
    }


    bool Input::IsMouseButtonDown(const MouseCode button)
    {
        Uint32 mask = 0;

        switch (button)
        {
        case mouse::ButtonLeft:
            mask = SDL_BUTTON_LMASK;
            break;

        case mouse::ButtonRight:
            mask = SDL_BUTTON_RMASK;
            break;

        case mouse::ButtonMiddle:
            mask = SDL_BUTTON_MMASK;
            break;

        case mouse::ButtonLast:
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
        case mouse::ButtonLeft:
            mask = SDL_BUTTON_LMASK;
            break;

        case mouse::ButtonRight:
            mask = SDL_BUTTON_RMASK;
            break;

        case mouse::ButtonMiddle:
            mask = SDL_BUTTON_MMASK;
            break;

        case mouse::ButtonLast:
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
        case mouse::ButtonLeft:
            mask = SDL_BUTTON_LMASK;
            break;

        case mouse::ButtonRight:
            mask = SDL_BUTTON_RMASK;
            break;

        case mouse::ButtonMiddle:
            mask = SDL_BUTTON_MMASK;
            break;

        case mouse::ButtonLast:
            mask = SDL_BUTTON_X1MASK;
            break;
        }

        return (m_prevMouseState & mask) && !(m_mouseState & mask);
    }


    std::pair<int, int> Input::GetMousePosition()
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        return { x, y };
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