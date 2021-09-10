/***********************************************************************************//*!
\file           GameSceneLayer.cpp
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Sept 06, 2021
\brief          The Main Layer that will run the actual game scene while editing.
                This means that certain systems dictated not to run during editing
                such as physics and scripting will not be updated here.
                Does not provide any functionality otherwise.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "GameSceneLayer.h"

GameSceneLayer::GameSceneLayer()
    : SceneBaseLayer{ "Game Scene Layer",""}
{
}

void GameSceneLayer::Init()
{
    //Register All Systems
    auto& prefabSystem = GetWorld()->RegisterSystem<engine::PrefabComponentSystem>();
    auto& EditorSystem = GetWorld()->RegisterSystem<engine::EditorComponentSystem>();

    auto& RenderingSystem = GetWorld()->RegisterSystem<engine::Renderer2DSystem>(DefaultCamera());
    auto& ScriptingSystem = GetWorld()->RegisterSystem<engine::ScriptSystem>();
    auto& PhysicsSystem = GetWorld()->RegisterSystem<engine::PhysicsSystem>();


    // Randomly create ogre texture in the middle
    auto ogreHandle = engine::AssetManager::ImportAsset("../Engine/assets/images/ogre.png");
    auto tex = engine::AssetManager::GetAsset<engine::Texture>(ogreHandle);
    auto& testObject = CreateGameObject();
    auto& sprite = testObject.AddComponent<engine::Sprite2D>();
    sprite.SetTexture(tex);
    testObject.Transform().Scale() = { 50, 50, 1 };

}

void GameSceneLayer::OnUpdate(engine::Timestep dt)
{
    ////only needed because of all the testing layers
    //engine::SceneManager::SetActiveScene(GetScene().GetID());
    //engine::WorldManager::SetActiveWorld(GetWorld()->GetID());

    //Only update things that will update while editing
    GetWorld()->GetSystem<engine::TransformSystem>()->Update();
    GetWorld()->GetSystem<engine::Renderer2DSystem>()->Update();
}

void GameSceneLayer::OnImGuiRender()
{
}
