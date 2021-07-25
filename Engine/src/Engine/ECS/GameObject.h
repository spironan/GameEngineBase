/************************************************************************************//*!
\file           GameObject.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Jul 07, 2021
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

namespace engine
{
    class GameObject final
    {
    private:
        Entity m_entity;
        bool m_active;

        std::vector<GameObject*> children;

    public:
        std::string Name;

        GameObject();
        ~GameObject();
        
        void Init();

        constexpr Entity GetID() const { return m_entity; }

        constexpr bool IsActive() const { return m_active; }
        void SetActive(bool const active) { m_active = active; }
        
        void AddChild(GameObject* gameObj, bool preserveTransforms = false);
        GameObject* AddChild(bool preserveTransforms = false);
        void AddChild(std::initializer_list<GameObject*> gameObjs, bool preserveTransforms = false);
        void RemoveChild(GameObject* gameObj);
        std::vector<GameObject*> const& GetChildren() const;

        GameObject* FindGameObjectInChildrenByName(std::string const& name);


        template<typename Component>
        Component& GetComponent();

        template<typename Component>
        Component* TryGetComponent();

        template<typename Component>
        bool HasComponent() const;


        template<typename Component, typename... Args>
        Component& AddComponent(Args... args);

        template<typename Component>
        Component& AddComponent(Component component);

        template <typename Component>
        Component* AddComponent(GameObject const& src);


        template<typename Component>
        void RemoveComponent();

        template<typename Component>
        bool CopyComponent(GameObject const& src);
        
        template<typename Component>
        bool TransferComponent(GameObject const& src);

        /*template<typename Component>
        bool AttemptRemoveComponent();

        template<typename Component>
        Component& AssertComponent();*/

        template<typename Component, typename... Args>
        Component& EnsureComponent(Args... args);

    };

    /*---------------------------------------------------------------------------------*/
    /* Template Implementation                                                         */
    /*---------------------------------------------------------------------------------*/
    template<typename Component>
    Component& GameObject::GetComponent()
    {
        return WorldManager::GetActiveWorld().GetComponent<Component>(m_entity);
    }
    
    template<typename Component>
    Component* GameObject::TryGetComponent()
    {
        return WorldManager::GetActiveWorld().TryGetComponent<Component>(m_entity);
    }

    template<typename Component>
    bool GameObject::HasComponent() const
    {
        return WorldManager::GetActiveWorld().HasComponent<Component>(m_entity);
    }

    template<typename Component, typename...Args>
    Component& GameObject::AddComponent(Args...args)
    {
        return WorldManager::GetActiveWorld().EmplaceComponent<Component>(m_entity, args...);
    }

    template<typename Component>
    Component& GameObject::AddComponent(Component component)
    {
        return WorldManager::GetActiveWorld().AddComponent<Component>(m_entity);
    }

    template <typename Component>
    Component* GameObject::AddComponent(GameObject const& src)
    {
        CopyComponent<Component>(src)
        return TryGetComponent<Component>(m_entity);
    }

    template<typename Component>
    void GameObject::RemoveComponent()
    {
        WorldManager::GetActiveWorld().RemoveComponent<Component>(entity.uniqueID);
    }
    
    template<typename Component>
    bool GameObject::CopyComponent(GameObject const& src)
    {
        return WorldManager::GetActiveWorld().CopyComponent<Component>(src.m_entity, m_entity);
    }

    template<typename Component>
    bool GameObject::TransferComponent(GameObject const& src)
    {
        return WorldManager::GetActiveWorld().TransferComponent<Component>(src.m_active, m_entity);
    }

    /*template<typename Component>
    bool GameObject::AttemptRemoveComponent()
    {
        return ECS_Manager::AttemptRemoveComponent<Component>(entity.uniqueID);
    }

    template<typename Component>
    Component& GameObject::AssertComponent()
    {
        Component* c = GetComponent<Component>();
        if (c == nullptr)
        {
            std::ostringstream oss;
            oss << "GameObject does not contain a ";
            oss << typeid(Component).name();
            oss << " Component!";
            throw MissingComponentException(oss.str());
        }
        return c;
        ECS_Manager::TryGetComponent
    }*/

    template<typename Component, typename...Args>
    Component& GameObject::EnsureComponent(Args...args)
    {
        return HasComponent<Component>() ? GetComponent<Component>() : AddComponent<Component>(args...);
    }

}
