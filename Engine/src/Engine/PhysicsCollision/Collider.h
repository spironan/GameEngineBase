#pragma once

#include "Engine/ECS/Component.h"

#include "Shapes.h"

namespace engine
{
    class Collider2D : public Component
    {
    public:
        virtual Manifold2D TestCollision(Collider2D const* collider) const = 0;
        virtual Manifold2D TestCollision(BoxCollider2D const* boxCollider2D) const = 0;
        //virtual Manifold2D TestCollision(CircleCollider2D const* circleCollider2D) const = 0;

    public:
        bool IsTrigger;
        vec2 Offset;

        explicit Collider2D(Entity entity, bool active = true);
        
        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        Collider2D()                            = delete;
        Collider2D(Collider2D const&)           = default;
        Collider2D(Collider2D &&)               = default;
        Collider2D& operator=(Collider2D const&)= default;
        Collider2D& operator=(Collider2D &&)    = default;
        virtual ~Collider2D() override          = default;

        glm::vec2 WorldScale() const;
        glm::vec2 WorldPosition() const;

        /*virtual void OnTriggerEnter(Collider2D collider) {};
        virtual void OnTriggerStay(Collider2D collider) {};
        virtual void OnTriggerExit(Collider2D collider) {};*/
    };
}
