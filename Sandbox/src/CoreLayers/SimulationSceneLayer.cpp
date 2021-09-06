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
