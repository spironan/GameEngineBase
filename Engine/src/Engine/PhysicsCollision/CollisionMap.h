#pragma once

#include "ColliderCore.h"

namespace engine
{
    struct CollisionMap
    {
        // non physics - triggers only test
        static bool TestCollision2D(Collider2D const& first, Collider2D const& second);

        // physics based collision
        static Manifold2D TestPhysicsCollision2D(Collider2D const& first, Collider2D const& second);
    };
}
