#pragma once
#include "ECS_Utility.h"

namespace engine
{
    class Component
    {
    public:
        Component() = delete;

        Component(const Component&) = default;
        Component& operator=(const Component&) = default;
        virtual ~Component() = default;

        Component(Entity entity, bool active) : m_entity(entity), m_active(active) {}

        Entity GetEntity() const            { return m_entity; }
        void SetEntity(Entity entity)       { m_entity = entity; }
        constexpr bool IsActive() const     { return m_active; }

        void SetActive(bool _active) { m_active = _active; }

        template<typename T, typename T2 = engine::WorldManager>
        T& GetComponent() const
        {
            return T2::GetActiveWorld().GetComponent<T>(m_entity);
        }
        
        template<typename T, typename T2 = engine::WorldManager>
        T* TryGetComponent() const
        {
            return T2::GetActiveWorld().TryGetComponent<T>(m_entity);
        }

        template<typename T, typename T2 = engine::WorldManager>
        bool HasComponent() const
        {
            return T2::GetActiveWorld().HasComponent<T>(m_entity);
        }

    protected:
        Entity m_entity;
        bool m_active;
    };


}