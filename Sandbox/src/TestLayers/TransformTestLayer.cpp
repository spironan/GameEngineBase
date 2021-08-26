#include "TransformTestLayer.h"

#include <Engine.h>

TransformTestLayer::TransformTestLayer()
    : SceneBaseLayer{ "TransformTestLayer" }
{
}

void TransformTestLayer::SelectNewTarget()
{
    ++m_target;
    if (m_target == m_gos.end())
    {
        m_target = m_gos.begin();
    }
}


void TransformTestLayer::Init()
{
    engine::Window& x = engine::Application::Get().GetWindow();
    int width = x.GetSize().first;
    int height = x.GetSize().second;
    auto& cam = m_camera.AddComponent<engine::SceneCamera>();
    cam.UpdateViewportSize(width,height);

    auto& rs = m_scene.GetWorld().RegisterSystem<engine::Renderer2DSystem>(cam);

    engine::Texture tex = engine::TextureLoader::LoadFromFilePath("../Engine/assets/images/ogre.png");
    engine::TextureDatabase::AddTexture("ogre", tex);

    /*auto& rootSpr = m_root.AddComponent<engine::Sprite2D>();
    rootSpr.SetTexture(tex);
    m_gos.emplace_back(m_root);*/
    //RootGameObject().Transform().Scale() = { scaling, scaling, 1.0f };

    engine::GameObject m_child = CreateGameObject();
    m_child.Transform().Scale() = { scaling, scaling, 1.0f };
    auto& childSpr = m_child.AddComponent<engine::Sprite2D>();
    childSpr.SetTexture(tex);
    RootGameObject().AddChild(m_child, true);

    m_gos.emplace_back(m_child);

    engine::Entity prev = m_child;

    for (int i = 1; i < 10; ++i)
    {
        engine::GameObject ent = CreateGameObject();

        m_gos.emplace_back(ent);

        ent.Transform().SetPosition({ 1, 1, 0.f });
        auto& objSprite = ent.AddComponent<engine::Sprite2D>();
        objSprite.SetTexture(tex);

        //Nested Add child
        static_cast<engine::GameObject>(prev).AddChild(ent);
        prev = ent;
    }

    //for (int i = 1; i < 10; ++i)
    //{
    //    engine::GameObject ent = CreateGameObject();
    //    m_gos.emplace_back(ent);

    //    auto& objSprite = ent.AddComponent<engine::Sprite2D>();
    //    objSprite.SetTexture(tex);

    //    //Nested Add child
    //    RootGameObject().AddChild(ent, false);
    //}

    // set default controller
    m_controller = m_gos.begin();
    // set target to be controller too.
    m_target = m_controller + 1;
}

void TransformTestLayer::OnUpdate(engine::Timestep dt)
{
    float deltaTime = static_cast<float>(dt);

    engine::WorldManager::SetActiveWorld(m_scene.GetWorld().GetID());

    m_scene.GetWorld().GetSystem<engine::TransformSystem>()->Update();

    if (engine::Input::IsKeyDown(ENGINE_KEY_UP))
    {
        m_controller->Transform().Position().y += MOVESPEED * deltaTime;
    }
    if (engine::Input::IsKeyDown(ENGINE_KEY_DOWN))
    {
        m_controller->Transform().Position().y -= MOVESPEED * deltaTime;
    }
    if (engine::Input::IsKeyDown(ENGINE_KEY_LEFT))
    {
        m_controller->Transform().Position().x -= MOVESPEED * deltaTime;
    }
    if (engine::Input::IsKeyDown(ENGINE_KEY_RIGHT))
    {
        m_controller->Transform().Position().x += MOVESPEED * deltaTime;
    }
    if (engine::Input::IsKeyDown(ENGINE_KEY_Z))
    {
        m_controller->Transform().RotationAngle() -= ROTATIONSPEED * deltaTime;
    }
    if (engine::Input::IsKeyDown(ENGINE_KEY_X))
    {
        m_controller->Transform().RotationAngle() += ROTATIONSPEED * deltaTime;
    }
    if (engine::Input::IsKeyDown(ENGINE_KEY_C))
    {
        m_controller->Transform().Scale().x -= SCALINGSPEED * deltaTime;
    }
    if (engine::Input::IsKeyDown(ENGINE_KEY_V))
    {
        m_controller->Transform().Scale().x += SCALINGSPEED * deltaTime;
    }
    if (engine::Input::IsKeyDown(ENGINE_KEY_B))
    {
        m_controller->Transform().Scale().y -= SCALINGSPEED * deltaTime;
    }
    if (engine::Input::IsKeyDown(ENGINE_KEY_N))
    {
        m_controller->Transform().Scale().y += SCALINGSPEED * deltaTime;
    }

    if (engine::Input::IsKeyPressed(ENGINE_KEY_S))
    {
        if (m_controller == m_gos.begin())
            m_controller = m_gos.end() - 1;
        else
            --m_controller;
    }
    if (engine::Input::IsKeyPressed(ENGINE_KEY_D))
    {
        ++m_controller;
        if (m_controller == m_gos.end()) m_controller = m_gos.begin();
    }


    if (engine::Input::IsKeyPressed(ENGINE_KEY_TAB))
    {
        SelectNewTarget();
    }

    if (engine::Input::IsKeyPressed(ENGINE_KEY_R))
    {
        RootGameObject().Transform().Position() = { 0.f, 0.f, 0.f };
        RootGameObject().Transform().RotationAngle() = 0.f;
        RootGameObject().Transform().Scale() = { scaling, scaling, 1.f };
    }

    if (engine::Input::IsKeyPressed(ENGINE_KEY_P))
    {
        auto vec = RootGameObject().GetChildren();
        for (auto res : vec)
        {
            LOG_INFO(res);
        }
    }
}

void TransformTestLayer::OnImGuiRender()
{
    m_scene.GetWorld().GetSystem<engine::Renderer2DSystem>()->Update();
}
