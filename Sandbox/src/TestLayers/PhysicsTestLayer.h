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

#include "UtilityLayers/SceneBaseLayer.h"

class PhysicsTestLayer : public SceneBaseLayer
{
private:
    //engine::World& m_scene.GetWorld();
    engine::OrthographicCamera cam{ -1, 1, -1, 1 };
    int width{}, height{};
    engine::GameObject m_second, m_third;

    glm::vec2 upperbounds, lowerbounds;

public:

    PhysicsTestLayer()
        : SceneBaseLayer{ "PhysicsTestLayer" }
        , upperbounds   { 500,  200 }
        , lowerbounds   {-500, -200 }
        , m_second      { }
        , m_third       { }
    {
    }

    virtual void Init() override
    {
        engine::WorldManager::SetActiveWorld(m_scene.GetWorld().GetID());

        engine::Window& x = engine::Application::Get().GetWindow();
        width = x.GetSize().first;
        height = x.GetSize().second;
        cam.SetProjection(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f);

        auto& rs = m_scene.GetWorld().RegisterSystem<engine::Renderer2DSystem>(cam);
        auto& ps = m_scene.GetWorld().RegisterSystem<engine::PhysicsSystem>();

        {
            m_second = CreateGameObject();
            m_second.Transform().Scale() = { 50.f, 50.f, 1.0f };
            m_second.AddComponent<engine::Sprite2D>();
            //m_second.AddComponent<engine::BoxCollider2D>();
            m_second.AddComponent<engine::CircleCollider2D>();
            auto& pc = m_second.AddComponent<engine::Rigidbody2D>();
            pc.SetMass(1.f);
            pc.GravityScale = 0.0f;
            RootGameObject().AddChild(m_second);
        }

        {
            m_third = CreateGameObject();
            m_third.Transform().Position() = { 100.f, 0.f, 0.f };
            m_third.Transform().Scale() = { 50.f, 50.f, 1.0f };
            m_third.AddComponent<engine::Sprite2D>();
            //m_third.AddComponent<engine::BoxCollider2D>();
            m_third.AddComponent<engine::CircleCollider2D>();
            auto& pc = m_third.AddComponent<engine::Rigidbody2D>();
            pc.SetMass(1.f);
            pc.GravityScale = 0.0f;
            RootGameObject().AddChild(m_third);
        }

        ////Ground
        //{
        //    m_floor.Transform.Position() = { 0.f, -150.f, 0.f };
        //    m_floor.Transform.Scale() = { 1000.f, 5.f, 1.0f };
        //    m_floor.AddComponent<engine::Sprite2D>();
        //    m_floor.AddComponent<engine::BoxCollider2D>();
        //    auto& pc = m_floor.AddComponent<engine::Rigidbody2D>();
        //    /*pc.SetMass(1.f);*/
        //    pc.BodyType = engine::BodyType::STATIC;
        //    pc.GravityScale = 0.0f;
        //}
    }


    virtual void OnUpdate(engine::Timestep dt) override
    {
        engine::WorldManager::SetActiveWorld(m_scene.GetWorld().GetID());
        m_scene.GetWorld().GetSystem<engine::TransformSystem>()->Update();
        m_scene.GetWorld().GetSystem<engine::PhysicsSystem>()->Update(dt);

        constexpr float force   = 300.f;
        constexpr float jumpforce = 10000.f;

        if (engine::Input::IsKeyDown(ENGINE_KEY_UP))
        {
            m_second.GetComponent<engine::Rigidbody2D>().ApplyForce(glm::vec2{ 0, force });
        }
        
        if (engine::Input::IsKeyDown(ENGINE_KEY_DOWN))
        {
            m_second.GetComponent<engine::Rigidbody2D>().ApplyForce(glm::vec2{ 0, -force });
        }
        
        if (engine::Input::IsKeyDown(ENGINE_KEY_RIGHT))
        {
            m_second.GetComponent<engine::Rigidbody2D>().ApplyForce(glm::vec2{ force, 0 });
        }
        
        if (engine::Input::IsKeyDown(ENGINE_KEY_LEFT))
        {
            m_second.GetComponent<engine::Rigidbody2D>().ApplyForce(glm::vec2{ -force, 0 });
        }

        if (engine::Input::IsKeyPressed(ENGINE_KEY_SPACE))
        {
            m_second.GetComponent<engine::Rigidbody2D>().ApplyForce(glm::vec2{ 0, jumpforce });
        }

        /*auto rootsForce = m_second.GetComponent<engine::Rigidbody2D>().GetForce();
        LOG_TRACE("{0}{1}", rootsForce.x, rootsForce.y );*/

        // transform objects
        auto view = m_scene.GetWorld().GetComponentView<engine::Transform3D>();
        for (auto[transform] : view)
        {
            //auto& transform = m_scene.GetWorld().GetComponent<engine::Transform3D>(ent);
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
        engine::WorldManager::SetActiveWorld(m_scene.GetWorld().GetID());
        m_scene.GetWorld().GetSystem<engine::Renderer2DSystem>()->Update();
    }

};
