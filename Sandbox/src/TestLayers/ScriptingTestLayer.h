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

class ScriptingTestLayer : public SceneBaseLayer
{
private:
    std::vector<engine::GameObject> goList;
    engine::GameObject m_Camera;
    bool isPlaying;

public:
    ScriptingTestLayer() 
        : SceneBaseLayer{ "ScriptingTestLayer" }
        , isPlaying{ false }
    {
    }

    void Init() final override
    {
        // initilization of camera
        engine::Window& x = engine::Application::Get().GetWindow();
        int width = x.GetSize().first;
        int height = x.GetSize().second;
        m_Camera = CreateGameObject();
        m_Camera.Name() = "Main Camera";
        auto& cam = m_Camera.AddComponent<engine::SceneCamera>();
        cam.UpdateViewportSize(width, height);

        auto& rs = m_scene.GetWorld().RegisterSystem<engine::Renderer2DSystem>(cam);
        auto& ss = m_scene.GetWorld().RegisterSystem<engine::ScriptSystem>();
        auto& ps = m_scene.GetWorld().RegisterSystem<engine::PhysicsSystem>();

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
        m_scene.SetWorldAsActive();

        auto ss = m_scene.GetWorld().GetSystem<engine::ScriptSystem>();
        ss->InvokeFunctionAll("Update", 1, (float)dt);
        m_scene.GetWorld().GetSystem<engine::PhysicsSystem>()->Update(dt);
        m_scene.GetWorld().GetSystem<engine::TransformSystem>()->Update();

        m_scene.GetWorld().ProcessDeletions();

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
        m_scene.GetWorld().GetSystem<engine::Renderer2DSystem>()->Update();
    }
};