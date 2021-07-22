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
#include "Engine/Transform/Transform3D.h"

namespace engine
{
    class GameObject final
    {
    private:
        Entity m_entity;
        Transform3D& m_transform;

        bool m_active;

        //std::vector<GameObject*> children;

    public:
        std::string Name;

        GameObject();
        ~GameObject();
        
        constexpr Entity GetID() const { return m_entity; }

        constexpr bool IsActive() const { return m_active; }
        
        Transform3D& Transform() const { return m_transform; }

        void SetActive(bool const active) { m_active = active; }
        
        void AddChild(GameObject const& gameObj, bool preserveTransforms = false);
        void AddChild(std::initializer_list<GameObject> gameObjs, bool preserveTransforms = false);

        //void RemoveChild(GameObject* gameObj);
        //std::vector<GameObject*> const& GetChildren() const; -- straight up not needed anymore

        // CAN BE DONE BUT NOT REQUIRED RIGHT NOW
        //GameObject* FindGameObjectInChildrenByName(std::string const& name);


        // Retrieve the index of the gameobject.
        //int IndexPosition() const
        //{
        //    // do code here to return the index
        //    return ;
        //}


        template<typename Component>
        Component& GetComponent() const;
        
        template<>
        Transform3D& GetComponent() const { return m_transform; }

        template<typename Component>
        Component* TryGetComponent() const;

        template<typename Component>
        bool HasComponent() const;


        template<typename Component, typename... Args>
        Component& AddComponent(Args... args) const;

        template<typename Component>
        Component& AddComponent(Component component) const;

        template <typename Component>
        Component* AddComponent(GameObject const& src) const;


        template<typename Component>
        void RemoveComponent() const;

        template<>
        void RemoveComponent<Transform3D>() const { throw "Cannot Remove Transform Component"; }

        template<typename Component>
        bool CopyComponent(GameObject const& src) const;
        
        template<typename Component>
        bool TransferComponent(GameObject const& src) const;

        /*template<typename Component>
        bool AttemptRemoveComponent();

        template<typename Component>
        Component& AssertComponent();*/

        template<typename Component, typename... Args>
        Component& EnsureComponent(Args... args) const;

    };

    /*---------------------------------------------------------------------------------*/
    /* Template Implementation                                                         */
    /*---------------------------------------------------------------------------------*/
    template<typename Component>
    Component& GameObject::GetComponent() const
    {
        return WorldManager::GetActiveWorld().GetComponent<Component>(m_entity);
    }
    
    template<typename Component>
    Component* GameObject::TryGetComponent() const
    {
        return WorldManager::GetActiveWorld().TryGetComponent<Component>(m_entity);
    }

    template<typename Component>
    bool GameObject::HasComponent() const
    {
        return WorldManager::GetActiveWorld().HasComponent<Component>(m_entity);
    }

    template<typename Component, typename...Args>
    Component& GameObject::AddComponent(Args...args) const
    {
        return WorldManager::GetActiveWorld().EmplaceComponent<Component>(m_entity, args...);
    }

    template<typename Component>
    Component& GameObject::AddComponent(Component component) const
    {
        return WorldManager::GetActiveWorld().AddComponent<Component>(m_entity);
    }

    template <typename Component>
    Component* GameObject::AddComponent(GameObject const& src) const
    {
        CopyComponent<Component>(src);
        return TryGetComponent<Component>(m_entity);
    }

    template<typename Component>
    void GameObject::RemoveComponent() const
    {
        WorldManager::GetActiveWorld().RemoveComponent<Component>(entity.uniqueID);
    }
    
    template<typename Component>
    bool GameObject::CopyComponent(GameObject const& src) const
    {
        return WorldManager::GetActiveWorld().CopyComponent<Component>(src.m_entity, m_entity);
    }

    template<typename Component>
    bool GameObject::TransferComponent(GameObject const& src) const
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
    Component& GameObject::EnsureComponent(Args...args) const
    {
        return HasComponent<Component>() ? GetComponent<Component>() : AddComponent<Component>(args...);
    }

}