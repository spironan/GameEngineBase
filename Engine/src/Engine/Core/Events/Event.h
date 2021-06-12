/************************************************************************************//*!
\file           Event.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 15, 2021
\brief          Describes the various units that describes an events, from 
                enum to determine an events type and category to what a basic event is
                and what a dispatcher does.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Utility/Bitmask.h"

namespace engine
{
    /********************************************************************************//*!
     @brief     Describes the different types of events that the engine caters for.
    *//*********************************************************************************/
    enum class EVENT_TYPE : int
    {
        NONE = 0,
        WINDOWCLOSE, WINDOWRESIZE, WINDOWFOCUS, WINDOWLOSEFOCUS, WINDOWMOVED,
        KEYPRESSED, KEYRELEASED, KEYTYPED,
        MOUSEBUTTONPRESSED, MOUSEBUTTONRELEASED, MOUSEMOVED, MOUSESCROLLED
    };

    /********************************************************************************//*!
     @brief     Describes the various categories used to classify each event type.
     
     @note      An event can have more then 1 category. Used to simplify when looking
                for inputs
    *//*********************************************************************************/
    enum class EVENT_CATEGORY : int
    {
        NONE,
        APPLICATION,
        INPUT,
        KEYBOARD,
        MOUSE,
        MOUSEBUTTON,
    };

    /********************************************************************************//*!
     @brief     typesafe enum that still has normal enum operations.
    *//*********************************************************************************/
    using EventCategoryType = utility::bitmask<EVENT_CATEGORY>;
    
    #define EVENT_CLASS_CATEGORY(category) \
    virtual EventCategoryType GetCategoryFlag() const override { return category; }

    #define EVENT_CLASS_TYPE(type) \
    static EVENT_TYPE GetStaticType() { return EVENT_TYPE::##type; }\
    virtual EVENT_TYPE GetEventType() const override { return GetStaticType(); }\
    virtual const char* GetName() const override { return #type; }

    /********************************************************************************//*!
     @brief     Describes a basic event.
    *//*********************************************************************************/
    class Event 
    {
    public:
        bool Handled = false;

        virtual EVENT_TYPE GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual EventCategoryType GetCategoryFlag() const = 0;
        virtual std::string ToString() const { return GetName(); }

        inline bool IsInCategory(EVENT_CATEGORY categories)
        {
            return GetCategoryFlag() & categories;
        }

    };

    /********************************************************************************//*!
     @brief     Describes an event that will handle and invoke any event it encounters.
    *//*********************************************************************************/
    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event)
            : m_event(event)
        {
        }

        template<typename T, typename F>
        bool Dispatch(const F& func)
        {
            if (m_event.GetEventType() == T::GetStaticType())
            {
                m_event.Handled |= func(static_cast<T&>(m_event));
                return true;
            }
            return false;
        }

    private:
        Event& m_event;
    };

    /********************************************************************************//*!
     @brief     Ostream operator overload for outputting events
    *//*********************************************************************************/
    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }

}