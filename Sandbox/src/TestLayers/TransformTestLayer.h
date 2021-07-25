/************************************************************************************//*!
\file           TransformTestLayer.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           June 22, 2021
\brief          Describes a Test scene used to test The Transform Components and Systems
                Functionality with ECS and Gameobjects.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include <Engine.h>
#include <random>

/****************************************************************************//*!
 @brief     Describes a Test scene used to test The Transform Components 
            and Systems Functionality with ECS and Gameobjects.
*//*****************************************************************************/
class TransformTestLayer : public engine::Layer
{
private:
    engine::World& m_world;
    engine::GameObject m_root;
    engine::GameObject m_child;
    
    std::vector<engine::GameObject> m_gos;
    std::vector<engine::GameObject>::iterator m_controller;

    engine::OrthographicCamera cam{ -1, 1, -1, 1 };
    int width{}, height{};

    static constexpr float scaling = 50.f;

public:

    TransformTestLayer() 
        : Layer{ "TransformTestLayer" }
        , m_world{ engine::WorldManager::CreateWorld() }
        , m_root { engine::WorldManager::GetActiveWorld().CreateEntity() }
        , m_child{ engine::WorldManager::GetActiveWorld().CreateEntity() }
    {

        engine::Window& x = engine::Application::Get().GetWindow();
        width = x.GetSize().first;
        height = x.GetSize().second;
        cam.SetProjection(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f);

        auto& ts = m_world.RegisterSystem<engine::TransformSystem>();
        auto& rs = m_world.RegisterSystem<engine::Renderer2DSystem>(cam);

        engine::Texture tex = engine::TextureLoader::LoadFromFilePath("../Engine/assets/images/ogre.png");
        engine::TextureDatabase::AddTexture("ogre", tex);

        engine::Entity prev;
        
        m_root.Transform.Scale() = { scaling, scaling, 1.0f };
        auto& rootSpr = m_root.AddComponent<engine::Sprite2D>();
        rootSpr.SetTexture(tex);
        
        m_gos.emplace_back(m_root);

        m_child.Transform.Position() = { 1.f, 1.f, 1.f };
        //m_child.Transform.Scale() = { childScale, childScale, 1.0f };
        //m_child.Transform.RotationAngle() = 90.f;
        auto& childSpr = m_child.AddComponent<engine::Sprite2D>();
        childSpr.SetTexture(tex);
        m_root.AddChild(m_child, true);
        
        prev = m_child;

        m_gos.emplace_back(m_child);

        for (int i = 1; i < 10; ++i)
        {
            engine::GameObject ent{ engine::WorldManager::GetActiveWorld().CreateEntity() };

            m_gos.emplace_back(ent);

            //ent.Transform.Scale() = { childScale, childScale, 1.f };
            ent.Transform.Position() = { 1.f, 1.f, 1.f };
            //ent.Transform.RotationAngle() = i * 36.f;
            auto& objSprite = ent.AddComponent<engine::Sprite2D>();
            objSprite.SetTexture(tex);
            
            if(prev != m_child.GetID())
                static_cast<engine::GameObject>(prev).AddChild(ent);
            else
                m_child.AddChild(ent);

            prev = ent;
        }

        // set default controller
        m_controller = m_gos.begin();

       /* int iteration = 0;
        auto view = m_world.GetComponentView<engine::Transform3D>();
        for (auto& ent : view)
        {
            auto& tf = m_world.GetComponent<engine::Transform3D>(ent);
            tf.Position() = { childScale * iteration , childScale * iteration, 1.f };
            ++iteration;
        }*/

    }


    virtual void OnUpdate(engine::Timestep dt) override
    {
        engine::WorldManager::SetActiveWorld(m_world.GetID());
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dis(-1.0f, std::nextafter(1.0f, FLT_MAX));
        
        m_world.GetSystem<engine::TransformSystem>()->Update();

        if (engine::Input::IsKeyPressed(ENGINE_KEY_TAB))
        {
            ++m_controller;
            if (m_controller == m_gos.end()) m_controller = m_gos.begin();
        }

        if (engine::Input::IsKeyPressed(ENGINE_KEY_1))
        {
            m_root.Transform.Position() = { 0.f,0.f,0.f };
            m_root.Transform.RotationAngle() = 0.f;
            m_root.Transform.Scale() = { scaling, scaling, 1.f };

            m_child.Transform.Position() = { 0.f, 0.f, 0.f };
            m_child.Transform.RotationAngle() = 0.f;
            m_child.Transform.Scale() = { 1.f, 1.f, 1.f };
        }

        static constexpr float MOVESPEED_PARENT     = 300.f;
        static constexpr float ROTATIONSPEED_PARENT = 10.f;
        static constexpr float SCALINGSPEED_PARENT  = 20.f;

        static constexpr float MOVESPEED_CHILD      = 30.f;
        static constexpr float ROTATIONSPEED_CHILD  = 10.f;
        static constexpr float SCALINGSPEED_CHILD   = 10.f;

        if (engine::Input::IsKeyDown(ENGINE_KEY_W))
        {
            m_root.Transform.Position().y += MOVESPEED_PARENT * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_A))
        {
            m_root.Transform.Position().x -= MOVESPEED_PARENT * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_S))
        {
            m_root.Transform.Position().y -= MOVESPEED_PARENT * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_D))
        {
            m_root.Transform.Position().x += MOVESPEED_PARENT * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_Q))
        {
            m_root.Transform.RotationAngle() -= ROTATIONSPEED_PARENT * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_E))
        {
            m_root.Transform.RotationAngle() += ROTATIONSPEED_PARENT * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_R))
        {
            m_root.Transform.Scale() -= SCALINGSPEED_PARENT * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_T))
        {
            m_root.Transform.Scale() += SCALINGSPEED_PARENT * dt;
        }


        if (engine::Input::IsKeyDown(ENGINE_KEY_I))
        {
            m_controller->Transform.Position().y += MOVESPEED_CHILD * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_J))
        {
            m_controller->Transform.Position().x -= MOVESPEED_CHILD * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_K))
        {
            m_controller->Transform.Position().y -= MOVESPEED_CHILD * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_L))
        {
            m_controller->Transform.Position().x += MOVESPEED_CHILD * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_U))
        {
            m_controller->Transform.RotationAngle() -= ROTATIONSPEED_CHILD * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_O))
        {
            m_controller->Transform.RotationAngle() += ROTATIONSPEED_CHILD * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_M))
        {
            m_controller->Transform.Scale() -= SCALINGSPEED_CHILD * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_N))
        {
            m_controller->Transform.Scale() += SCALINGSPEED_CHILD * dt;
        }

        auto view = m_world.GetComponentView<engine::Transform3D>();

        for (auto& ent : view)
        {
            auto& transform = m_world.GetComponent<engine::Transform3D>(ent);

            LOG_INFO("ent {0}: rotation : {1},  globalRotation : {2}"
                , ent
                , transform.GetRotationAngle()
                , transform.GetGlobalRotationDeg()
                );

            // rttr code below
            //auto rttrProps = transform.get_type().get_properties();
            //rttrProps[0].set_value(transform, glm::vec3{ 100, 0, 100 });
           /* LOG_INFO("ent {0}: position ({1},{2})  parent : {3} childs : {4}"
                        , ent
                        , transform.GetGlobalPosition().x
                        , transform.GetGlobalPosition().y
                        , static_cast<engine::GameObject>(transform.GetParentId()).GetID()
                        , transform.GetChildCount());*/
        }
    }

    virtual void OnImGuiRender() override
    {
        m_world.GetSystem<engine::Renderer2DSystem>()->Update();
        ImGui::Begin("OgreImage");
        ImGui::Image((ImTextureID)engine::TextureDatabase::GetTexture("ogre").id, { 200.0f, 200.0f });
        ImGui::End();
    }
};
