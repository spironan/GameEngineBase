#include "pch.h"
#include "ColliderCore.h"

#include "Colliders.h"
#include "Engine/ECS/GameObject.h"


namespace engine
{

    void Collider2D::Update()
    {
        if (IsTrigger)
        {
            if (!m_previous && m_current)
            {
                OnTriggerEnter(m_triggers);
            }
            else if (m_previous && m_current)
            {
                OnTriggerStay(m_triggers);
            }
            else if (m_previous && !m_current)
            {
                OnTriggerExit(m_triggers);
            }
            m_triggers.clear();
        }
        else
        {
            if (!m_previous && m_current)
            {
                OnCollisionEnter(m_collisions);
            }
            else if (m_previous && m_current)
            {
                OnCollisionStay(m_collisions);
            }
            else if (m_previous && !m_current)
            {
                OnCollisionExit(m_collisions);
            }
            m_collisions.clear();
        }
        m_previous = m_current;
        m_current = false;
    }

    Collider2D::Collider2D(Entity entity, bool active)
        : Component{ entity, active }
        //, collider{ BoxCollider2D{ GetComponent<Transform3D>() } }
    {
    };

    glm::vec2 Collider2D::WorldPosition() const
    {
        return GetComponent<Transform3D>().GetGlobalPosition();
    }

    glm::vec2 Collider2D::WorldScale() const
    {
        return GetComponent<Transform3D>().GetGlobalScale();
    }

    void Collider2D::SetNarrowPhaseCollider(ColliderType narrowPhaseCollider)
    {
        if (m_narrowPhaseCollider == narrowPhaseCollider) return;

        /*switch (m_narrowPhaseCollider)
        {
        case ColliderType::BOX: RemoveComponent<BoxCollider2D>();
            break;
        case ColliderType::CIRCLE: RemoveComponent<CircleCollider2D>();
            break;
        };

        m_narrowPhaseCollider = narrowPhaseCollider;

        switch (m_narrowPhaseCollider)
        {
        case ColliderType::BOX: AddComponent<BoxCollider2D>();
            break;
        case ColliderType::CIRCLE: AddComponent<CircleCollider2D>();
            break;
        };*/
    }
}
