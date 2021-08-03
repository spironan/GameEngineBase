#include "pch.h"
#include "Engine/ECS/WorldManager.h"
#include "CircleCollider.h"

#include "Engine/PhysicsCollision/Algorithms/Collision.h"

#include "BoxCollider.h"
#include "Manifold.h"
#include "Engine/PhysicsCollision/RigidBody.h"

namespace engine
{
    CircleCollider2D::CircleCollider2D(Entity entity, bool active)
        : Collider2D{ entity, active }
        , m_bounds{ { 0.f, 0.f }, 1.f }
        , m_radius{ 0.5f }
    {};
    
    Circle CircleCollider2D::GetGlobalBounds() const
    {
        vec2 worldScale = WorldScale();
        float scalar = worldScale.x >= worldScale.y ? worldScale.x : worldScale.y;

        return Circle
        { WorldPosition() + m_bounds.center * Offset
        , m_bounds.radius * m_radius * scalar
        };
    }

    Manifold2D CircleCollider2D::TestCollision(Collider2D const* collider) const
    {
        return collider->TestCollision(this);
    }

    Manifold2D CircleCollider2D::TestCollision(BoxCollider2D const* boxCollider2D) const
    {
        return {};
        //return Collision::Test2DCircleAABB(this->GetGlobalBounds(), boxCollider2D->GetGlobalBounds());
    }

    Manifold2D CircleCollider2D::TestCollision(CircleCollider2D const* circleCollider2D) const
    {
        auto circleA = this->GetGlobalBounds();
        auto circleB = circleCollider2D->GetGlobalBounds();

        if(!Collision::Test2DCircleCircle(circleA, circleB)) return {};

        Manifold2D result
        {
            TryGetComponent<Rigidbody2D>(),
            circleCollider2D->TryGetComponent<Rigidbody2D>(),
        };

        
        vec2 dirVec = circleB.center - circleA.center;
        // find distance
        float dist = glm::length(dirVec);

        // if the two circles are placed onto each other
        if (dist == 0.f)
        {
            // Choose random (but consistent) values
            result.PenetrationDepth = circleA.radius;
            result.Normal = vec2{ 1, 0 };
            result.Position = circleA.center;
        }
        else
        {
            result.PenetrationDepth = (circleA.radius + circleB.radius) - dist;
            // Utilize our d since we performed sqrt on it already
            result.Normal = dirVec / dist;

            // Take A's position and move it along the contact normal
            // the distance of A's radius
            //c->position = c->normal * A->r + a->pos;
            result.Position = result.Normal * circleA.radius + circleA.center;
        }

        result.HasCollision = true;

        return result;
    }
}
