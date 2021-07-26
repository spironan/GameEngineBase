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

#include <Engine.h>

/****************************************************************************//*!
 @brief     Describes a Test scene used to test The Transform Components
            and Systems Functionality with ECS and Gameobjects.
*//*****************************************************************************/
class GameObjectTestLayer : public engine::Layer
{
private:
    engine::World& m_world;

public:

    GameObjectTestLayer()
        : Layer{ "GameObjectTestLayer" }
        , m_world{ engine::WorldManager::CreateWorld() }
    {
        {
            // Creates an Entity : should not be allowed
            //engine::Entity ent{};
            // Creates a GameObject That Generates a unique Entity ID : allowed
            engine::GameObject go0{};
            // Creates a GameObject with entity : should not be allowed, soln : dont allowed entity to be constructed by any tomdickharry.
            //engine::GameObject go1{ engine::Entity{} };
            //Creates a GameObject and copy over its data from the other gameobject : allowed
            engine::GameObject go2{ engine::GameObject{} };

            // Create an Entity base on a gameObject
            engine::Entity ent1{ engine::GameObject{} };
            engine::Entity ent2{ go0 };

            //Copy Assignment
            //engine::GameObject go3 = go1;
            
            //Move Assignment
            //engine::GameObject go4 = std::move(go1);

            //Copy Assignment
            //engine::GameObject go5 = engine::Entity{};
            engine::GameObject go6 = engine::GameObject{};


            //objects Does not get destroyed when getting out of scope!

            go0.Destroy();
            //go1.Destroy();
            go2.Destroy();
            go6.Destroy();
        }

        // Create Another GameObject
        engine::GameObject test{};




    }


    virtual void OnUpdate(engine::Timestep dt) override
    {
    }

    virtual void OnImGuiRender() override
    {
    }
};
