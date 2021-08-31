#include "pch.h"
#include "Colliders.h"

#include "Engine/ECS/WorldManager.h"
#include "Collider.h"

namespace engine
{
    /*ColliderBase2D::ColliderBase2D(Transform3D const& transform)
        : Transform{ transform }
        , IsTrigger{ false }
        , Offset{ 0.f, 0.f }
    {}

    glm::vec2 ColliderBase2D::WorldPosition() const
    {
        return Transform.GetGlobalPosition();
    }

    glm::vec2 ColliderBase2D::WorldScale() const
    {
        return Transform.GetGlobalScale();
    }*/

    BoxCollider2D::BoxCollider2D(Entity entity, bool active/*Transform3D const& transform*/)
        //: ColliderBase2D{ transform }
        : Component{ entity, active }
        , Bounds{ {-0.5f, -0.5f}, { 0.5f, 0.5f } }
        , Size{ 1.f, 1.f }
    {
    };

    AABB2D BoxCollider2D::GetGlobalBounds() const
    {
        auto worldPos = GetComponent<Collider2D>().WorldPosition();
        auto worldScale = GetComponent<Collider2D>().WorldScale();
        AABB2D result
        { worldPos + Bounds.min * worldScale * Size
        , worldPos + Bounds.max * worldScale * Size
        };

        return result;
    }

    CircleCollider2D::CircleCollider2D(Entity entity, bool active/*Transform3D const& transform*/)
        //: ColliderBase2D{ transform }
        : Component{ entity, active }
        , Bounds{ { 0.f, 0.f }, 1.f }
        , Radius{ 0.5f }
    {};

    Circle CircleCollider2D::GetGlobalBounds() const
    {
        vec2 worldScale = GetComponent<Collider2D>().WorldScale();
        float scalar = worldScale.x >= worldScale.y ? worldScale.x : worldScale.y;

        return Circle
        { GetComponent<Collider2D>().WorldPosition() + Bounds.center * GetComponent<Collider2D>().Offset
        , Bounds.radius * Radius * scalar
        };
    }

}