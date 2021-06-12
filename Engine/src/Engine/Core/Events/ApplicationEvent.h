/************************************************************************************//*!
\file           ApplicationEvent.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 15, 2021
\brief          Implements an event related to the window. Used to extract info from
                the event such as what is the screen resized to etc.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/Core/Events/Event.h"

namespace engine
{
    /********************************************************************************//*!
     @brief     Implements a Window Resize Event
    *//*********************************************************************************/
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
            : m_width{ width }, m_height{ height } {}

        unsigned int GetWidth() const { return m_width; }
        unsigned int GetHeight() const { return m_height; }

        std::string ToString() const override 
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_width << ", " << m_height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WINDOWRESIZE)
        EVENT_CLASS_CATEGORY(utility::bitmask{ EVENT_CATEGORY::APPLICATION })

    private:
        unsigned int m_width, m_height;
    };

    /********************************************************************************//*!
     @brief     Implements a Window Close Event
    *//*********************************************************************************/
    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;
        EVENT_CLASS_TYPE(WINDOWCLOSE)
        EVENT_CLASS_CATEGORY(utility::bitmask{ EVENT_CATEGORY::APPLICATION })
    };


    /********************************************************************************//*!
     @brief     Implements a Window Focus Event
    *//*********************************************************************************/
    class WindowFocusEvent : public Event
    {
    public:
        WindowFocusEvent() = default;
        EVENT_CLASS_TYPE(WINDOWFOCUS)
        EVENT_CLASS_CATEGORY(utility::bitmask{ EVENT_CATEGORY::APPLICATION })
    };

    /********************************************************************************//*!
     @brief     Implements a Window Lose Focus Event
    *//*********************************************************************************/
    class WindowLoseFocusEvent : public Event
    {
    public:
        WindowLoseFocusEvent() = default;
        EVENT_CLASS_TYPE(WINDOWLOSEFOCUS)
        EVENT_CLASS_CATEGORY(utility::bitmask{ EVENT_CATEGORY::APPLICATION })
    };

    class WindowMovedEvent : public Event
    {
    public:
        WindowMovedEvent() = default;
        EVENT_CLASS_TYPE(WINDOWMOVED)
        EVENT_CLASS_CATEGORY(utility::bitmask{ EVENT_CATEGORY::APPLICATION })
    };

}