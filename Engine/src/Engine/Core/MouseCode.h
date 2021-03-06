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
    using MouseCode = int;

    namespace mouse 
    {
        enum : MouseCode
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
        };

    }
}