/************************************************************************************//*!
\file          Colliders.h
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
#pragma once

#include "shapes.h"
#include "Engine/ECS/Component.h"
#include "Engine/Transform/Transform3D.h"

#include <rttr/type>

namespace engine
{
    struct ColliderBase2D : public Component
    {
        /*bool IsTrigger;*/
        vec2 Offset;

        oom::vec2 WorldScale() const;
        oom::vec2 WorldPosition() const;

        explicit ColliderBase2D(Entity entity, bool active = true);

        ColliderBase2D()                                    = delete;
        ColliderBase2D(ColliderBase2D const&)               = default;
        ColliderBase2D(ColliderBase2D&&)                    = default;
        ColliderBase2D& operator=(ColliderBase2D const&)    = default;
        ColliderBase2D& operator=(ColliderBase2D&&)         = default;
        virtual ~ColliderBase2D()                           = default;

    };
    
    struct CircleCollider2D : public ColliderBase2D
    {
        Circle Bounds;
        float Radius;

        Circle GetGlobalBounds() const;

        explicit CircleCollider2D(Entity entity, bool active = true);

        CircleCollider2D()                                      = delete;
        CircleCollider2D(CircleCollider2D const&)               = default;
        CircleCollider2D(CircleCollider2D&&)                    = default;
        CircleCollider2D& operator=(CircleCollider2D const&)    = default;
        CircleCollider2D& operator=(CircleCollider2D&&)         = default;
        virtual ~CircleCollider2D()                             = default;

        RTTR_ENABLE();
    };

    struct BoxCollider2D : public ColliderBase2D
    {
        AABB2D Bounds;
        vec2 Size;

        AABB2D GetGlobalBounds() const;

        explicit BoxCollider2D(Entity entity, bool active = true);

        BoxCollider2D()                                 = delete;
        BoxCollider2D(BoxCollider2D const&)             = default;
        BoxCollider2D(BoxCollider2D&&)                  = default;
        BoxCollider2D& operator=(BoxCollider2D const&)  = default;
        BoxCollider2D& operator=(BoxCollider2D&&)       = default;
        virtual ~BoxCollider2D()                        = default;

        RTTR_ENABLE();
    };

}