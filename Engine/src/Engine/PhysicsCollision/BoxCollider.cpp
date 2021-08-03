#include "pch.h"
#include "BoxCollider.h"

#include "Engine/PhysicsCollision/Algorithms/Collision.h"

//#include "CircleCollider.h"

#include "Engine/PhysicsCollision/Manifold.h"

#include "Engine/ECS/GameObject.h"

#include "Engine/PhysicsCollision/RigidBody.h"

namespace engine
{
    BoxCollider2D::BoxCollider2D(Entity entity, bool active)
        : Collider2D{ entity, active }
        , m_bounds { {-0.5f, -0.5f}, { 0.5f, 0.5f } }
        , m_size { 1.f, 1.f }
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

        if (!Collision::Test2DAABBAABB(aabb, aabb2))
        {
            return Manifold2D{};    // empty manifold, no collision.
        }


        Manifold2D result
        {
            static_cast<GameObject>(GetEntity()).TryGetComponent<Rigidbody2D>(),
            static_cast<GameObject>(boxCollider2D->GetEntity()).TryGetComponent<Rigidbody2D>(),
            /*{ aabb.max.x - aabb2.min.x, aabb.max.y - aabb2.min.y },
            { aabb2.min.x - aabb.max.x, aabb2.min.y - aabb.min.y },
            glm::normalize(result.B - result.A),
            glm::length(result.B - result.A),
            true*/
        };


        // Calculate half extents along x axis for each object
        vec2 a_extent = (aabb.max - aabb.min) * 0.5f;
        vec2 b_extent = (aabb2.max - aabb2.min) * 0.5f;

        vec2 a_pos = aabb.max - a_extent;
        vec2 b_pos = aabb2.max - b_extent;

        // Vector from A to B
        vec2 n = b_pos - a_pos;

        // Calculate overlap on x axis
        float x_overlap = a_extent.x + b_extent.x - abs(n.x);

        // Calculate overlap on y axis
        float y_overlap = a_extent.y + b_extent.y - abs(n.y);

        // Find out which axis is axis of least penetration
        if (x_overlap < y_overlap)
        {
            // Point towards B knowing that n points from A to B
            result.Normal = n.x < 0 ? vec2{ -1, 0 } : vec2{ 1, 0 };
            result.PenetrationDepth = x_overlap;
            /*if (n.x < 0)
            {
                result.Normal = vec2(-1, 0);
            }
            else
            {
                result.Normal = vec2(1, 0);
                result.PenetrationDepth = x_overlap;
            }*/
        }
        else
        {
            // Point toward B knowing that n points from A to B
            result.Normal = n.y < 0 ? vec2{ 0, -1 } : vec2{ 0, 1 };
            result.PenetrationDepth = y_overlap;
            /*if (n.y < 0)
            {
                result.Normal = vec2(0, -1);
            }
            else
            {
                result.Normal = vec2(0, 1);
                result.PenetrationDepth = y_overlap;
            }*/
        }
        
        result.HasCollision = true;

        return result;
    }

    Manifold2D BoxCollider2D::TestCollision(CircleCollider2D const* circleCollider2D) const
    {
        //return circleCollider2D->TestCollision(this);
        return Manifold2D{};    // empty manifold, not handled yet.
    }

}
