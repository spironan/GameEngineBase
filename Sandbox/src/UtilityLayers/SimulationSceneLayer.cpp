#include "SimulationSceneLayer.h"

SimulationSceneLayer::SimulationSceneLayer()
    : SceneBaseLayer{ "Simluation Scene Layer" }
{
}

void SimulationSceneLayer::Init()
{
    //Register All Systems
    //auto& rs = m_scene.GetWorld().RegisterSystem<engine::Renderer2DSystem>(cam);
    auto& ss = m_scene.GetWorld().RegisterSystem<engine::ScriptSystem>();
    auto& ps = m_scene.GetWorld().RegisterSystem<engine::PhysicsSystem>();
}

void SimulationSceneLayer::OnUpdate(engine::Timestep dt)
{
    auto& world = m_scene.GetWorld();
    //Update All Systems
    auto ss = world.GetSystem<engine::ScriptSystem>();
    ss->InvokeFunctionAll("Update", 1, (float)dt);
    world.GetSystem<engine::TransformSystem>()->Update();
    world.GetSystem<engine::PhysicsSystem>()->Update(dt);
    world.GetSystem<engine::Renderer2DSystem>()->Update();
}

void SimulationSceneLayer::OnImGuiRender()
{
}
