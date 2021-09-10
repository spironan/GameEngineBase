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
using namespace engine;
static std::vector<GameObject> gameobjects;
static std::vector<World::DeletedObjectPtr> deletedObjects;
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
        gameobjects.clear();
        deletedObjects.clear();
        // Create Another GameObject
        GameObject test = CreateGameObject();
        //test.AddComponent<engine::EditorComponent>();
    }

    virtual void OnUpdate(engine::Timestep dt) override
    {
        //GetScene().SetWorldAsActive();
        if (Input::IsKeyPressed(Key::A))
        {
            gameobjects.emplace_back(CreateGameObject());
            auto result1 = gameobjects.back().HasComponent<Transform3D>();
            int i = 0;
        }
        if (Input::IsKeyPressed(Key::S))
        {
            if (gameobjects.size() > 0)
            {
                gameobjects.back().Destroy();
                gameobjects.pop_back();
            }
        }
        if (Input::IsKeyPressed(Key::D))
        {
            if (gameobjects.size() > 0)
            {
                deletedObjects.emplace_back(GetWorld()->StoreAsDeleted(gameobjects.back().GetEntity()));
                gameobjects.back().Destroy();
                gameobjects.pop_back();
            }
        }
        if (Input::IsKeyPressed(Key::F))
        {
            if (deletedObjects.size() > 0)
            {
                auto go = GetWorld()->RestoreFromDeleted(*deletedObjects.back());
                deletedObjects.pop_back();
                GameObject temp{ go };
                gameobjects.emplace_back(temp);
                auto result1 = temp.HasComponent<Transform3D>();
                auto result2 = temp.HasComponent<GameObjectComponent>();
                auto result3 = temp.HasComponent<EditorComponent>();
                int i = 0;
            }
        }
    }

    virtual void OnImGuiRender() override
    {
    }
};
