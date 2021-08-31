#include "pch.h"
#include "Collider.h"

#include "Colliders.h"
#include "Engine/ECS/GameObject.h"

#include "Manifold.h"
#include "Algorithms/PhysicsCollision.h"

namespace engine
{

    Manifold2D CollisionMap::TestCollision2D(Collider2D const& first, Collider2D const& second)
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


    Collider2D::Collider2D(Entity entity, bool active)
        : Component{ entity, active }
        , IsTrigger{ false }
        , Offset{ 0, 0 }
        //, collider{ BoxCollider2D{ GetComponent<Transform3D>() } }
        //, collider{}
    {
    };

    glm::vec2 Collider2D::WorldPosition() const
    {
        return GetComponent<Transform3D>().GetGlobalPosition();
    }

    glm::vec2 Collider2D::WorldScale() const
    {
        return GetComponent<Transform3D>().GetGlobalScale();
    }

    void Collider2D::SetNarrowPhaseCollider(ColliderType narrowPhaseCollider)
    {
        if (m_narrowPhaseCollider == narrowPhaseCollider) return;

        /*switch (m_narrowPhaseCollider)
        {
        case ColliderType::BOX: RemoveComponent<BoxCollider2D>();
            break;
        case ColliderType::CIRCLE: RemoveComponent<CircleCollider2D>();
            break;
        };

        m_narrowPhaseCollider = narrowPhaseCollider;

        switch (m_narrowPhaseCollider)
        {
        case ColliderType::BOX: AddComponent<BoxCollider2D>();
            break;
        case ColliderType::CIRCLE: AddComponent<CircleCollider2D>();
            break;
        };*/
    }
}
