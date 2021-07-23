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
    engine::OrthographicCamera cam{ -1, 1, -1, 1 };
    int width{}, height{};
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
        static constexpr float childScale = 50.f;
        
        m_root.Transform.Scale() = { childScale, childScale, 1.0f };
        auto& rootSpr = m_root.AddComponent<engine::Sprite2D>();
        rootSpr.SetTexture(tex);

        //m_child.Transform.Position() = { childScale, childScale, 1.f };
        //m_child.Transform.Scale() = { childScale, childScale, 1.0f };
        //m_child.Transform.RotationAngle() = 90.f;
        auto& childSpr = m_child.AddComponent<engine::Sprite2D>();
        childSpr.SetTexture(tex);
        m_root.AddChild(m_child, true);

        //for (int i = 0; i < 10; ++i)
        //{
        //    if (i == 0)
        //    {
        //        //m_child.Transform.RotationAngle() = 36.f;
        //        auto& objSprite = m_child.AddComponent<engine::Sprite2D>();
        //        objSprite.SetTexture(tex);
        //        m_root.AddChild(m_child);
        //    }
        //    else
        //    {
        //        engine::GameObject ent{ engine::WorldManager::GetActiveWorld().CreateEntity() };
        //        ent.Transform.Scale() = { childScale, childScale, 1.f };
        //        ent.Transform.Position() = { childScale * i , childScale * i, 1.f };
        //        //ent.Transform.RotationAngle() = i * 36.f;
        //        auto& objSprite = ent.AddComponent<engine::Sprite2D>();
        //        objSprite.SetTexture(tex);

        //        /*auto& trans = ent.GetComponent<engine::Transform3D>();
        //        trans.Position().x = 10 * i;*/

        //        /*if (i % 2 == 1)
        //        {
        //            static_cast<engine::GameObject>(prev).AddChild(ent);
        //        }
        //        else
        //        {*/
        //        m_child.AddChild(ent);
        //        //}
        //        prev = ent;
        //    }
        //}
        
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

        // transform 2 info : 1. parentID, 2. number of Children
        // 1. parentID == itself is root
        // 2. 

                    // root child child2 child3 child4 child child child child
        // ID       :   0     A     B       C      D      E     F     G     H
        // parentID :   0     0     A       B      C      0     0     0     0
        // position :   0     1     2       3      4      5     6     7     8
        // vector   :   

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
            m_child.Transform.Position().y += MOVESPEED_CHILD * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_J))
        {
            m_child.Transform.Position().x -= MOVESPEED_CHILD * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_K))
        {
            m_child.Transform.Position().y -= MOVESPEED_CHILD * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_L))
        {
            m_child.Transform.Position().x += MOVESPEED_CHILD * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_U))
        {
            m_child.Transform.RotationAngle() -= ROTATIONSPEED_CHILD * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_O))
        {
            m_child.Transform.RotationAngle() += ROTATIONSPEED_CHILD * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_M))
        {
            m_child.Transform.Scale() -= SCALINGSPEED_CHILD * dt;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_N))
        {
            m_child.Transform.Scale() += SCALINGSPEED_CHILD * dt;
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
