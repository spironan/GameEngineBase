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
#include "Engine/Transform/TransformSystem.h"

namespace engine
{
    // order matters! dont switch it!
    GameObject::GameObject()
        : m_entity{ WorldManager::GetActiveWorld().CreateEntity() }
        , m_transform { AddComponent<Transform3D>() }
    {
    }

    GameObject::~GameObject()
    {
        WorldManager::GetActiveWorld().DestroyEntity(m_entity);
    }

    void GameObject::AddChild(GameObject const& go, bool preserveTransforms)
    {
        // Flag coordinates to be converted when parented
        if (preserveTransforms)
        {
            go.Transform().ConvertCoordinates();
        }
        
        //WorldManager::GetActiveWorld().GetSystem<engine::TransformSystem>()->GetSceneGraph().Attach(go, *this);

        //children.push_back(gameObj);
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