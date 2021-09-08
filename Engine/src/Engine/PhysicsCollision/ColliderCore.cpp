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

    void CollisionInfo::Update()
    {
        if (GetComponent<Collider2D>().IsTrigger)
        {
            for (auto const& [entity, trigger] : m_triggers)
            {
                // if previously not collided with this object
                if (m_previousTriggers.find(entity) == m_previousTriggers.end())
                    OnTriggerEnter(trigger);
                else
                    OnTriggerStay(trigger);
            }

            for (auto const& [entity, prevTrigger] : m_previousTriggers)
            {
                // if currently not trigger with this object
                if (m_triggers.find(entity) == m_triggers.end())
                    OnTriggerExit(prevTrigger);
            }

            m_previousTriggers.clear();
            m_previousTriggers = std::move(m_triggers);
            m_triggers.clear();
        }
        else
        {
            for (auto const& [entity, collider] : m_collisions)
            {
                // if previously not collided with this object
                if (m_previousCollisions.find(entity) == m_previousCollisions.end())
                    OnCollisionEnter(collider);
                else
                    OnCollisionStay(collider);
            }

            for (auto const& [entity, prevTrigger] : m_previousCollisions)
            {
                // if currently not colliding with this object
                if (m_collisions.find(entity) == m_collisions.end())
                    OnCollisionExit(prevTrigger);
            }

            m_previousCollisions.clear();
            m_previousCollisions = std::move(m_collisions);
            m_collisions.clear();
        }
    }


    Collider2D::Collider2D(Entity entity, bool active)
        : Component{ entity, active }
    {
        EnsureComponent<CollisionInfo>();
    };

    Collider2D::~Collider2D()
    {
        //EnsureRemove<CollisionInfo>();
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
