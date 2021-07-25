/************************************************************************************//*!
\file           GameObject.cpp
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Jul 07, 2021
\brief          Describes a gameobject which is the interface for everything ECS related

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "GameObject.h"

#include "Engine/Transform/Transform3D.h"

namespace engine
{
    GameObject::GameObject()
    {
        //m_entity = ECS_Manager::GenerateUID();
        //m_entity = EntityManager::CreateEntity();

    }

    GameObject::~GameObject()
    {
        WorldManager::GetActiveWorld().DestroyEntity(m_entity);
    }

    void GameObject::Init()
    {
        m_entity = WorldManager::GetActiveWorld().CreateEntity();
        auto& trans = AddComponent<Transform3D>();
    }

    void GameObject::AddChild(GameObject* gameObj, bool preserveTransforms)
    {
        // Flag coordinates to be converted when parented
        if (preserveTransforms)
        {
            //gameObj->GetComponent<Transform3D>().ConvertCoordinates();
        }

        children.push_back(gameObj);
    }

    GameObject* GameObject::AddChild(bool preserveTransforms)
    {
        // Flag coordinates to be converted when parented
        if (preserveTransforms)
        {
            //gameObj->GetComponent<Transform3D>().ConvertCoordinates();
        }
        auto child = children.emplace_back(new GameObject());
        child->Init();
        return child;
    }


    void GameObject::AddChild(std::initializer_list<GameObject*> gameObjs, bool preserveTransforms)
    {
        for (GameObject* obj : gameObjs)
        {
            AddChild(obj, preserveTransforms);
        }
    }

    void GameObject::RemoveChild(GameObject* gameObj)
    {
        children.erase(std::find(children.begin(), children.end(), gameObj));
    }

    std::vector<GameObject*> const& GameObject::GetChildren() const
    {
        return children;
    }

    GameObject* GameObject::FindGameObjectInChildrenByName(std::string const& name)
    {
        // Is this the one?
        if (Name == name)
        {
            return this;
        }

        // Search Children
        for (auto& go : children)
        {
            // Checks to see if the object was found
            GameObject* findResult = go->FindGameObjectInChildrenByName(name);
            if (findResult)
            {
                // Found!
                return findResult;
            }
        }

        // Went through everything and didn't find anything
        return nullptr;
    }
}