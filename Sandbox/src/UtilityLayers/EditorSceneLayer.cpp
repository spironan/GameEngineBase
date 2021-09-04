#include "EditorSceneLayer.h"

EditorSceneLayer::EditorSceneLayer()
    : SceneBaseLayer("Editor Scene Layer")
{
}

void EditorSceneLayer::Init()
{
    engine::Window& x = engine::Application::Get().GetWindow();
    auto [width, height] = x.GetSize();

    engine::Texture tex = engine::TextureLoader::LoadFromFilePath("../Engine/assets/images/ogre.png");
    engine::TextureDatabase::AddTexture("ogre", tex);
    auto& testObject = CreateGameObject();
    testObject.AddComponent<engine::Sprite2D>().SetTexture(tex);
    testObject.Transform().Scale() = { 50, 50, 1 };
    
    m_defaultCamera = CreateGameObject();
    m_defaultCamera.Name() = "Default Camera";
    m_defaultCamera.AddComponent<engine::SceneCamera>().UpdateViewportSize(width, height);
    
    m_editorCamera = CreateGameObject();
    m_editorCamera.Name() = "Editor Camera";
    auto& cam = m_editorCamera.AddComponent<engine::SceneCamera>();
    cam.UpdateViewportSize(width, height);

    //Register All Systems
    auto& prefabSystem = m_world->RegisterSystem<engine::PrefabComponentSystem>();
    auto& EditorSystem = m_world->RegisterSystem<engine::EditorComponentSystem>();

    auto& RenderingSystem = m_world->RegisterSystem<engine::Renderer2DSystem>(cam);
    auto& ScriptingSystem = m_world->RegisterSystem<engine::ScriptSystem>();
    auto& PhysicsSystem = m_world->RegisterSystem<engine::PhysicsSystem>();

}

void EditorSceneLayer::OnUpdate(engine::Timestep dt)
{
    //only needed because of all the testing layers
    engine::SceneManager::SetActiveScene(m_scene.GetID());
    engine::WorldManager::SetActiveWorld(m_world->GetID());

    //Only update things that will update while editing
    m_world->GetSystem<engine::TransformSystem>()->Update();
    m_world->GetSystem<engine::Renderer2DSystem>()->Update();
    
    //Editor Scene-only input detection and resolution.
    // move camera here

    if (engine::Input::IsMouseButtonDown(ENGINE_MOUSE_BUTTON_RIGHT))
    {
        if (engine::Input::IsKeyDown(ENGINE_KEY_W))
        {
            m_editorCamera.Transform().Position().y += CameraMovingSpeed * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_S))
        {
            m_editorCamera.Transform().Position().y -= CameraMovingSpeed * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_D))
        {
            m_editorCamera.Transform().Position().x += CameraMovingSpeed * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_A))
        {
            m_editorCamera.Transform().Position().x -= CameraMovingSpeed * dt;
        }
    }

    m_world->GetSystem<engine::Renderer2DSystem>()->SetCamera(
        m_editorCamera.GetComponent<engine::SceneCamera>(),
        m_editorCamera.Transform().GetPosition());
}

void EditorSceneLayer::OnImGuiRender()
{
}
