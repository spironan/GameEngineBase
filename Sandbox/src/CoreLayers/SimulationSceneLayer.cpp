#include "SimulationSceneLayer.h"

SimulationSceneLayer::SimulationSceneLayer()
    : SceneBaseLayer{ "Simluation Scene Layer" }
{
}

void SimulationSceneLayer::Init()
{
    //Register All Systems
    auto& prefabSystem = GetWorld()->RegisterSystem<engine::PrefabComponentSystem>();
    auto& EditorSystem = GetWorld()->RegisterSystem<engine::EditorComponentSystem>();

    auto& RenderingSystem = GetWorld()->RegisterSystem<engine::Renderer2DSystem>(DefaultCamera());
    auto& ScriptingSystem = GetWorld()->RegisterSystem<engine::ScriptSystem>();
    auto& PhysicsSystem = GetWorld()->RegisterSystem<engine::PhysicsSystem>();
}

void SimulationSceneLayer::OnUpdate(engine::Timestep dt)
{
    //Update All Systems
    auto ss = GetWorld()->GetSystem<engine::ScriptSystem>();
    ss->InvokeFunctionAll("Update", 1, (float)dt);
    GetWorld()->GetSystem<engine::TransformSystem>()->Update();
    GetWorld()->GetSystem<engine::PhysicsSystem>()->Update(dt);
    GetWorld()->GetSystem<engine::Renderer2DSystem>()->Update();
}

void SimulationSceneLayer::OnImGuiRender()
{
}
