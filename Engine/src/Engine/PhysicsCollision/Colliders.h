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
    //Broadphase collider : Replica of Box Collider stripped to bare minimum
    struct BoundingVolume : public Component
    {
        AABB2D Bounds;
        vec2 Size;

        DEFAULT_COMPONENT(BoundingVolume);

        explicit BoundingVolume(Entity entity, bool active = true)
            : Component {entity, active}
            , Bounds { {-0.5f, -0.5f}, { 0.5f, 0.5f } }
            , Size{ 1.f, 1.f }
        {}
    };

    struct ColliderBase2D : public Component
    {
        /*bool IsTrigger;*/
        vec2 Offset;

        oom::vec2 WorldScale() const;
        oom::vec2 WorldPosition() const;

        explicit ColliderBase2D(Entity entity, bool active = true);
        virtual ~ColliderBase2D();

        DEFAULT_COMPONENT_CONSTRUCTORS(ColliderBase2D);
        RTTR_ENABLE(Component);
    };

    struct CircleCollider2D : public ColliderBase2D
    {
        Circle Bounds;
        float Radius;

        Circle GetGlobalBounds() const;
        float GetGlobalRadius() const;

        explicit CircleCollider2D(Entity entity, bool active = true);
        DEFAULT_COMPONENT(CircleCollider2D);

        RTTR_ENABLE(ColliderBase2D);
    };

    struct BoxCollider2D : public ColliderBase2D
    {
        AABB2D Bounds;
        vec2 Size;

        AABB2D GetGlobalBounds() const;

        vec2 GetWidthAndHeight() const;
        vec2 GetCentroid() const;

        explicit BoxCollider2D(Entity entity, bool active = true);
        DEFAULT_COMPONENT(BoxCollider2D);

        RTTR_ENABLE(ColliderBase2D);
    };

    /*struct PlaneCollider2D 
    {
        AABB2D GetGlobalBounds() const;

        explicit BoxCollider2D(Entity entity, bool active = true);

        BoxCollider2D() = delete;
        BoxCollider2D(BoxCollider2D const&) = default;
        BoxCollider2D(BoxCollider2D&&) = default;
        BoxCollider2D& operator=(BoxCollider2D const&) = default;
        BoxCollider2D& operator=(BoxCollider2D&&) = default;
        virtual ~BoxCollider2D() = default;

        RTTR_ENABLE();
    };*/
}