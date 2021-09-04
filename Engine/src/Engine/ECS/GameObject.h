/************************************************************************************//*!
\file           GameObject.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Jul 22, 2021
\brief          Describes a gameobject which is the basic unit and building block for 
                every scene. This object provides the ability to tap on existing ECS
                systems and the ability to have scenegraph.
                

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "ECS.h"
#include "WorldManager.h"
#include "Engine/Transform/Transform3D.h"
#include "GameObjectComponent.h"

namespace engine
{
    class GameObject final
    {
    private:
        Entity m_entity = -1;

    public:
        
        // Getters
        Transform3D& Transform()    const { return GetComponent<Transform3D>(); };
        bool& ActiveSelf()          const { return GetComponent<GameObjectComponent>().ActiveSelf; }
        std::string& Name()         const { return GetComponent<GameObjectComponent>().Name; }
        Entity GetID()              const { return m_entity; }

        // Rule of 5
        GameObject()                              = default;
        GameObject(GameObject const& copy)        = default;
        GameObject(GameObject &&)                 = default;
        GameObject& operator=(GameObject const&)  = default;
        GameObject& operator=(GameObject&&)       = default;

        //gameobject assignment to entity
        GameObject& operator=(Entity entt) { m_entity = entt; return *this; };

        // Explicit Instantiation constructor, instantiate is a dummy type
        struct Create {};
        explicit GameObject(Create);

        // Non-Traditional Copy Construct GameObject Based on Entity
        GameObject(Entity entt);
        
        /*---------------------------------------------------------------------------------*/
        /* Static Functions                                                                */
        /*---------------------------------------------------------------------------------*/
        // Duplicating objects
        static void GameObject::DestroyGameObject(Entity entt);
        static GameObject& Instantiate(GameObject const& source);


        // use with caution, this should not immediately delete the object!
        void Destroy();

        //implicit cast operator
        operator Entity() const { return m_entity; }

        void AddChild(GameObject const& gameObj, bool preserveTransforms = false) const;
        void AddChild(std::initializer_list<GameObject> gameObjs, bool preserveTransforms = false) const;

        std::vector<Entity> GetChildren() const;

        // CAN BE DONE BUT NOT REQUIRED RIGHT NOW
        //GameObject* FindGameObjectInChildrenByName(std::string const& name);

        /*---------------------------------------------------------------------------------*/
        /* Queries                                                                         */
        /*---------------------------------------------------------------------------------*/
        template<typename Component>
        Component& GetComponent() const
        {
            return WorldManager::GetActiveWorld().GetComponent<Component>(m_entity);
        }
        
        template<typename... Components>
        decltype(auto) GetComponents() const
        {
            return WorldManager::GetActiveWorld().GetComponents<Components>(m_entity);
        }

        template<typename Component>
        Component* TryGetComponent() const
        {
            return WorldManager::GetActiveWorld().TryGetComponent<Component>(m_entity);
        }

        template<typename Component>
        bool HasComponent() const
        {
            return WorldManager::GetActiveWorld().HasComponent<Component>(m_entity);
        }

        template<typename Component>
        ComponentType GetComponentType()
        {
            return WorldManager::GetActiveWorld().GetComponentType<Component>();
        }

        /*---------------------------------------------------------------------------------*/
        /* Manipulation                                                                    */
        /*---------------------------------------------------------------------------------*/
        
        template<typename Derived, typename... Args>
        std::enable_if_t<std::is_base_of_v<Component, Derived>, Derived& >
        AddComponent(Args... args) const
        {
            return WorldManager::GetActiveWorld().EmplaceComponent<Derived>(m_entity, true, args...);
        }

        template<typename NotDerived, typename... Args>
        std::enable_if_t<!std::is_base_of_v<Component, NotDerived>, NotDerived& >
        AddComponent(Args... args) const
        {
            return WorldManager::GetActiveWorld().EmplaceComponent<NotDerived>(m_entity, args...);
        }

        template<typename Component>
        Component& AddComponent(Component component) const
        {
            return WorldManager::GetActiveWorld().AddComponent<Component>(m_entity, component);
        }

        template <typename Component>
        Component* AddComponent(GameObject const& src) const
        {
            CopyComponent<Component>(src);
            return TryGetComponent<Component>(m_entity);
        }

        template<typename Component>
        void RemoveComponent() const
        {
            WorldManager::GetActiveWorld().RemoveComponent<Component>(m_entity);
        }

        template<>
        void RemoveComponent<Transform3D>() const { throw "Cannot Remove Transform Component!"; }

        template<typename Component>
        bool CopyComponent(GameObject const& src) const
        {
            return WorldManager::GetActiveWorld().CopyComponent<Component>(src.m_entity, m_entity);
        }

        template<typename Component>
        bool TransferComponent(GameObject const& src) const
        {
            return WorldManager::GetActiveWorld().TransferComponent<Component>(src.m_active, m_entity);
        }

        /*template<typename Component>
        bool AttemptRemoveComponent();

        template<typename Component>
        Component& AssertComponent();*/

        template<typename Component, typename...Args>
        Component& EnsureComponent(Args...args) const
        {
            return HasComponent<Component>() ? GetComponent<Component>() : AddComponent<Component>(args...);
        }


        /*GameObject& CopyGameObject(GameObject const& source)
        {
            return *this = WorldManager::GetActiveWorld().DuplicateEntity(source);
        }*/

    };

}
