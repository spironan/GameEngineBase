#pragma once

#include "shapes.h"
#include "Engine/ECS/Component.h"

namespace engine
{
    /*struct ColliderBase2D
    {
        bool IsTrigger;
        vec2 Offset;
        Transform3D const& Transform;

        explicit ColliderBase2D(Transform3D const& transform);

        ColliderBase2D()                                    = delete;
        ColliderBase2D(ColliderBase2D const&)               = default;
        ColliderBase2D(ColliderBase2D&&)                    = default;
        ColliderBase2D& operator=(ColliderBase2D const&)    = default;
        ColliderBase2D& operator=(ColliderBase2D&&)         = default;
        virtual ~ColliderBase2D()                           = default;

        glm::vec2 WorldScale() const;
        glm::vec2 WorldPosition() const;
    };*/
    
    struct CircleCollider2D : public Component/*: public ColliderBase2D*/
    {
        Circle Bounds;
        float Radius;

        explicit CircleCollider2D(Entity entity, bool active = true/*Transform3D const& transform*/);

        CircleCollider2D()                                      = delete;
        CircleCollider2D(CircleCollider2D const&)               = default;
        CircleCollider2D(CircleCollider2D&&)                    = default;
        CircleCollider2D& operator=(CircleCollider2D const&)    = default;
        CircleCollider2D& operator=(CircleCollider2D&&)         = default;
        virtual ~CircleCollider2D()                             = default;

        Circle GetGlobalBounds() const;
    };

    struct BoxCollider2D : public Component/*: public ColliderBase2D*/
    {
        AABB2D Bounds;
        vec2 Size;

        explicit BoxCollider2D(Entity entity, bool active = true/*Transform3D const& transform*/);

        BoxCollider2D()                                 = delete;
        BoxCollider2D(BoxCollider2D const&)             = default;
        BoxCollider2D(BoxCollider2D&&)                  = default;
        BoxCollider2D& operator=(BoxCollider2D const&)  = default;
        BoxCollider2D& operator=(BoxCollider2D&&)       = default;
        virtual ~BoxCollider2D()                        = default;

        AABB2D GetGlobalBounds() const;
    };

}