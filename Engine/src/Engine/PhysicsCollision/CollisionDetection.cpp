/************************************************************************************//*!
\file           PhysicsUtils.cpp
\project        <PROJECT_NAME>
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           September 2, 2021
\brief          Encapsulates the Collision Detection Interface for Determining
                Collisions between various physics shapes and triggers.
                Stores the resulting manifold if a collision between physics rigidbodies
                are detected.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "CollisionDetection.h"

#include "Components/Colliders.h"

#include "Manifold.h"
#include "Algorithms/PhysicsManifold.h"
#include "Algorithms/Collision.h"

#include "Engine/ECS/WorldManager.h"
#include "Engine/ECS/GameObject.h"

namespace engine
{
    std::map<CollisionDetection::CombinationKey, CollisionDetection::StaticTriggerCollisionFnc> CollisionDetection::m_collisionMap
    {
        { {ColliderType::CIRCLE, ColliderType::CIRCLE}, StaticTrigger2D_CIRCLE_CIRCLE},
        { {ColliderType::CIRCLE, ColliderType::BOX}, StaticTrigger2D_CIRCLE_BOX},
        { {ColliderType::BOX, ColliderType::CIRCLE}, StaticTrigger2D_BOX_CIRCLE},
        { {ColliderType::BOX, ColliderType::BOX}, StaticTrigger2D_BOX_BOX}
    };

    std::map<CollisionDetection::CombinationKey, CollisionDetection::GenerateManifoldFnc> CollisionDetection::m_manifoldMap
    {
        { {ColliderType::CIRCLE, ColliderType::CIRCLE}, StaticCollider2D_CIRCLE_CIRCLE},
        { {ColliderType::CIRCLE, ColliderType::BOX}, StaticCollider2D_CIRCLE_BOX},
        { {ColliderType::BOX, ColliderType::CIRCLE}, StaticCollider2D_BOX_CIRCLE},
        { {ColliderType::BOX, ColliderType::BOX}, StaticCollider2D_BOX_BOX}
    };


    bool CollisionDetection::StaticTrigger2D(Collider2D const& first, Collider2D const& second)
    {
        return m_collisionMap[std::make_pair(first.GetNarrowPhaseCollider(), second.GetNarrowPhaseCollider())](first, second);
    }

    Manifold2D CollisionDetection::StaticCollider2D(Collider2D const& first, Collider2D const& second)
    {
        return m_manifoldMap[std::make_pair(first.GetNarrowPhaseCollider(), second.GetNarrowPhaseCollider())](first, second);
    }




    bool CollisionDetection::StaticTrigger2D_BOX_BOX(Collider2D boxA, Collider2D boxB)
    {
        return Collision::Test2DAABBAABB(
            ColliderUtil::GetGlobalBounds(boxA.GetComponent<BoxCollider2D>(), boxA.GetComponent<Transform3D>())
            , ColliderUtil::GetGlobalBounds(boxB.GetComponent<BoxCollider2D>(), boxB.GetComponent<Transform3D>())
        );
    }

    bool CollisionDetection::StaticTrigger2D_CIRCLE_CIRCLE(Collider2D circleA, Collider2D circleB)
    {
        return Collision::Test2DCircleCircle(
            ColliderUtil::GetGlobalBounds(circleA.GetComponent<CircleCollider2D>(), circleA.GetComponent<Transform3D>())
            , ColliderUtil::GetGlobalBounds(circleB.GetComponent<CircleCollider2D>(), circleB.GetComponent<Transform3D>()));
    }

    bool CollisionDetection::StaticTrigger2D_CIRCLE_BOX(Collider2D circleA, Collider2D boxB)
    {

        return Collision::Test2DCircleAABB(
            ColliderUtil::GetGlobalBounds(circleA.GetComponent<CircleCollider2D>(), circleA.GetComponent<Transform3D>())
            , ColliderUtil::GetGlobalBounds(boxB.GetComponent<BoxCollider2D>(), boxB.GetComponent<Transform3D>()));
    }

    bool CollisionDetection::StaticTrigger2D_BOX_CIRCLE(Collider2D boxA, Collider2D circleB)
    {
        return Collision::Test2DCircleAABB(
            ColliderUtil::GetGlobalBounds(circleB.GetComponent<CircleCollider2D>(), circleB.GetComponent<Transform3D>())
            , ColliderUtil::GetGlobalBounds(boxA.GetComponent<BoxCollider2D>(), boxA.GetComponent<Transform3D>()));
    }



    Manifold2D CollisionDetection::StaticCollider2D_BOX_BOX(Collider2D boxA, Collider2D boxB)
    {
        auto& boxABounds = ColliderUtil::GetGlobalBounds(boxA.GetComponent<BoxCollider2D>(), boxA.GetComponent<Transform3D>());
        auto& boxBBounds = ColliderUtil::GetGlobalBounds(boxB.GetComponent<BoxCollider2D>(), boxB.GetComponent<Transform3D>());
        return PhysicsManifold::Generate(boxABounds, boxBBounds);
    }

    Manifold2D CollisionDetection::StaticCollider2D_CIRCLE_CIRCLE(Collider2D circleA, Collider2D circleB)
    {
        auto& circleABounds = ColliderUtil::GetGlobalBounds(circleA.GetComponent<CircleCollider2D>(), circleA.GetComponent<Transform3D>());
        auto& circleBBounds = ColliderUtil::GetGlobalBounds(circleB.GetComponent<CircleCollider2D>(), circleB.GetComponent<Transform3D>());
        return PhysicsManifold::Generate(circleABounds, circleBBounds);
    }

    Manifold2D CollisionDetection::StaticCollider2D_CIRCLE_BOX(Collider2D circleA, Collider2D boxB)
    {
        auto& circleABounds = ColliderUtil::GetGlobalBounds(circleA.GetComponent<CircleCollider2D>(), circleA.GetComponent<Transform3D>());
        auto& boxBBounds = ColliderUtil::GetGlobalBounds(boxB.GetComponent<BoxCollider2D>(), boxB.GetComponent<Transform3D>());
        return PhysicsManifold::Generate(circleABounds, boxBBounds);
    }

    Manifold2D CollisionDetection::StaticCollider2D_BOX_CIRCLE(Collider2D boxA, Collider2D circleB)
    {
        auto& boxABounds = ColliderUtil::GetGlobalBounds(boxA.GetComponent<BoxCollider2D>(), boxA.GetComponent<Transform3D>());
        auto& circleBBounds = ColliderUtil::GetGlobalBounds(circleB.GetComponent<CircleCollider2D>(), circleB.GetComponent<Transform3D>());
        return PhysicsManifold::Generate(boxABounds, circleBBounds);
    }

}
