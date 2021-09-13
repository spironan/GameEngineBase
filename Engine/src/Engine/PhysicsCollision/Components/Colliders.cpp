/************************************************************************************//*!
\file           Colliders.cpp
\project        <PROJECT_NAME>
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           September 2, 2021
\brief          Describes the various collider components that are supported.
                Take note not all colliders should be exposed. Some are only for physics
                to take control of.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "Colliders.h"

#include "Engine/ECS/GameObject.h"

#include "Engine/ECS/WorldManager.h"
#include "ColliderCore.h"

#include <rttr/registration>

namespace engine
{
    RTTR_REGISTRATION
    {
        using namespace rttr;

        /*registration::class_<ColliderBase2D>("ColliderBase2D")
            .property("Offset", &ColliderBase2D::Offset);*/

        registration::class_<BoxCollider2D>("BoxCollider2D")
            .property("Offset", &BoxCollider2D::Offset)
            .property("Bounds", &BoxCollider2D::Bounds)
            .property("Size", &BoxCollider2D::Size);
        
        registration::class_<CircleCollider2D>("CircleCollider2D")
            .property("Offset", &CircleCollider2D::Offset)
            .property("Bounds", &CircleCollider2D::Bounds)
            .property("Radius", &CircleCollider2D::Radius);
        
    }
    
    ColliderBase2D::ColliderBase2D(Entity entity, bool active)
        : Component{ entity, active }
        /*, IsTrigger{ false }*/
        , Offset{ 0.f, 0.f }
    {
        EnsureComponent<Collider2D>();
    }

    BoxCollider2D::BoxCollider2D(Entity entity, bool active)
        : ColliderBase2D{ entity, active }
        , Bounds{ {-0.5f, -0.5f}, { 0.5f, 0.5f } }
        , Size{ 1.f, 1.f }
    {
        GetComponent<Collider2D>().SetNarrowPhaseCollider(engine::ColliderType::BOX);
    };

    CircleCollider2D::CircleCollider2D(Entity entity, bool active)
        : ColliderBase2D{ entity, active }
        , Bounds{ { 0.f, 0.f }, 1.f }
        , Radius{ 0.5f }
    {
        GetComponent<Collider2D>().SetNarrowPhaseCollider(engine::ColliderType::CIRCLE);
    };


    AABB2D ColliderUtil::GetGlobalBounds(BoxCollider2D const& boxCollider, Transform3D const& transform)
    {
        oom::vec2 worldPos = transform.GetGlobalPosition();
        oom::vec2 worldScale = transform.GetGlobalScale();
        return AABB2D{ worldPos + boxCollider.Bounds.min * worldScale * boxCollider.Size,
                        worldPos + boxCollider.Bounds.max * worldScale * boxCollider.Size };
    }

    Circle ColliderUtil::GetGlobalBounds(CircleCollider2D const& circleCollider, Transform3D const& transform)
    {
        return Circle
        { oom::vec2{ transform.GetGlobalPosition() } + circleCollider.Bounds.center * circleCollider.Offset
        , GetGlobalRadius(circleCollider, transform)
        };
    }

    AABB2D ColliderUtil::GetGlobalBounds(BoundingVolume const& broadCollider, Transform3D const& transform)
    {
        oom::vec2 worldPos = transform.GetGlobalPosition();
        oom::vec2 worldScale = transform.GetGlobalScale();
        return AABB2D{ worldPos + broadCollider.Bounds.min * worldScale * broadCollider.Size,
                        worldPos + broadCollider.Bounds.max * worldScale * broadCollider.Size };
    }

    vec2 ColliderUtil::GetGlobalDimensions(BoxCollider2D const& boxCollider, Transform3D const& transform)
    {
        auto result = GetGlobalBounds(boxCollider, transform);
        return result.max - result.min;
    }

    float ColliderUtil::GetGlobalRadius(CircleCollider2D const& circleCollider, Transform3D const& transform)
    {
        auto worldScale = transform.GetGlobalScale();
        float scalar = worldScale.x >= worldScale.y ? worldScale.x : worldScale.y;
        return circleCollider.Bounds.radius * circleCollider.Radius * scalar;
    }

    
}