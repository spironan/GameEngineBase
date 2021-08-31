#pragma once

#include "Engine/PhysicsCollision/Colliders.h"

namespace engine
{
    struct PhysicsCollision
    {
        static Manifold2D Test2DCollision(CircleCollider2D const& circle, CircleCollider2D const& circle2);
        static Manifold2D Test2DCollision(CircleCollider2D const& circle, BoxCollider2D const& aabb);
        static Manifold2D Test2DCollision(BoxCollider2D const& aabb, CircleCollider2D const& circle);
        static Manifold2D Test2DCollision(BoxCollider2D const& aabb, BoxCollider2D const& aabb2);
    };

}