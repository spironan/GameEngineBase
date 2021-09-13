/***********************************************************************************//*!
\file           SimulationLayer.cpp
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Sept 06, 2021
\brief          The Main Layer that will run the actual game scene while playing.
                This means that all systems that will run in the final binary
                will be ran similarly here.
                Does not provide any functionality otherwise.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "SimulationSceneLayer.h"


SimulationSceneLayer::SimulationSceneLayer(std::string const& filepath)
    : SceneBaseLayer{ "Simluation Scene Layer" }
    , m_filepath{ filepath }
{
    //temporary code
    ENGINE_ASSERT(std::filesystem::exists(filepath)
        , "this is testing code, comment out for now or use your own file path");
}

void SimulationSceneLayer::Init()
{
    ////temporary code
    //ENGINE_ASSERT(std::filesystem::exists(m_filepath)
    //    , "this is testing code, comment out for now or use your own file path");
    m_scene.LoadFromFile(m_filepath);

    //Register All Systems
    auto& prefabSystem = GetWorld()->RegisterSystem<engine::PrefabComponentSystem>();
    auto& EditorSystem = GetWorld()->RegisterSystem<engine::EditorComponentSystem>();

    auto& RenderingSystem = GetWorld()->RegisterSystem<engine::Renderer2DSystem>(DefaultCamera());
    auto& ScriptingSystem = GetWorld()->RegisterSystem<engine::ScriptSystem>();
    auto& PhysicsSystem = GetWorld()->RegisterSystem<engine::PhysicsSystem>();

    StartSimulation();
}

void SimulationSceneLayer::Exit()
{
    StopSimulation();
}

void SimulationSceneLayer::OnUpdate(engine::Timestep dt)
{
    if (m_isPause) return;
    if (m_stepMode && m_framesLeft == 0) return;
    --m_framesLeft;

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

void SimulationSceneLayer::ProcessFrame(int count)
{
    ENGINE_ASSERT_MSG(count <= 0, "frames should be lesser at least 1");

    m_stepMode = true;
    m_framesLeft = count;
}

void SimulationSceneLayer::StartSimulation()
{
    GetWorld()->GetSystem<engine::ScriptSystem>()->StartPlay();
}

void SimulationSceneLayer::PauseSimulation()
{
    m_isPause = true;
}

void SimulationSceneLayer::UnpauseSimulation()
{
    m_isPause = false;
}


void SimulationSceneLayer::StopSimulation()
{
    GetWorld()->GetSystem<engine::ScriptSystem>()->StopPlay();
}
