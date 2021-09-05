#pragma once

#include "Engine.h"

class ScriptingLayer : public engine::Layer
{
public:
    ScriptingLayer() : Layer("Scripting Layer")
    {
        // Register ECS Components here
        engine::ScriptSystem::RegisterComponent<engine::Transform3D>("Ouroboros.Transform");
        engine::ScriptSystem::RegisterComponent<engine::Sprite2D>("Ouroboros.SpriteRenderer");
        engine::ScriptSystem::RegisterComponent<engine::Rigidbody2D>("Ouroboros.Rigidbody2D");
        engine::ScriptSystem::RegisterComponent<engine::Collider2D>("Ouroboros.Collider2D");
        engine::ScriptSystem::RegisterComponent<engine::CircleCollider2D>("Ouroboros.CircleCollider2D");

        // load scripting.dll
        engine::ScriptSystem::Initialize();
    }

    ~ScriptingLayer()
    {
        engine::ScriptSystem::CleanUp();
    }

    void OnUpdate(engine::Timestep dt) override
    {
    }

    //void OnEvent(engine::Event& e) override
    //{
    //    engine::EventDispatcher dispatcher(e);
    //    dispatcher.Dispatch<engine::WindowFocusEvent>(ENGINE_BIND_EVENT_FN(ScriptingLayer::Compile));
    //}

    //bool Compile(engine::WindowFocusEvent& e)
    //{
    //    engine::ScriptSystem::Compile();
    //    return true;
    //}
};