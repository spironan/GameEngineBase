/************************************************************************************//*!
\file           KeyEvent.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 15, 2021
\brief          

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/Events/Event.h"

namespace Engine 
{
    class KeyEvent : public Event
    {
    public:
        inline int GetKeyCode() const { return _keycode; }
        
        EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input)
    protected:
        KeyEvent(int keycode)
            : _keycode{ keycode } {}
        
        int _keycode;
    };


    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(int keycode, int repeatCount)
            : KeyEvent{ keycode }, _RepeatCount{ repeatCount } {}
    
        inline int GetRepeatCount() const { return _RepeatCount; }

        std::string ToString() const override 
        {
            std::stringstream ss;
            ss << "KeyPressedEvent : " << _keycode << "(" << _RepeatCount << ")";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int _RepeatCount;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keycode)
            : KeyEvent{ keycode } {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent : " << _keycode;
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
            ss << "KeyTypedEvent : " << _keycode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };



}

