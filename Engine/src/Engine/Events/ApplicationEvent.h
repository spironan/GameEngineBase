/************************************************************************************//*!
\file           ApplicationEvent.h
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

#include "Event.h"

namespace Engine 
{
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
            : _width{ width }, _height{ height } {}

        unsigned int GetWidth() const { return _width; }
        unsigned int GetHeight() const { return _height; }

        std::string ToString() const override 
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << _width << ", " << _height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(bitmask{ EventCategory::Application })

    private:
        unsigned int _width, _height;
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;
        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(bitmask{ EventCategory::Application })
    };

    class WindowFocusEvent : public Event
    {
    public:
        WindowFocusEvent() = default;
        EVENT_CLASS_TYPE(WindowFocus)
        EVENT_CLASS_CATEGORY(bitmask{ EventCategory::Application })
    };

    class WindowLoseFocusEvent : public Event
    {
    public:
        WindowLoseFocusEvent() = default;
        EVENT_CLASS_TYPE(WindowLoseFocus)
        EVENT_CLASS_CATEGORY(bitmask{ EventCategory::Application })
    };

    class WindowMovedEvent : public Event
    {
    public:
        WindowMovedEvent() = default;
        EVENT_CLASS_TYPE(WindowMoved)
        EVENT_CLASS_CATEGORY(bitmask{ EventCategory::Application })
    };

}