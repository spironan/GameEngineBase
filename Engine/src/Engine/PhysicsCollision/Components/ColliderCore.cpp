/************************************************************************************//*!
\file          ColliderCore.cpp
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
#include "ColliderCore.h"

#include "Colliders.h"
#include "Engine/ECS/GameObject.h"

#include "Engine/Scripting/Scripting.h"

#include <rttr/registration>

namespace engine
{
    RTTR_REGISTRATION
    {
        using namespace rttr;
        registration::class_<Collider2D>("Collider2D")
            .property("IsTrigger", &Collider2D::IsTrigger);
    }

    CollisionInfo::CollisionInfo(Entity entity, bool active)
        : Component{ entity, active }
    {
        // Register event callbacks
        OnTriggerEnter += [this](const auto& trigger)
        {
            Scripting& scripting = GetComponent<Scripting>();
            Scripting& other = GameObject(trigger.GetEntity()).GetComponent<Scripting>();
            scripting.InvokeTriggerEnter2D(other);
        };
        OnTriggerStay += [this](const auto& trigger)
        {
            Scripting& scripting = GetComponent<Scripting>();
            Scripting& other = GameObject(trigger.GetEntity()).GetComponent<Scripting>();
            scripting.InvokeTriggerStay2D(other);
        };
        OnTriggerExit += [this](const auto& trigger)
        {
            Scripting& scripting = GetComponent<Scripting>();
            Scripting& other = GameObject(trigger.GetEntity()).GetComponent<Scripting>();
            scripting.InvokeTriggerExit2D(other);
        };
    }

    Collider2D::Collider2D(Entity entity, bool active)
        : Component{ entity, active }
    {
        EnsureComponent<CollisionInfo>();   // collision callbacks
        EnsureComponent<BoundingVolume>();  // broadphase collider
    };

    Collider2D::~Collider2D()
    {
        //EnsureRemove<CollisionInfo>();
        //EnsureRemove<BoundingVolume>();
    }

    void Collider2D::SetNarrowPhaseCollider(ColliderType narrowPhaseCollider)
    {
        if (m_narrowPhaseCollider == narrowPhaseCollider) return;
        //remove to achieve one collider per objet
        switch (m_narrowPhaseCollider)
        {
        case ColliderType::BOX: TryRemoveComponent<BoxCollider2D>();
            break;
        case ColliderType::CIRCLE: TryRemoveComponent<CircleCollider2D>();
            break;
        };

        m_narrowPhaseCollider = narrowPhaseCollider;
        
        // no need to add because the interface to add is via the derived colliders.
    }


    bool Collider2D_GetIsTriggered(Entity instanceID)
    {
        GameObject obj{ instanceID };
        return obj.GetComponent<Collider2D>().IsTrigger;
    }

    void Collider2D_SetIsTriggered(Entity instanceID, bool value)
    {
        GameObject obj{ instanceID };
        obj.GetComponent<Collider2D>().IsTrigger = value;
    }
}
