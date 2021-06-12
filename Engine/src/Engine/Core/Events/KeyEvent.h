/************************************************************************************//*!
\file           KeyEvent.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 15, 2021
\brief          Implements an event related to the keyboard. Used to extract info from
                the event such as what key was pressed and released.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/Core/Events/Event.h"
#include "Engine/Core/KeyCode.h"

namespace engine
{
    /********************************************************************************//*!
     @brief     Implements a basic Key Event. Stores the basic keycode
                and is categorised under a keyboard and input category
    *//*********************************************************************************/
    class KeyEvent : public Event
    {
    public:
        inline KeyCode GetKeyCode() const { return m_keycode; }
        
        EVENT_CLASS_CATEGORY(utility::bitmask{ EVENT_CATEGORY::KEYBOARD } | EVENT_CATEGORY::INPUT)
    protected:
        KeyEvent(KeyCode keycode)
            : m_keycode{ keycode } {}
        
        KeyCode m_keycode;
    };

    /********************************************************************************//*!
     @brief     Implements the Key Pressed Event. Stores the keycode and whether
                the key currently pressed is a repeatedly held down key.
    *//*********************************************************************************/
    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(const KeyCode keycode, int repeatCount)
            : KeyEvent{ keycode }, m_repeatCount{ repeatCount } {}
    
        inline int GetRepeatCount() const { return m_repeatCount; }

        std::string ToString() const override 
        {
            std::stringstream ss;
            ss << "KeyPressedEvent : " << m_keycode << "(" << m_repeatCount << ")";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KEYPRESSED)
    private:
        int m_repeatCount;
    };

    /********************************************************************************//*!
     @brief     Implements the Key Released Event.
    *//*********************************************************************************/
    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(const KeyCode keycode)
            : KeyEvent{ keycode } {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent : " << m_keycode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KEYRELEASED)
    };

    /********************************************************************************//*!
     @brief     Implements the Key Typed Event.
    *//*********************************************************************************/
    class KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(const KeyCode keycode)
            : KeyEvent(keycode) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyTypedEvent : " << m_keycode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KEYTYPED)
    };

}