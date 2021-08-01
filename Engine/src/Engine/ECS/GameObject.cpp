/************************************************************************************//*!
\file           GameObject.cpp
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Jul 22, 2021
\brief          Describes a gameobject which is the interface for everything ECS related

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "GameObject.h"

#include "Engine/Transform/Transform3D.h"
#include "Engine/Transform/TransformSystem.h"

namespace engine
{
    // order matters! dont switch it!
    GameObject::GameObject()
        : m_entity      { WorldManager::GetActiveWorld().CreateEntity() }
        /*, ActiveSelf    { AddComponent<GameObjectComponent>().ActiveSelf }
        , Name          { GetComponent<GameObjectComponent>().Name }
        , Transform     { AddComponent<Transform3D>() }*/
    {
        AddComponent<Transform3D>();
        AddComponent<GameObjectComponent>();
    }

    GameObject::GameObject(Entity entt)
        : m_entity      { entt }
        /*, ActiveSelf    { GetComponent<GameObjectComponent>().ActiveSelf }
        , Name          { GetComponent<GameObjectComponent>().Name }
        , Transform     { GetComponent<Transform3D>() }*/
    {
    }

    void GameObject::Destroy()
    {
        // use with caution : should not delete the gameobject immediately
        // but add to a stack instead that is used to call the code below
        // at the end of the frame.
        // should use a pair to avoid multiple removes of the same object.
        WorldManager::GetActiveWorld().DestroyEntity(m_entity);
    }

    //GameObject& GameObject::operator=(GameObject const& other)
    //{
    //    //What happens if im another entity before assignment?
    //    //1. do i destroy the previous gameobject?

    //    m_entity = other.m_entity;
    //    /*Transform = other.Transform;
    //    ActiveSelf = other.ActiveSelf;*/

    //    return *this;
    //}

    void GameObject::AddChild(GameObject const& child, bool preserveTransforms)
    {
        // Flag coordinates to be converted when parented
        if (preserveTransforms)
        {
            child.Transform().ConvertCoordinates();
        }
        
        WorldManager::GetActiveWorld().GetSystem<engine::TransformSystem>()->Attach(child, *this);
    }

    void GameObject::AddChild(std::initializer_list<GameObject> gos, bool preserveTransforms)
    {
        for (auto  go : gos)
        {
            AddChild(go, preserveTransforms);
        }
    }

    //void GameObject::RemoveChild(GameObject* gameObj)
    //{
    //    //children.erase(std::find(children.begin(), children.end(), gameObj));
    //}

    //std::vector<GameObject*> const& GameObject::GetChildren() const
    //{
    //    //return children;
    //}

    // CAN BE DONE BUT NOT REQUIRED RIGHT NOW
    //GameObject* GameObject::FindGameObjectInChildrenByName(std::string const& name)
    //{
    //    // Is this the one?
    //    if (Name == name)
    //    {
    //        return this;
    //    }

    //    // Search Children
    //    for (auto& go : children)
    //    {
    //        // Checks to see if the object was found
    //        GameObject* findResult = go->FindGameObjectInChildrenByName(name);
    //        if (findResult)
    //        {
    //            // Found!
    //            return findResult;
    //        }
    //    }

    //    // Went through everything and didn't find anything
    //    return nullptr;
    //}
}