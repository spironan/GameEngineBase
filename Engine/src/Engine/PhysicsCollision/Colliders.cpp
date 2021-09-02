#include "pch.h"
#include "Colliders.h"

#include "Engine/ECS/WorldManager.h"
#include "ColliderCore.h"

namespace engine
{
    ColliderBase2D::ColliderBase2D(Entity entity, bool active)
        : Component{ entity, active }
        /*, IsTrigger{ false }*/
        , Offset{ 0.f, 0.f }
    {}

    glm::vec2 ColliderBase2D::WorldPosition() const
    {
        return GetComponent<Transform3D>().GetGlobalPosition();
    }

    glm::vec2 ColliderBase2D::WorldScale() const
    {
        return GetComponent<Transform3D>().GetGlobalScale();
    }

    BoxCollider2D::BoxCollider2D(Entity entity, bool active)
        : ColliderBase2D{ entity, active }
        , Bounds{ {-0.5f, -0.5f}, { 0.5f, 0.5f } }
        , Size{ 1.f, 1.f }
    {
    };

    AABB2D BoxCollider2D::GetGlobalBounds() const
    {
        auto worldPos = WorldPosition();
        auto worldScale = WorldScale();
        AABB2D result
        { worldPos + Bounds.min * worldScale * Size
        , worldPos + Bounds.max * worldScale * Size
        };

        return result;
    }

    CircleCollider2D::CircleCollider2D(Entity entity, bool active/*Transform3D const& transform*/)
        : ColliderBase2D{ entity, active }
        , Bounds{ { 0.f, 0.f }, 1.f }
        , Radius{ 0.5f }
    {};

    Circle CircleCollider2D::GetGlobalBounds() const
    {
        vec2 worldScale = WorldScale();
        float scalar = worldScale.x >= worldScale.y ? worldScale.x : worldScale.y;

        return Circle
        { WorldPosition() + Bounds.center * Offset
        , Bounds.radius * Radius * scalar
        };
    }

}