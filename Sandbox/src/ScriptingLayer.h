#pragma once

#include "Engine.h"

class ScriptingLayer : public engine::Layer
{
public:
    ScriptingLayer() : Layer("Scripting Layer")
    {
        // Register ECS Components here
        // engine::ScriptSystem::RegisterComponent<engine::Transform3D>("Transform");
    }

    ~ScriptingLayer()
    {
        engine::ScriptSystem::CleanUp();
    }

    void OnUpdate(engine::Timestep dt) override
    {
    }

    void OnEvent(engine::Event& e) override
    {
        engine::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<engine::WindowFocusEvent>(ENGINE_BIND_EVENT_FN(ScriptingLayer::Compile));
    }

    bool Compile(engine::WindowFocusEvent& e)
    {
        engine::ScriptSystem::Compile();
        return true;
    }
};