#pragma once

#include "ColliderCore.h"

namespace engine
{
    struct PhysicsUtils
    {
        using CombinationKey = std::pair<ColliderType, ColliderType>;
        using StaticTriggerCollisionFnc = bool((*)(Collider2D, Collider2D));
        using GenerateManifoldFnc = Manifold2D((*)(Collider2D, Collider2D));

        // non physics - triggers only test
        static bool TestCollision2D(Collider2D const& first, Collider2D const& second);

        // physics based collision
        static Manifold2D GenerateManifold2D(Collider2D const& first, Collider2D const& second);

    private:
        //Maps
        static std::map<CombinationKey, StaticTriggerCollisionFnc> m_collisionMap;
        static std::map<CombinationKey, GenerateManifoldFnc> m_manifoldMap;

        //helper functions
        // collision detection
        static bool TestCollision2D_BOX_BOX(Collider2D, Collider2D);
        static bool TestCollision2D_CIRCLE_CIRCLE(Collider2D, Collider2D);
        static bool TestCollision2D_CIRCLE_BOX(Collider2D, Collider2D);
        static bool TestCollision2D_BOX_CIRCLE(Collider2D, Collider2D);

        // manifold generation
        static Manifold2D GenerateManifold2D_BOX_BOX(Collider2D, Collider2D);
        static Manifold2D GenerateManifold2D_CIRCLE_CIRCLE(Collider2D, Collider2D);
        static Manifold2D GenerateManifold2D_CIRCLE_BOX(Collider2D, Collider2D);
        static Manifold2D GenerateManifold2D_BOX_CIRCLE(Collider2D, Collider2D);
    };
}
