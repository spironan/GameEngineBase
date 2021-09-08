/************************************************************************************//*!
\file          Colliders.cpp
\project       <PROJECT_NAME>
\author        Chua Teck Lee, c.tecklee, 390008420
\par           email: c.tecklee\@digipen.edu
\date          September 2, 2021
\brief

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

    ColliderBase2D::~ColliderBase2D()
    {
        //EnsureRemove<Collider2D>();
    }

    vec2 ColliderBase2D::WorldPosition() const
    {
        return GetComponent<Transform3D>().GetGlobalPosition();
    }

    vec2 ColliderBase2D::WorldScale() const
    {
        return GetComponent<Transform3D>().GetGlobalScale();
    }


    BoxCollider2D::BoxCollider2D(Entity entity, bool active)
        : ColliderBase2D{ entity, active }
        , Bounds{ {-0.5f, -0.5f}, { 0.5f, 0.5f } }
        , Size{ 1.f, 1.f }
    {
        GetComponent<Collider2D>().SetNarrowPhaseCollider(engine::ColliderType::BOX);
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
    {
        GetComponent<Collider2D>().SetNarrowPhaseCollider(engine::ColliderType::CIRCLE);
    };

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