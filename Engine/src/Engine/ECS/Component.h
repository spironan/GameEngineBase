#pragma once
#include "ECS_Utility.h"

#include <rttr/type>

namespace engine
{
    class WorldManager;
    class Component
    {
    public:
        Component() = delete;

        Component(const Component&) = default;
        Component& operator=(const Component&) = default;
        virtual ~Component() = default;

        Component(Entity entity, bool active) : m_entity(entity), m_active(active) {}
        virtual Component& CopyComponent(Component const& comp) { return *this = comp; };

        Entity GetEntity() const            { return m_entity; }
        void SetEntity(Entity entity)       { m_entity = entity; }
        constexpr bool IsActive() const     { return m_active; }

        void SetActive(bool _active) { m_active = _active; }

        template<typename Derived, typename T2 = engine::WorldManager, typename... Args>
        std::enable_if_t<std::is_base_of_v<Component, Derived>, Derived& >
        AddComponent(Args... args) const
        {
            return T2::GetActiveWorld().EmplaceComponent<Derived>(m_entity, true, args...);
        }
        
        template<typename NotDerived, typename T2 = engine::WorldManager, typename... Args>
        std::enable_if_t<!std::is_base_of_v<Component, NotDerived>, NotDerived& >
        AddComponent(Args... args) const
        {
            return T2::GetActiveWorld().EmplaceComponent<NotDerived>(m_entity, args...);
        }


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

        template<typename T, typename T2 = engine::WorldManager, typename... Args>
        T& EnsureComponent(Args... args) const
        {
            return HasComponent<T>() ? GetComponent<T>() : AddComponent<T>(args...);
        }

        template<typename T, typename T2 = engine::WorldManager>
        bool TryRemoveComponent() const
        {
            if (!T2::GetActiveWorld().HasComponent<T>(m_entity)) return false;
            
            T2::GetActiveWorld().RemoveComponent<T>(m_entity);
            return true;
        }

        RTTR_ENABLE()

    protected:
        Entity m_entity;
        bool m_active;
    };


}