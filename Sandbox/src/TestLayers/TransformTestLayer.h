#pragma once

#include <Engine.h>

class TransformTestLayer : public engine::Layer
{
private:
    engine::World& world;
    engine::GameObject* root;

public:

    TransformTestLayer() 
        : Layer{ "TransformTestLayer" }
        , world(engine::WorldManager::CreateWorld())
    {
        auto& ts = world.RegisterSystem<engine::TransformSystem>();

        root = new engine::GameObject();

        for (int i = 0; i < 1; ++i)
        {
            auto* ent = new engine::GameObject();
        }

    }

    virtual void OnUpdate(engine::Timestep dt) override
    {
        world.GetSystem<engine::TransformSystem>()->Update();

        auto view = world.GetComponentView<engine::Transform3D>();

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
            auto& transform = world.GetComponent<engine::Transform3D>(ent);
            auto rttrProps = transform.get_type().get_properties();
            rttrProps[0].set_value(transform, glm::vec3{ 100, 0, 100 });
            //LOG_INFO("ent {0}: position ({1},{2})", ent, transform.Position().x, transform.Position().y);
            LOG_INFO("ent {0}", transform.IsDirty());
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