#include "GameSceneLayer.h"

GameSceneLayer::GameSceneLayer()
    : SceneBaseLayer{ "Game Scene Layer" }
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
    engine::Texture tex = engine::TextureLoader::LoadFromFilePath("../Engine/assets/images/ogre.png");
    engine::TextureDatabase::AddTexture("ogre", tex);
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
