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
    engine::OrthographicCamera cam{ -1,1,-1,1 };
    int width{}, height{};
    double timer = 1.0;
public:

    TransformTestLayer() 
        : Layer{ "TransformTestLayer" }
        , m_world{ engine::WorldManager::CreateWorld() }
        , m_root { engine::WorldManager::GetActiveWorld().CreateEntity() }
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
        for (int i = 0; i < 10; ++i)
        {
            engine::GameObject ent{ engine::WorldManager::GetActiveWorld().CreateEntity() };
            ent.Transform.SetScale({ 40.0f, 40.0f, 1.0f });
            auto& objSprite =  ent.AddComponent<engine::Sprite2D>();
            objSprite.SetTexture(tex);
            auto& trans = ent.GetComponent<engine::Transform3D>();
            trans.Position().x = 10 * i;

            if (i % 2 == 1)
            {
                static_cast<engine::GameObject>(prev).AddChild(ent);
            }
            else
            {
               m_root.AddChild(ent);
            }

            prev = ent;
        }
       
        

        for (int i = 0; i < 50; ++i)
        {
            engine::GameObject ent = engine::GameObject();
            m_root.AddChild(ent);
            ent.Transform.SetScale({ 40.0f, 40.0f, 1.0f });
            ent.Transform.SetPosition({ -width / 2.0f, height / 2.0f, 1.0f });
            auto& objSprite = ent.AddComponent<engine::Sprite2D>();
            objSprite.SetTexture(tex);

        }
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

        auto view = m_world.GetComponentView<engine::Transform3D>();

        float iteration{ 1.0f };
        timer -= dt;
        if (timer < 0.f)
        {
            timer = 3.f;
            //m_root.Transform.Position().x +=0.0f;
            for (auto& ent : view)
            {
               auto& transform = m_world.GetComponent<engine::Transform3D>(ent);
               transform.Position().x += 1.f*iteration;

                transform.Position().y += 1.f*iteration++;

                // rttr code below
                //auto rttrProps = transform.get_type().get_properties();
                //rttrProps[0].set_value(transform, glm::vec3{ 100, 0, 100 });
                LOG_INFO("ent {0}: position ({1},{2})  parent : {3} childs : {4}"
                         , ent
                         , transform.GetGlobalPosition().x
                         , transform.GetGlobalPosition().y
                         , static_cast<engine::GameObject>(transform.GetParentId()).GetID()
                         , transform.GetChildCount());
            }
        }
        glm::vec4 mouse{ engine::Input::GetMousePosition().first,engine::Input::GetMousePosition().second ,1.0f ,1.0f};
        //mouse = (cam.GetProjectionMatrix()) * mouse;
        mouse.x -= width/2;
        mouse.y = -mouse.y + height/2;

        for (auto& ent : view)
        {
            auto& transform = m_world.GetComponent<engine::Transform3D>(ent);
            //m_root.Transform.Position() = mouse;
            //auto rttrProps = transform.get_type().get_properties();
            //rttrProps[0].set_value(transform, glm::vec3{ 100, 0, 100 });
            //LOG_INFO("ent {0}: position ({1},{2})", ent, transform.Position().x, transform.Position().y);
            //LOG_INFO("ent {0}", transform.IsDirty());
           //transform.Position() = mouse;
           //transform.Position().x += dis(gen)* 0.2f * ++iteration;
           //transform.Position().y += dis(gen)* 0.2f * iteration;
           //transform.RotationAngle() += 0.02f*iteration;
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
