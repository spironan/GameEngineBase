/************************************************************************************//*!
\file           MouseEvent.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 15, 2021
\brief          Implements an event related to the mouse. Used to extract info from
                the event such as what mouse button was pressed and how much the mouse
                has scrolled.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/Core/Events/Event.h"
#include "Engine/Core/MouseCode.h"

namespace engine
{
    /********************************************************************************//*!
     @brief     Implements a MOUSE Moved Event. 
    *//*********************************************************************************/
    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(const float x, const float y)
            : m_mouseX{x}, m_mouseY{y} {}

        float GetX() const { return m_mouseX; }
        float GetY() const { return m_mouseY; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MOUSEMOVED)
        EVENT_CLASS_CATEGORY(utility::bitmask{ EVENT_CATEGORY::MOUSE } | EVENT_CATEGORY::INPUT)
    private:
        float m_mouseX, m_mouseY;
    };

    /********************************************************************************//*!
     @brief     Implements a MOUSE Scrolled Event.
    *//*********************************************************************************/
    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(const float xOffset, const float yOffset)
            : m_xOffset{ xOffset }, m_yOffset{ yOffset } {}

        float GetX() const { return m_xOffset; }
        float GetY() const { return m_yOffset; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << m_xOffset << ", " << m_yOffset;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MOUSESCROLLED)
        EVENT_CLASS_CATEGORY(utility::bitmask{ EVENT_CATEGORY::MOUSE } | EVENT_CATEGORY::INPUT)
    private:
        float m_xOffset, m_yOffset;
    };

    /********************************************************************************//*!
     @brief     Implements a MOUSE Button Event.
    *//*********************************************************************************/
    class MouseButtonEvent : public Event
    {
    public:
        MouseCode GetMouseButton() const { return m_Button; }

        EVENT_CLASS_CATEGORY(utility::bitmask{ EVENT_CATEGORY::MOUSE } | EVENT_CATEGORY::INPUT | EVENT_CATEGORY::MOUSEBUTTON)
    protected:
        MouseButtonEvent(const MouseCode button)
            : m_Button(button) {}

        MouseCode m_Button;
    };

    /********************************************************************************//*!
     @brief     Implements a MOUSE Button Pressed Event.
    *//*********************************************************************************/
    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(const MouseCode button)
            : MouseButtonEvent(button) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MOUSEBUTTONPRESSED)
    };

    /********************************************************************************//*!
     @brief     Implements a MOUSE Button Released Event.
    *//*********************************************************************************/
    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(const MouseCode button)
            : MouseButtonEvent(button) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MOUSEBUTTONRELEASED)
    };
}