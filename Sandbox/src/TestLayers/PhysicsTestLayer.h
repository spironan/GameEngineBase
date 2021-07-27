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

#include <Engine.h>

class PhysicsTestLayer : public engine::Layer
{
private:
    engine::World& m_world;
    engine::OrthographicCamera cam{ -1, 1, -1, 1 };
    int width{}, height{};
    engine::GameObject m_root;

    glm::vec2 upperbounds, lowerbounds;
public:

    PhysicsTestLayer()
        : Layer{ "PhysicsTestLayer" }
        , m_world{ engine::WorldManager::CreateWorld() }
        , upperbounds { 200,  200 }
        , lowerbounds {-200, -200 }
    {
        engine::Window& x = engine::Application::Get().GetWindow();
        width = x.GetSize().first;
        height = x.GetSize().second;
        cam.SetProjection(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f);

        auto& ts = m_world.RegisterSystem<engine::TransformSystem>();
        auto& rs = m_world.RegisterSystem<engine::Renderer2DSystem>(cam);
        auto& ps = m_world.RegisterSystem<engine::PhysicsSystem>();

        engine::Texture tex = engine::TextureLoader::LoadFromFilePath("../Engine/assets/images/ogre.png");
        engine::TextureDatabase::AddTexture("ogre", tex);

        m_root.Transform.Scale() = { 50.f, 50.f, 1.0f };
        auto& pc = m_root.AddComponent<engine::RigidBody>();
        pc.SetMass(100.f);
        auto& rootSpr = m_root.AddComponent<engine::Sprite2D>();
        rootSpr.SetTexture(tex);
    }


    virtual void OnUpdate(engine::Timestep dt) override
    {
        m_world.GetSystem<engine::TransformSystem>()->Update();
        m_world.GetSystem<engine::PhysicsSystem>()->Update(dt);
        
        // transform objects
        auto view = m_world.GetComponentView<engine::Transform3D>();
        for (auto& ent : view)
        {
            auto& transform = m_world.GetComponent<engine::Transform3D>(ent);
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
        m_world.GetSystem<engine::Renderer2DSystem>()->Update();
    }

};