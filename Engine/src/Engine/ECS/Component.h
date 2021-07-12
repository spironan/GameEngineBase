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

        Component(Entity entity, bool active = true) : m_entity(entity), m_active(active) {}

        constexpr Entity GetEntity()       const { return m_entity; }
        constexpr bool IsActive()    const { return m_active; }

        void SetActive(bool _active) { m_active = _active; }

    protected:
        Entity m_entity;
        bool m_active;
    };


	

}