#include "pch.h"
#include "Collider.h"

#include "Engine/ECS/GameObject.h"

namespace engine
{
    Collider2D::Collider2D(Entity entity, bool active)
        : Component{ entity, active }
        , IsTrigger{ false }
        , Offset{ 0, 0 }
    {
    };

    glm::vec2 Collider2D::WorldPosition() const
    {
        return static_cast<GameObject>(GetEntity()).Transform().GetGlobalPosition();
    }

    glm::vec2 Collider2D::WorldScale() const
    {
        return static_cast<GameObject>(GetEntity()).Transform().GetGlobalScale();
    }
}
