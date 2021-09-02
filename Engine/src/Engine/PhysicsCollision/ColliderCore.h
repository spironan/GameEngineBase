/************************************************************************************//*!
\file          ColliderCore.h
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

#include "Engine/ECS/Component.h"

#include "Shapes.h"
#include "Manifold.h"   // requires full definition for events to compile

#include <rttr/type>

namespace engine
{
    enum class ColliderType
    {
        CIRCLE,
        BOX
    };

    class Collider2D : public Component
    {
    /*public:
        virtual Manifold2D TestCollision(Collider2D const* collider) const = 0;
        virtual Manifold2D TestCollision(BoxCollider2D const* boxCollider2D) const = 0;
        virtual Manifold2D TestCollision(CircleCollider2D const* circleCollider2D) const = 0;*/

        // third attempt : double components - function map collision
        ColliderType m_broadphaseCollider   = ColliderType::BOX;
        ColliderType m_narrowPhaseCollider  = ColliderType::CIRCLE;

        bool m_previous = false, m_current = false;
        std::vector<Manifold2D> m_collisions{};
        std::vector<Collider2D> m_triggers{};
        friend class PhysicsSystem;
        void Update();

    public:
        bool IsTrigger = false; // for now the entire object is either a trigger or collider
        /*vec2 Offset = { 0, 0 };*/

        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        Collider2D()                            = delete;
        Collider2D(Collider2D const&)           = default;
        Collider2D(Collider2D &&)               = default;
        Collider2D& operator=(Collider2D const&)= default;
        Collider2D& operator=(Collider2D &&)    = default;
        virtual ~Collider2D()                   = default;

        explicit Collider2D(Entity entity, bool active = true);

        /*-----------------------------------------------------------------------------*/
        /* Supported Event Callbacks                                                   */
        /*-----------------------------------------------------------------------------*/
        
        EventCallback<std::vector<Manifold2D>> OnCollisionEnter;
        EventCallback<std::vector<Manifold2D>> OnCollisionStay;
        EventCallback<std::vector<Manifold2D>> OnCollisionExit;

        EventCallback<std::vector<Collider2D>> OnTriggerEnter;
        EventCallback<std::vector<Collider2D>> OnTriggerStay;
        EventCallback<std::vector<Collider2D>> OnTriggerExit;
        

        /*-----------------------------------------------------------------------------*/
        /* Supported Event Callbacks                                                   */
        /*-----------------------------------------------------------------------------*/

        // third attempt : double components - function map collision
        ColliderType GetBroadPhaseCollider() const { return m_broadphaseCollider; }
        ColliderType GetNarrowPhaseCollider() const { return m_narrowPhaseCollider; }

        void SetNarrowPhaseCollider(ColliderType narrowPhaseCollider);

        /*Manifold2D TestCollision(Collider2D other) const
        {
            return CollisionMap::TestCollision(*this, other);
        }*/

        // second attempt : using std::variant for collision
        //std::variant<BoxCollider2D, CircleCollider2D> collider;

        /* original first attempt: inheritence double dispatch
        virtual void OnTriggerEnter(Collider2D collider) {};
        virtual void OnTriggerStay(Collider2D collider) {};
        virtual void OnTriggerExit(Collider2D collider) {};*/

        RTTR_ENABLE();
    };
}
