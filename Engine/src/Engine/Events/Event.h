/************************************************************************************//*!
\file           Event.h
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

#include "Utility/Bitmask.h"

namespace Engine 
{
    enum class EventType : int
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLoseFocus, WindowMoved,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum class EventCategory : int
    {
        None,
        Application,
        Input,
        Keyboard,
        Mouse,
        MouseButton,
    };

    //typesafe enum that still has normal enum operations.
    using EventCategoryType = bitmask<EventCategory>;   

    #define EVENT_CLASS_CATEGORY(category) \
    virtual EventCategoryType GetCategoryFlag() const override { return category; }

    #define EVENT_CLASS_TYPE(type) \
    static EventType GetStaticType() { return EventType::##type; }\
    virtual EventType GetEventType() const override { return GetStaticType(); }\
    virtual const char* GetName() const override { return #type; }

    class Event 
    {
    public:
        bool Handled = false;

        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual EventCategoryType GetCategoryFlag() const = 0;
        virtual std::string ToString() const { return GetName(); }

        inline bool IsInCategory(EventCategory categories)
        {
            return GetCategoryFlag() & categories;
        }

    };

    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event)
            : _Event(event)
        {
        }

        template<typename T, typename F>
        bool Dispatch(const F& func)
        {
            if (_Event.GetEventType() == T::GetStaticType())
            {
                _Event.Handled |= func(static_cast<T&>(_Event));
                return true;
            }
            return false;
        }

    private:
        Event& _Event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }

}