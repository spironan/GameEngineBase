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
    
    class CollisionInfo : public Component
    {
    public:
        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        DEFAULT_COMPONENT(CollisionInfo);

        explicit CollisionInfo(Entity entity, bool active = true);

        /*-----------------------------------------------------------------------------*/
        /* Supported Event Callbacks                                                   */
        /*-----------------------------------------------------------------------------*/
        EventCallback<Manifold2D> OnCollisionEnter;
        EventCallback<Manifold2D> OnCollisionStay;
        EventCallback<Manifold2D> OnCollisionExit;

        EventCallback<Collider2D> OnTriggerEnter;
        EventCallback<Collider2D> OnTriggerStay;
        EventCallback<Collider2D> OnTriggerExit;
    
        /*-----------------------------------------------------------------------------*/
        /* Collision Maps                                                              */
        /*-----------------------------------------------------------------------------*/
        std::unordered_map<Entity, Manifold2D> m_previousCollisions{};
        std::unordered_map<Entity, Collider2D> m_previousTriggers{};
        std::unordered_map<Entity, Manifold2D> m_collisions{};
        std::unordered_map<Entity, Collider2D> m_triggers{};
    };

    class Collider2D : public Component
    {
    public:
        bool IsTrigger = false; // for now the entire object is either a trigger or collider

        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        explicit Collider2D(Entity entity, bool active = true);
        virtual ~Collider2D();
        DEFAULT_COMPONENT_CONSTRUCTORS(Collider2D);

        // third attempt : double components - function map collision
        ColliderType GetBroadPhaseCollider() const { return m_broadphaseCollider; }
        ColliderType GetNarrowPhaseCollider() const { return m_narrowPhaseCollider; }

        void SetNarrowPhaseCollider(ColliderType narrowPhaseCollider);

        RTTR_ENABLE(Component);

    private:
        // third attempt : double components - function map collision
        ColliderType m_broadphaseCollider = ColliderType::BOX;
        ColliderType m_narrowPhaseCollider = ColliderType::CIRCLE;

    };

    extern "C"
    {
        __declspec(dllexport) bool Collider2D_GetIsTriggered(Entity instanceID);
        __declspec(dllexport) void Collider2D_SetIsTriggered(Entity instanceID, bool value);
    }
}
