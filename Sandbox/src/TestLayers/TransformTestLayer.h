#pragma once

#include <Engine.h>
#include <random>

class TransformTestLayer : public engine::Layer
{
private:
    engine::World& world;
    engine::GameObject* root;
    engine::OrthographicCamera cam{ -1,1,-1,1 };
    int width{}, height{};
public:

    TransformTestLayer() 
        : Layer{ "TransformTestLayer" }
        , world(engine::WorldManager::CreateWorld())
    {
        engine::Window& x = engine::Application::Get().GetWindow();
        width = x.GetSize().first;
        height = x.GetSize().second;
        cam.SetProjection(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f);


        auto& ts = world.RegisterSystem<engine::TransformSystem>();
        auto& rs = world.RegisterSystem<engine::Renderer2DSystem>(cam);
        root = new engine::GameObject();

        engine::Texture tex = engine::TextureLoader::LoadFromFilePath("../Engine/assets/images/ogre.png");
        engine::TextureDatabase::AddTexture("ogre", tex);

        for (int i = 0; i < 1; ++i)
        {
            auto* ent = new engine::GameObject();
            ent->Transform().SetScale({ 40.0f, 40.0f, 1.0f });
            ent->Transform().SetPosition({ -width/2.0f, height/2.0f, 1.0f });
            auto& objSprite = ent->AddComponent<engine::Sprite2D>();
            objSprite.SetTexture(tex);
            
        }

    }

    virtual void OnUpdate(engine::Timestep dt) override
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dis(-1.0f, std::nextafter(1.0f, FLT_MAX));
        
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

        glm::vec4 mouse{ engine::Input::GetMousePosition().first,engine::Input::GetMousePosition().second ,1.0f ,1.0f};
        //mouse = (cam.GetProjectionMatrix()) * mouse;
        mouse.x -= width/2;
        mouse.y = -mouse.y + height/2;

        for (auto& ent : view)
        {
            auto& transform = world.GetComponent<engine::Transform3D>(ent);
            //auto rttrProps = transform.get_type().get_properties();
            //rttrProps[0].set_value(transform, glm::vec3{ 100, 0, 100 });
            //LOG_INFO("ent {0}: position ({1},{2})", ent, transform.Position().x, transform.Position().y);
            //LOG_INFO("ent {0}", transform.IsDirty());
            transform.Position() = mouse;
            transform.Position().x += dis(gen)* 0.2f * ++iteration;
            transform.Position().y += dis(gen)* 0.2f * iteration;
            transform.RotationAngle() += 0.02f*iteration;
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
        ImGui::Begin("OgreImage");
        ImGui::Image((ImTextureID)engine::TextureDatabase::GetTexture("ogre").id, { 200.0f, 200.0f });
        ImGui::End();
    }
};