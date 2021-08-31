#include "pch.h"
#include "PhysicsCollision.h"

#include "Collision.h"
#include "Engine/PhysicsCollision/Manifold.h"

#include "Engine/PhysicsCollision/Colliders.h"
#include "Engine/PhysicsCollision/RigidBody.h"

namespace engine
{
    Manifold2D PhysicsCollision::Test2DCollision(CircleCollider2D const& circle, CircleCollider2D const& circle2)
    {
        Manifold2D result {};

        auto circleA = circle.GetGlobalBounds();
        auto circleB = circle2.GetGlobalBounds();

        if (!Collision::Test2DCircleCircle(circleA, circleB)) return result;

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

    Manifold2D PhysicsCollision::Test2DCollision(CircleCollider2D const& circle, BoxCollider2D const& aabb)
    {
        return Manifold2D{};
    }

    Manifold2D PhysicsCollision::Test2DCollision(BoxCollider2D const& aabb, CircleCollider2D const& circle)
    {
        return Manifold2D{};
    }

    Manifold2D PhysicsCollision::Test2DCollision(BoxCollider2D const& aabb, BoxCollider2D const& aabb2)
    {
        return Manifold2D{};
    }

}