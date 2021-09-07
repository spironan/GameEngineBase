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
#include "PhysicsTestLayer.h"

PhysicsTestLayer::PhysicsTestLayer()
    : SceneBaseLayer{ "PhysicsTestLayer" }
    , m_upperbounds{ 500,  200 }
    , m_lowerbounds{ -500, -200 }
    , m_spawnPosController { 0, 0, 0 }
    , m_spawnPosOther { 75, 0, 0 }
    , m_controller{ }
    , m_other{ }
{
}

void PhysicsTestLayer::Init()
{
    auto ogreHandle = engine::AssetManager::ImportAsset("../Engine/assets/images/ogre.png");
    auto tex = engine::AssetManager::GetAsset<engine::Texture>(ogreHandle);

    DefaultCamera().SetOrthographic(1000);
    auto& rs = GetWorld()->RegisterSystem<engine::Renderer2DSystem>(DefaultCamera());
    auto& ps = GetWorld()->RegisterSystem<engine::PhysicsSystem>();

    {
        m_controller = CreateGameObject();
        m_controller.Transform().Scale() = { 50.f, 50.f, 1.0f };
        m_controller.Transform().Position() = m_spawnPosController;
        m_controller.AddComponent<engine::Sprite2D>();

        auto& pc = m_controller.AddComponent<engine::Rigidbody2D>();
        pc.SetMass(1.f);
        pc.GravityScale = 0.0f;
        RootGameObject().AddChild(m_controller);

        m_controller.AddComponent<engine::BoxCollider2D>();
        auto& c = m_controller.GetComponent<engine::Collider2D>();
        //c.IsTrigger = true;
        
        //c.SetNarrowPhaseCollider(engine::ColliderType::BOX);
        //m_controller.AddComponent<engine::BoxCollider2D>();

        c.OnTriggerEnter +=
            [=](auto const& manifolds) 
        { 
            m_controller.GetComponent<engine::Sprite2D>().SetColor(oom::vec4{ 0, 0.5, 0, 1 });
            LOG_TRACE("ON TRIGGER ENTER");
        };
        c.OnTriggerStay +=
            [=](auto const& manifolds)
        {
            m_controller.GetComponent<engine::Sprite2D>().SetColor(oom::vec4{ 0, 0, 0.5, 1 });
            LOG_TRACE("ON TRIGGER STAY");
        };
        c.OnTriggerExit +=
            [=](auto const& manifolds)
        {
            m_controller.GetComponent<engine::Sprite2D>().SetColor(oom::vec4{ 0.5, 0, 0, 1 });
            LOG_TRACE("ON TRIGGER EXIT");
        };
        c.OnCollisionEnter +=
            [=](auto const& manifolds)
        {
            m_controller.GetComponent<engine::Sprite2D>().SetColor(oom::vec4{ 0, 0.5, 0.5, 1 });
            LOG_TRACE("ON COLLISION ENTER");
        };
        c.OnCollisionStay +=
            [=](auto const& manifolds)
        {
            m_controller.GetComponent<engine::Sprite2D>().SetColor(oom::vec4{ 0.5, 0.5, 0, 1 });
            LOG_TRACE("ON COLLISION STAY");
        };
        c.OnCollisionExit +=
            [=](auto const& manifolds)
        {
            m_controller.GetComponent<engine::Sprite2D>().SetColor(oom::vec4{ 0, 0.5, 0.5, 1 });
            LOG_TRACE("ON COLLISION EXIT");
        };

        
    }

    {
        m_other = CreateGameObject();
        m_other.Transform().Position() = m_spawnPosOther;
        m_other.Transform().Scale() = { 50.f, 50.f, 1.0f };
        m_other.AddComponent<engine::Sprite2D>();
        
        auto& pc = m_other.AddComponent<engine::Rigidbody2D>();
        pc.SetMass(1000.f);
        pc.GravityScale = 0.0f;
        RootGameObject().AddChild(m_other);

        m_other.AddComponent<engine::BoxCollider2D>();
        auto& c = m_other.GetComponent<engine::Collider2D>();
        //c.IsTrigger = true;
        c.OnTriggerEnter +=
            [=](auto const& manifolds)
        {
            m_other.GetComponent<engine::Sprite2D>().SetColor(oom::vec4{ 1, 0, 0, 1 });
            LOG_TRACE("ON TRIGGER ENTER");
        };
        c.OnTriggerStay +=
            [=](auto const& manifolds)
        {
            m_other.GetComponent<engine::Sprite2D>().SetColor(oom::vec4{ 0, 1, 0, 1 });
            LOG_TRACE("ON TRIGGER STAY");
        };
        c.OnTriggerExit +=
            [=](auto const& manifolds)
        {
            m_other.GetComponent<engine::Sprite2D>().SetColor(oom::vec4{ 0, 0, 1, 1 });
            LOG_TRACE("ON TRIGGER EXIT");
        };
        c.OnCollisionEnter +=
            [=](auto const& manifolds)
        {
            m_other.GetComponent<engine::Sprite2D>().SetColor(oom::vec4{ 1, 1, 0, 1 });
            LOG_TRACE("ON COLLISION ENTER");
        };
        c.OnCollisionStay +=
            [=](auto const& manifolds)
        {
            m_other.GetComponent<engine::Sprite2D>().SetColor(oom::vec4{ 0, 1, 1, 1 });
            LOG_TRACE("ON COLLISION STAY");
        };
        c.OnCollisionExit +=
            [=](auto const& manifolds)
        {
            m_other.GetComponent<engine::Sprite2D>().SetColor(oom::vec4{ 1, 0, 1, 1 });
            LOG_TRACE("ON COLLISION EXIT");
        };
        //c.SetNarrowPhaseCollider(engine::ColliderType::BOX);
        //m_other.AddComponent<engine::BoxCollider2D>();
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


void PhysicsTestLayer::OnUpdate(engine::Timestep dt)
{
    GetWorld()->GetSystem<engine::TransformSystem>()->Update();
    GetWorld()->GetSystem<engine::PhysicsSystem>()->Update(dt);

    //reset
    if (engine::Input::IsKeyDown(ENGINE_KEY_R))
    {
        m_controller.Transform().Position() = m_spawnPosController;
        m_controller.GetComponent<engine::Rigidbody2D>().SetForce({ 0,0 });
        m_other.Transform().Position() = m_spawnPosOther;
        m_other.GetComponent<engine::Rigidbody2D>().SetForce({ 0,0 });
    }

    constexpr float force = 300.f;
    constexpr float jumpforce = 10000.f;
    


    if (engine::Input::IsKeyHeld(ENGINE_KEY_UP))
    {
        m_controller.GetComponent<engine::Rigidbody2D>().ApplyForce(oom::vec2{ 0, force });
    }

    if (engine::Input::IsKeyHeld(ENGINE_KEY_DOWN))
    {
        m_controller.GetComponent<engine::Rigidbody2D>().ApplyForce(oom::vec2{ 0, -force });
    }

    if (engine::Input::IsKeyHeld(ENGINE_KEY_RIGHT))
    {
        m_controller.GetComponent<engine::Rigidbody2D>().ApplyForce(oom::vec2{ force, 0 });
    }

    if (engine::Input::IsKeyHeld(ENGINE_KEY_LEFT))
    {
        m_controller.GetComponent<engine::Rigidbody2D>().ApplyForce(oom::vec2{ -force, 0 });
    }

    if (engine::Input::IsKeyPressed(ENGINE_KEY_SPACE))
    {
        m_controller.GetComponent<engine::Rigidbody2D>().ApplyForce(oom::vec2{ 0, jumpforce });
    }

    /*auto rootsForce = m_controller.GetComponent<engine::Rigidbody2D>().GetForce();
    LOG_TRACE("{0}{1}", rootsForce.x, rootsForce.y );*/

    // transform objects
    auto view = GetWorld()->GetComponentView<engine::Transform3D>();
    for (auto [transform] : view)
    {
        //auto& transform = GetWorld()->GetComponent<engine::Transform3D>(ent);
        if (transform.Position().y < m_lowerbounds.y)
        {
            transform.Position().y = m_upperbounds.y;
        }
        if (transform.Position().y > m_upperbounds.y)
        {
            transform.Position().y = m_lowerbounds.y;
        }
        if (transform.Position().x < m_lowerbounds.x)
        {
            transform.Position().x = m_upperbounds.x;
        }
        if (transform.Position().x > m_upperbounds.x)
        {
            transform.Position().x = m_lowerbounds.x;
        }

    }
}

void PhysicsTestLayer::OnImGuiRender()
{
    GetWorld()->GetSystem<engine::Renderer2DSystem>()->Update();
}

