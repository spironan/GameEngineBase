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

/****************************************************************************//*!
 @brief     Describes a Test scene used to test The Transform Components 
            and Systems Functionality with ECS and Gameobjects.
*//*****************************************************************************/
class TransformTestLayer : public engine::Layer
{
private:
    engine::World& m_world;
    engine::GameObject m_root;

    double timer = 1.0;

public:

    TransformTestLayer() 
        : Layer{ "TransformTestLayer" }
        , m_world{ engine::WorldManager::CreateWorld() }
        , m_root { engine::WorldManager::GetActiveWorld().CreateEntity() }
    {
        engine::WorldManager::SetActiveWorld(m_world.GetID());

        auto& ts = m_world.RegisterSystem<engine::TransformSystem>();

        engine::Entity prev;
        
        for (int i = 0; i < 10; ++i)
        {
            engine::GameObject ent { engine::WorldManager::GetActiveWorld().CreateEntity() };
            
            //auto& trans = ent.GetComponent<engine::Transform3D>();
            //trans.Position().x = 10 * i;

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
    }


    virtual void OnUpdate(engine::Timestep dt) override
    {
        engine::WorldManager::SetActiveWorld(m_world.GetID());

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

        timer -= dt;
        if (timer < 0.f)
        {
            timer = 3.f;

            for (auto& ent : view)
            {
                auto& transform = m_world.GetComponent<engine::Transform3D>(ent);
                transform.Position().x += 1.f;
                
                //transform.Position().y += 1.f;
                
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


    }

    virtual void OnImGuiRender() override
    {
    }
};