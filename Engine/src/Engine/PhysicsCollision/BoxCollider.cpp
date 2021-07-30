#include "pch.h"
#include "BoxCollider.h"

#include "Engine/PhysicsCollision/Algorithms/Collision.h"

#include "CircleCollider.h"

#include "Engine/PhysicsCollision/Manifold.h"

#include "Engine/ECS/GameObject.h"

namespace engine
{
    BoxCollider2D::BoxCollider2D(Entity entity, bool active)
        : Collider2D{ entity, active }
        , m_bounds{ {-0.5f, -0.5f}, { 0.5f, 0.5f } }
        , m_size{ 1.f, 1.f }
    {};

    AABB2D BoxCollider2D::GetGlobalBounds() const
    {
        auto worldPos = WorldPosition();
        auto worldScale = WorldScale();
        AABB2D result
        { worldPos + m_bounds.min * worldScale * m_size
        , worldPos + m_bounds.max * worldScale * m_size
        };

        return result;
    }

    Manifold2D BoxCollider2D::TestCollision(Collider2D const* collider) const
    {
        return collider->TestCollision(this);
    }

    Manifold2D BoxCollider2D::TestCollision(BoxCollider2D const* boxCollider2D) const
    {
        auto aabb = this->GetGlobalBounds();
        auto aabb2 = boxCollider2D->GetGlobalBounds();

        Manifold2D result
        {
            static_cast<GameObject>(GetEntity()).GetComponent<Rigidbody2D>(),
            static_cast<GameObject>(boxCollider2D->GetEntity()).GetComponent<Rigidbody2D>(),
            { aabb.max.x - aabb2.min.x, aabb.max.y - aabb2.min.y },
            { aabb2.min.x - aabb.max.x, aabb2.min.y - aabb.min.y },
            glm::normalize(result.B - result.A),
            (result.B - result.A).length(),
            Collision::Test2DAABBAABB(aabb, aabb2)
        };
        
        return result;
    }

    //Manifold2D BoxCollider2D::TestCollision(CircleCollider2D const* circleCollider2D) const
    //{
    //    //return circleCollider2D->TestCollision(this);
    //}
}
