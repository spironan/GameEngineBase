/************************************************************************************//*!
\file           Shapes.cpp
\project        <PROJECT_NAME>
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           September 2, 2021
\brief          Shapes contains the basic data that support the fundamental piece of
                geometry used in physics engine.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"

#include "Shapes.h"

#include <rttr/registration>

namespace engine
{
    RTTR_REGISTRATION
    {
        using namespace rttr;
        registration::class_<Plane2D>("Plane2D")
            .property("Normal", &Plane2D::normal)
            .property("Distance", &Plane2D::dist);
        
        registration::class_<AABB2D>("AABB2D")
            .property("Min", &AABB2D::min)
            .property("Max", &AABB2D::max);

        registration::class_<Circle>("Circle")
            .property("Center", &Circle::center)
            .property("Radius", &Circle::radius);
    }

    Plane2D::Plane2D(vec2 normal, float dist)
    : normal{ normal }
    , dist{ dist }
    {
    }

    AABB2D::AABB2D(vec2 min, vec2 max)
        : min{ min }
        , max{ max }
    {
    }

    Circle::Circle(vec2 center, float radius)
        : center{ center }
        , radius{ radius }
    {
    }
}
