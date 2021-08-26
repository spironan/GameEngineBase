/************************************************************************************//*!
\file          PhysicsTestLayer.h
\project       <PROJECT_NAME>
\author        Chua Teck Lee, c.tecklee, 390008420
\par           email: c.tecklee\@digipen.edu
\date          July 27, 2021
\brief         Test Scene to test physics systems and components. 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine.h"
#include "UtilityLayers/SceneBaseLayer.h"

class PhysicsTestLayer final : public SceneBaseLayer
{
private:
    engine::GameObject m_cam;
    int width{}, height{};

    glm::vec2 upperbounds, lowerbounds;
public:

    PhysicsTestLayer()
        : SceneBaseLayer{ "PhysicsTestLayer" }
        , upperbounds{ 200,  200 }
        , lowerbounds{ -200, -200 }
    {
    }

    void Init() final override
    {
        engine::Window& x = engine::Application::Get().GetWindow();
        width = x.GetSize().first;
        height = x.GetSize().second;

        auto& cam = m_cam.AddComponent<engine::SceneCamera>();
            
        auto& rs = m_scene.GetWorld().RegisterSystem<engine::Renderer2DSystem>(cam);
        auto& ps = m_scene.GetWorld().RegisterSystem<engine::PhysicsSystem>();

        engine::Texture tex = engine::TextureLoader::LoadFromFilePath("../Engine/assets/images/ogre.png");
        engine::TextureDatabase::AddTexture("ogre", tex);

        auto& go = CreateGameObject();
        go.Transform().Scale() = { 50.f, 50.f, 1.0f };
        auto& rb = go.AddComponent<engine::RigidBody>();
        rb.SetMass(100.f);
        auto& rootSpr = go.AddComponent<engine::Sprite2D>();
        rootSpr.SetTexture(tex);
    }

    virtual void OnUpdate(engine::Timestep dt) override
    {
        engine::SceneManager::SetActiveScene(m_scene.GetID());
        m_scene.GetWorld().GetSystem<engine::TransformSystem>()->Update();
        m_scene.GetWorld().GetSystem<engine::PhysicsSystem>()->Update(dt);
        
        // transform objects
        auto view = m_scene.GetWorld().GetComponentView<engine::Transform3D>();
        for (auto& [transform] : view)
        {
            if (transform.Position().y < lowerbounds.y)
            {
                transform.Position().y = upperbounds.y;
            }
            if (transform.Position().y > upperbounds.y)
            {
                transform.Position().y = lowerbounds.y;
            }
            if (transform.Position().x < lowerbounds.x)
            {
                transform.Position().x = upperbounds.x;
            }
            if (transform.Position().x > upperbounds.x)
            {
                transform.Position().x = lowerbounds.x;
            }

        }
    }

    virtual void OnImGuiRender() override
    {
        m_scene.GetWorld().GetSystem<engine::Renderer2DSystem>()->Update();
    }

};
