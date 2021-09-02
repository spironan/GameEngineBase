#pragma once

#include "Engine/PhysicsCollision/Colliders.h"

namespace engine
{
    // generate manifold assumes that collision has been detected.
    struct PhysicsCollision
    {
        static Manifold2D GenerateManifold2D(CircleCollider2D const& circle, CircleCollider2D const& circle2);
        static Manifold2D GenerateManifold2D(CircleCollider2D const& circle, BoxCollider2D const& aabb);
        static Manifold2D GenerateManifold2D(BoxCollider2D const& aabb, CircleCollider2D const& circle);
        static Manifold2D GenerateManifold2D(BoxCollider2D const& aabb, BoxCollider2D const& aabb2);
    };

}