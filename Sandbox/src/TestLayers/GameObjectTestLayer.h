/************************************************************************************//*!
\file           GameObjectTestLayer.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           June 22, 2021
\brief          

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "UtilityLayers/SceneBaseLayer.h"

/****************************************************************************//*!
 @brief     Describes a Test scene used to test The Transform Components
            and Systems Functionality with ECS and Gameobjects.
*//*****************************************************************************/
class GameObjectTestLayer : public SceneBaseLayer
{

public:

    GameObjectTestLayer()
        : SceneBaseLayer{ "GameObjectTestLayer" }
    {
    }

    void Init() final override
    {
        {
            // Creates an Entity : should not be allowed
            engine::Entity ent{};
            // Creates a GameObject That Generates a unique Entity ID : allowed
            engine::GameObject go0{ engine::GameObject::Instantiate{} };
            // Creates a GameObject with entity : should not be allowed, soln : dont allowed entity to be constructed by any tomdickharry.
            engine::GameObject go1{ engine::Entity{} };
            // Creates a GameObject and copy over its data from the other gameobject : allowed
            engine::GameObject go2{ engine::GameObject{} };
            // Creates a GameObject and move its data from the other gameobject : allowed
            engine::GameObject go2move{ std::move(engine::GameObject{}) };

            // Create an Entity base on a gameObject : allowed
            engine::Entity ent1{ engine::GameObject{} };
            engine::Entity ent2{ go0 };

            //Copy Assignment
            engine::GameObject go3 = go1;

            //Move Assignment
            engine::GameObject go4 = std::move(go1);

            //Copy Assignment
            engine::GameObject go5 = engine::Entity{};
            engine::GameObject go6 = engine::GameObject{};

            engine::GameObject iAmObjectA{};
            // Copy ASSIGNMENT should not be allowed too : dk what to do with the previous object
            iAmObjectA = engine::GameObject{};
            //the old ObjectA is gone and no longer have a way to retrieve it.

            //Do not allow for this (although it make sense) reason is copy constructor here
            // always assumes the thing passed in has been created already (not just a random number)
            // So Make sure only selected classes can call 
            // engine::WorldManager::GetActiveWorld().CreateEntity()
            engine::GameObject{ engine::WorldManager::GetActiveWorld().CreateEntity() };

            //Manually do stupid shit. like creating an entity raising it to a gameobject and destroying it.
            //engine::GameObject{ engine::Entity{1} }.Destroy();

            //objects Does not get destroyed when getting out of scope!
            go0.Destroy();
            go1.Destroy();
            go2.Destroy();
            go6.Destroy();
        }

        // Create Another GameObject
        engine::GameObject test{ engine::GameObject::Instantiate{} };
    }

    virtual void OnUpdate(engine::Timestep dt) override
    {
    }

    virtual void OnImGuiRender() override
    {
    }
};
