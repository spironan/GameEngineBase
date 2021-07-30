#pragma once

#include "PhysicsFwd.h"
#include "Rigidbody.h"

namespace engine
{
    struct Manifold2D
    {
        Rigidbody2D& ObjA;
        Rigidbody2D& ObjB;
        vec2 A; // furthest point of A into B
        vec2 B; // furthest point of B into A
        vec2 Normal; // B - A normalized
        float PenetrationDepth; // Length of B - A
        bool HasCollision;
    };

}
