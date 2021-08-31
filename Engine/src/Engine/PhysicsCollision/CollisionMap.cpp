#include "pch.h"
#include "CollisionMap.h"

#include "Engine/ECS/GameObject.h"

#include "Manifold.h"
#include "Algorithms/PhysicsCollision.h"
#include "Algorithms/Collision.h"

namespace engine
{
    bool CollisionMap::TestCollision2D(Collider2D const& first, Collider2D const& second)
    {
        switch (first.GetNarrowPhaseCollider())
        {
        case ColliderType::CIRCLE:
        {
            auto const& circle = first.GetComponent<CircleCollider2D>().GetGlobalBounds();

            switch (second.GetNarrowPhaseCollider())
            {
            case ColliderType::CIRCLE:
                return Collision::Test2DCircleCircle(circle, second.GetComponent<CircleCollider2D>().GetGlobalBounds());


            case ColliderType::BOX:
                return Collision::Test2DCircleAABB(circle, second.GetComponent<BoxCollider2D>().GetGlobalBounds());
            }
        }

        case ColliderType::BOX:
        {
            auto const& box = first.GetComponent<BoxCollider2D>().GetGlobalBounds();
            switch (second.GetNarrowPhaseCollider())
            {
            case ColliderType::BOX:
                return Collision::Test2DAABBAABB(box, second.GetComponent<BoxCollider2D>().GetGlobalBounds());


            case ColliderType::CIRCLE:
                return Collision::Test2DCircleAABB(second.GetComponent<CircleCollider2D>().GetGlobalBounds(), box);

            }
        }
        }
    }

    Manifold2D CollisionMap::TestPhysicsCollision2D(Collider2D const& first, Collider2D const& second)
    {
        switch (first.GetNarrowPhaseCollider())
        {
        case ColliderType::CIRCLE:
            switch (second.GetNarrowPhaseCollider())
            {
            case ColliderType::CIRCLE:
                return PhysicsCollision::Test2DCollision(first.GetComponent<CircleCollider2D>(), second.GetComponent<CircleCollider2D>());

            case ColliderType::BOX:
                return PhysicsCollision::Test2DCollision(first.GetComponent<CircleCollider2D>(), second.GetComponent<BoxCollider2D>());
            }
        case ColliderType::BOX:
            switch (second.GetNarrowPhaseCollider())
            {
            case ColliderType::CIRCLE:
                return PhysicsCollision::Test2DCollision(first.GetComponent<BoxCollider2D>(), second.GetComponent<CircleCollider2D>());

            case ColliderType::BOX:
                return PhysicsCollision::Test2DCollision(first.GetComponent<BoxCollider2D>(), second.GetComponent<BoxCollider2D>());
            }
        }
    }

}
