#pragma once

#include <Engine.h>

class ScriptingTestLayer : public engine::Layer
{
private:
    engine::World& m_world;

public:
    ScriptingTestLayer() :
        Layer{ "ScriptingTestLayer" },
        m_world{ engine::WorldManager::CreateWorld() }
    {
        auto& ss = m_world.RegisterSystem<engine::ScriptSystem>();
        ss->Compile();
    }

    virtual void OnUpdate(engine::Timestep dt) override
    {
        if (engine::Input::IsKeyPressed(engine::KeyCode::SPACE))
        {
            m_world.GetSystem<engine::ScriptSystem>()->Compile();
        }
    }
};