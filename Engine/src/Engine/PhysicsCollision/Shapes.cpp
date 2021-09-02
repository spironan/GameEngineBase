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
