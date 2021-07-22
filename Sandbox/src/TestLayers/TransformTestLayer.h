#pragma once

#include <Engine.h>

class TransformTestLayer : public engine::Layer
{
private:
    engine::World& world;
    engine::GameObject* root;
    engine::OrthographicCamera cam{ -1,1,-1,1 };
public:

    TransformTestLayer() 
        : Layer{ "TransformTestLayer" }
        , world(engine::WorldManager::CreateWorld())
    {
        engine::Window& x = engine::Application::Get().GetWindow();
        int width = x.GetSize().first;
        int height = x.GetSize().second;
        cam.SetProjection(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f);


        auto& ts = world.RegisterSystem<engine::TransformSystem>();
        auto& rs = world.RegisterSystem<engine::Renderer2DSystem>(cam);
        root = new engine::GameObject();

        for (int i = 0; i < 10; ++i)
        {
            auto* ent = new engine::GameObject();
            ent->Transform().SetScale({ 20.0f, 20.0f, 1.0f });
            ent->Transform().SetPosition({ -width/2.0f, height/2.0f, 1.0f });
            ent->AddComponent<engine::Sprite2D>();
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
            //auto rttrProps = transform.get_type().get_properties();
            //rttrProps[0].set_value(transform, glm::vec3{ 100, 0, 100 });
            //LOG_INFO("ent {0}: position ({1},{2})", ent, transform.Position().x, transform.Position().y);
            //LOG_INFO("ent {0}", transform.IsDirty());
            transform.Position().x += 0.2f * ++iteration;
            transform.Position().y -= 0.2f * iteration;
            transform.RotationAngle() += 0.2f*iteration;
        }

        /*for (auto& ent : view)
        {
            auto& transform = world.GetComponent<engine::Transform3D>(ent);

            LOG_INFO("ent {0}: position ({1},{2})", ent, transform.Position().x, transform.Position().y);
        }*/
    }

    virtual void OnImGuiRender() override
    {
        world.GetSystem<engine::Renderer2DSystem>()->Update();
    }
};