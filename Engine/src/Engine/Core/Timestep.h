/************************************************************************************//*!
\file           Timestep.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 24, 2021
\brief          Describes how much time had changed from one iteration to the next.
                AKA DeltaTime.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

namespace engine
{
    /********************************************************************************//*!
     @brief     Describes how much time had changed from one iteration to the next.
                AKA DeltaTime.
    *//*********************************************************************************/
    class Timestep final
    {
    public:
        Timestep(double time = 0.f)
            : m_time(time)
        {
        }

        operator double() const { return m_time; }

        double GetSeconds() const { return m_time * 0.001; }
        double GetMilliSeconds() const { return m_time; }

    private:
        // time in milliseconds.
        double m_time;
    };
}
