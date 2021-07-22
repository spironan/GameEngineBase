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
    engine::GameObject* m_root;

public:

    TransformTestLayer() 
        : Layer{ "TransformTestLayer" }
        , m_world(engine::WorldManager::CreateWorld())
    {
        auto& ts = m_world.RegisterSystem<engine::TransformSystem>();

        m_root = new engine::GameObject();

        /*for (int i = 0; i < 1; ++i)
        {
            auto* ent = new engine::GameObject();
        }*/

    }

    ~TransformTestLayer()
    {
        delete m_root;
    }

    virtual void OnUpdate(engine::Timestep dt) override
    {
        m_world.GetSystem<engine::TransformSystem>()->Update();

        auto view = m_world.GetComponentView<engine::Transform3D>();

        // transform 2 info : 1. parentID, 2. number of Children
        // 1. parentID == itself is root
        // 2. 

                    // root child child2 child3 child4 child child child child
        // ID       :   0     A     B       C      D      E     F     G     H
        // parentID :   0     0     A       B      C      0     0     0     0
        // position :   0     1     2       3      4      5     6     7     8

        // vector   :   

        int iteration = 0;

        for (auto& ent : view)
        {
            auto& transform = m_world.GetComponent<engine::Transform3D>(ent);
            auto rttrProps = transform.get_type().get_properties();
            rttrProps[0].set_value(transform, glm::vec3{ 100, 0, 100 });
            //LOG_INFO("ent {0}", transform.IsDirty());
            //LOG_INFO("ent {0}: position ({1},{2})", ent, transform.GetPosition().x, transform.GetPosition().y);
            /*transform.Position().x += 1.f * ++iteration;
            transform.Position().y -= 1.f * iteration;*/
        }

        /*for (auto& ent : view)
        {
            auto& transform = world.GetComponent<engine::Transform3D>(ent);

            LOG_INFO("ent {0}: position ({1},{2})", ent, transform.Position().x, transform.Position().y);
        }*/
    }

    virtual void OnImGuiRender() override
    {
    }
};