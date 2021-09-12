/************************************************************************************//*!
\file          PhysicsUtils.h
\project       <PROJECT_NAME>
\author        Chua Teck Lee, c.tecklee, 390008420
\par           email: c.tecklee\@digipen.edu
\date          September 2, 2021
\brief         

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "ColliderCore.h"
#include "colliders.h"

namespace engine
{
    struct PhysicsUtils
    {
        using CombinationKey = std::pair<ColliderType, ColliderType>;
        using StaticTriggerCollisionFnc = bool((*)(Collider2D, Collider2D));
        using GenerateManifoldFnc = Manifold2D((*)(Collider2D, Collider2D));

        //static void ResolveStaticCollisions(Collider2D const& first, Collider2D const& second);

        // non physics - triggers only test
        static bool TestCollision2D(Collider2D const& first, Collider2D const& second);

        // physics based collision
        static Manifold2D GenerateManifold2D(Collider2D const& first, Collider2D const& second);

        // MakeCollider
        static AABB2D MakeCollider(BoundingVolume const& broadCollider, Transform3D const& transform, Collider2D const& collider);

    private:

        //Collision Maps
        //static std::map<Entity, std::map<Entity, Collider2D>> S_PreviousTriggers;
        //static std::map<Entity, std::map<Entity, Collider2D>> S_CurrentTriggers;
        //static std::map<Entity, std::map<Entity, Manifold2D>> S_PreviousCollisions;
        //static std::map<Entity, std::map<Entity, Manifold2D>> S_CurrentCollisions;

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
