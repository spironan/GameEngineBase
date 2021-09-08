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

#include "Engine/Scripting/Scripting.h"

// editor component - damn sus to be here
#include "Engine/Prefab/EditorComponent.h"

namespace engine
{
    /*---------------------------------------------------------------------------------*/
    /* Static Functions                                                                */
    /*---------------------------------------------------------------------------------*/
    GameObject GameObject::Instantiate(GameObject source)
    {
        return static_cast<GameObject>(WorldManager::GetActiveWorld().DuplicateEntity(source));
    }

    void GameObject::DestroyGameObject(GameObject go)
    {
        ////go.ActiveSelf() = false;
        ///*tf.DetachFromRoot();*/
        
        WorldManager::GetActiveWorld().GetSystem<engine::TransformSystem>()->Store(go);

        auto& tf = go.Transform();
        for(auto const& child : WorldManager::GetActiveWorld().GetSystem<engine::TransformSystem>()->GetChildren(tf))
            WorldManager::GetActiveWorld().DestroyEntity(child.GetEntity());

        WorldManager::GetActiveWorld().DestroyEntity(go);
    }


    // order matters! dont switch it! Instantiate is a dummy type
    GameObject::GameObject(Create)
        : m_entity  { WorldManager::GetActiveWorld().CreateEntity() }
    {
        AddComponent<Transform3D>();
        AddComponent<GameObjectComponent>();
        /*AddComponent<Scripting>();*/

        // could be moved... should also be removed if editor is not enabled...
        AddComponent<EditorComponent>();
    }

    GameObject::GameObject(Entity entt)
        : m_entity  { entt }
    {
    }

    void GameObject::Destroy()
    {
        // use with caution : should not delete the gameobject immediately
        // but add to a stack instead that is used to call the code below
        // at the end of the frame.
        // should use a pair to avoid multiple removes of the same object.
        DestroyGameObject(m_entity);
    }


    void GameObject::AddChild(GameObject const& child, bool preserveTransforms) const
    {
        // Flag coordinates to be converted when parented
        if (preserveTransforms)
        {
            child.Transform().ConvertCoordinates();
        }
        
        WorldManager::GetActiveWorld().GetSystem<engine::TransformSystem>()->Attach(child, *this);
    }

    void GameObject::AddChild(std::initializer_list<GameObject> gos, bool preserveTransforms) const
    {
        for (auto go : gos)
        {
            AddChild(go, preserveTransforms);
        }
    }

    std::vector<Entity> GameObject::GetChildren() const
    {
        auto const& childTfs = WorldManager::GetActiveWorld().GetSystem<engine::TransformSystem>()->GetChildren(Transform());
        std::vector<Entity> entities;
        for (auto const& childTf : childTfs) entities.emplace_back(childTf.GetEntity());
        return entities;
    }
    

    //void GameObject::RemoveChild(GameObject* gameObj)
    //{
    //    //children.erase(std::find(children.begin(), children.end(), gameObj));
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