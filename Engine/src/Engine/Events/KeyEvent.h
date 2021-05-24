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

#include "Engine/Events/Event.h"

namespace engine
{
    //using namespace utility;

    class KeyEvent : public Event
    {
    public:
        inline int GetKeyCode() const { return m_keycode; }
        
        EVENT_CLASS_CATEGORY(utility::bitmask{ EVENT_CATEGORY::Keyboard } | EVENT_CATEGORY::Input)
    protected:
        KeyEvent(int keycode)
            : m_keycode{ keycode } {}
        
        int m_keycode;
    };


    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(int keycode, int repeatCount)
            : KeyEvent{ keycode }, m_repeatCount{ repeatCount } {}
    
        inline int GetRepeatCount() const { return m_repeatCount; }

        std::string ToString() const override 
        {
            std::stringstream ss;
            ss << "KeyPressedEvent : " << m_keycode << "(" << m_repeatCount << ")";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int m_repeatCount;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keycode)
            : KeyEvent{ keycode } {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent : " << m_keycode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };


    class KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(int keycode) 
            : KeyEvent(keycode) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyTypedEvent : " << m_keycode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };



}

