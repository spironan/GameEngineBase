/************************************************************************************//*!
\file           MouseCode.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 24, 2021
\brief          Defines the engine mouse codes that are used the input system to map each
                mouse input to a behaviour regardless of what the backend is using.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

namespace engine
{
    typedef enum class MouseCode
    {
        // from glfw3.h
        Button0         = 0,
        Button1         = 1,
        Button2         = 2,
        Button3         = 3,
        Button4         = 4,
        Button5         = 5,
        Button6         = 6,
        Button7         = 7,

        ButtonLast      = Button7,
        ButtonLeft      = Button0,
        ButtonRight     = Button1,
        ButtonMiddle    = Button2,
    } Mouse;

    inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
    {
        os << static_cast<int>(mouseCode);
        return os;
    }

    inline MouseCode& operator++(MouseCode& mouseCode)
    {
        mouseCode = static_cast<MouseCode>(static_cast<int>(mouseCode) + 1);
        return mouseCode;
    }

    inline bool operator<(MouseCode mouseCode, int val)
    {
        return static_cast<int>(mouseCode) < val;
    }

}

// Mouse CODES
#define ENGINE_MOUSE_BUTTON_LEFT    ::engine::Mouse::ButtonLeft
#define ENGINE_MOUSE_BUTTON_RIGHT   ::engine::Mouse::ButtonRight
#define ENGINE_MOUSE_BUTTON_MIDDLE  ::engine::Mouse::ButtonMiddle