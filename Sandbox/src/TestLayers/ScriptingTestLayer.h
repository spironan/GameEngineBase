/************************************************************************************//*!
\file           ScriptingTestLayer.h
\project        <PROJECT_NAME>
\author         Solomon Tan Teng Shue, t.tengshuesolomon, 620010020
\par            email: t.tengshuesolomon\@digipen.edu
\date           August 3, 2021
\brief          Test scene for testing scripting functionality between C# and C++

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/

#pragma once

#include "UtilityLayers/SceneBaseLayer.h"
#include "Engine/Prefab/PrefabComponentSystem.h"
#include "Engine/Prefab/EditorComponentSystem.h"
class ScriptingTestLayer : public SceneBaseLayer
{
private:
    std::vector<engine::GameObject> goList;
    
    bool isPlaying;

public:
    ScriptingTestLayer() 
        : SceneBaseLayer{ "ScriptingTestLayer" }
        , isPlaying{ false }
    {
    }

    void Init() final override
    {
        auto& rs = GetWorld()->RegisterSystem<engine::Renderer2DSystem>(DefaultCamera());
        auto& ss = GetWorld()->RegisterSystem<engine::ScriptSystem>();
        auto& ps = GetWorld()->RegisterSystem<engine::PhysicsSystem>();

        // Player
        {
            engine::GameObject obj = CreateGameObject();
            obj.Name() = "Player";
            obj.Transform().SetScale({ 10, 10, 1 });
            obj.AddComponent<engine::Sprite2D>().SetColor({ 0, 1, 1, 1 });
            obj.GetComponent<engine::Scripting>().AddScriptInfo({ "", "PlayerScript" });
            auto& rbody = obj.AddComponent<engine::Rigidbody2D>();
            rbody.SetMass(1.0f);
            rbody.GravityScale = 0.0f;
            obj.AddComponent<engine::Collider2D>();
            obj.AddComponent<engine::CircleCollider2D>();
        }

        // Food
        {
            engine::GameObject obj = CreateGameObject();
            obj.Name() = "Food";
            obj.Transform().SetScale({ 5, 5, 1 });
            obj.AddComponent<engine::Sprite2D>().SetColor({ 1, 0, 0, 1 });
            obj.GetComponent<engine::Scripting>().AddScriptInfo({ "", "FoodScript" });
            auto& rbody = obj.AddComponent<engine::Rigidbody2D>();
            rbody.SetMass(1.0f);
            rbody.GravityScale = 0.0f;
            obj.AddComponent<engine::Collider2D>().IsTrigger = true;
            obj.AddComponent<engine::CircleCollider2D>();
        }
    }

    virtual void OnUpdate(engine::Timestep dt) override
    {
        //GetScene().SetWorldAsActive();
        engine::WorldManager::SetActiveWorld(GetWorld()->GetID());

        auto ss = GetWorld()->GetSystem<engine::ScriptSystem>();
        ss->InvokeFunctionAll("Update", 1, (float)dt);
        GetWorld()->GetSystem<engine::PhysicsSystem>()->Update(dt);
        GetWorld()->GetSystem<engine::TransformSystem>()->Update();

        //GetWorld()->ProcessDeletions();

        if (engine::Input::IsKeyPressed(engine::KeyCode::DEL))
        {
            if (!isPlaying)
            {
                if(ss->StartPlay())
                    isPlaying = true;
            }
            else
            {
                if(ss->StopPlay())
                    isPlaying = false;
            }
        }

        if (engine::Input::IsKeyPressed(engine::KeyCode::END))
        {
            if (isPlaying)
                ss->DebugPrint();
            else
                ss->DebugPrintInfo();
        }
    }

    virtual void OnImGuiRender() override
    {
        GetWorld()->GetSystem<engine::Renderer2DSystem>()->Update();
    }
};