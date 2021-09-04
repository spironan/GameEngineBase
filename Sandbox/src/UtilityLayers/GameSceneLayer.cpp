#include "GameSceneLayer.h"

GameSceneLayer::GameSceneLayer()
    : SceneBaseLayer{ "Game Scene Layer" }
{
}

void GameSceneLayer::Init()
{
    engine::Window& x = engine::Application::Get().GetWindow();
    auto [width, height] = x.GetSize();

    engine::Texture tex = engine::TextureLoader::LoadFromFilePath("../Engine/assets/images/ogre.png");
    engine::TextureDatabase::AddTexture("ogre", tex);

    auto& cam = m_defaultCamera.AddComponent<engine::SceneCamera>();
    cam.UpdateViewportSize(width, height);

    //Register All Systems
    auto& rs = m_scene.GetWorld().RegisterSystem<engine::Renderer2DSystem>(cam);
    auto& ss = m_scene.GetWorld().RegisterSystem<engine::ScriptSystem>();
    auto& ps = m_scene.GetWorld().RegisterSystem<engine::PhysicsSystem>();
}

void GameSceneLayer::OnUpdate(engine::Timestep dt)
{
    //Only update things that will update while editing
    m_scene.GetWorld().GetSystem<engine::TransformSystem>()->Update();
    m_scene.GetWorld().GetSystem<engine::Renderer2DSystem>()->Update();
}

void GameSceneLayer::OnImGuiRender()
{
}
