#pragma once

#include "PhysicsFwd.h"

namespace engine
{

    // considering whether to use contact points or not.
    // More then 1 contact point is only useful in complex physics
    struct Contact2D
    {
        vec2 Position = vec2{ 0 };
        vec2 Normal = vec2{ 0 };
        float PenetrationDepth = 0.f;
    };

    struct Manifold2D
    {
        Rigidbody2D* ObjA = nullptr;
        Rigidbody2D* ObjB = nullptr;
        //vec2 A; // furthest point of A into B
        //vec2 B; // furthest point of B into A

        vec2 Position = vec2{ 0 }; // Position of Collision
        vec2 Normal = vec2{ 0 }; // B - A normalized
        float PenetrationDepth = 0.f; // Length of B - A
        bool HasCollision = false;
    };

}
