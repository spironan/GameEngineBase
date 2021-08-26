/************************************************************************************//*!
\file          RenderingTestLayer.h
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 1, 2021
\brief         Environment for testing of rendering with ECS system


Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once


#include "../glm/glm/gtc/type_ptr.hpp"
//#include "../ImGui/ImGuizmo.h"

#include "UtilityLayers/SceneBaseLayer.h"


/****************************************************************************//*!
 @brief     Describes a Test scene used to test The Transform Components
            and Systems Functionality with ECS and Gameobjects.
*//*****************************************************************************/
class RenderingTestLayer : public SceneBaseLayer
{
private:
    engine::GameObject m_child;
    engine::GameObject m_camera;

    std::vector<engine::GameObject> m_gos;
    std::vector<engine::GameObject>::iterator m_controller;
    std::vector<engine::GameObject>::iterator m_target;


    static constexpr float scaling = 1.0f;
    static constexpr float TARGET_ROTATION = 90.f;

public:

    RenderingTestLayer()
        : SceneBaseLayer{ "RenderingTestLayer" }
    {
    }

    virtual void Init() override
    {

        // initilization of camera
        engine::Window& x = engine::Application::Get().GetWindow();
        int width = x.GetSize().first;
        int height = x.GetSize().second;
        float ar = (float)width / height;
        //auto& cam = m_camera.AddComponent<engine::Camera>(true, -100*ar,100*ar,-100,100);

        m_camera = CreateGameObject();
        auto& cam = m_camera.AddComponent<engine::SceneCamera>();
        cam.SetOrthographicSize(100.0f);
        cam.UpdateViewportSize(width, height);
        //cam.SetOrthographic(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f);

        LOG_ENGINE_INFO(cam.GetEntity());
        auto& rs = m_scene.GetWorld().RegisterSystem<engine::Renderer2DSystem>(cam);

        engine::Texture tex = engine::TextureLoader::LoadFromFilePath("../Engine/assets/images/ogre.png");
        engine::TextureDatabase::AddTexture("ogre", tex);

        //base world scaling
        //RootGameObject().Transform().Scale() = { scaling, scaling, 1.0f };

        auto& childSpr = m_child.AddComponent<engine::Sprite2D>();
        childSpr.SetTexture(tex);
        m_child.Transform().Scale() = { scaling, scaling, 1.0f };

        m_gos.emplace_back(m_child);

        engine::Entity prev = m_child;

        for (int i = 1; i < 10; ++i)
        {
            engine::GameObject ent = CreateGameObject();
            m_gos.emplace_back(ent);

            //ent.Transform.Position() = { -1.f, -1.f, 1.f };
            ent.Transform().RotationAngle() += 90.f;
            ent.Transform().Scale() = { scaling, scaling, 1.0f };
            auto& objSprite = ent.AddComponent<engine::Sprite2D>();
            objSprite.SetTexture(tex);

        }

        // set default controller
        m_controller = m_gos.begin();
        // set target to be controller too.
        m_target = m_controller + 1;
    }

    void SelectNewTarget()
    {
        /*m_target->Transform.RotationAngle() += TARGET_ROTATION;*/
        ++m_target;
        if (m_target == m_gos.end())
        {
            m_target = m_gos.begin();
            /*m_target->Transform.RotationAngle() -= TARGET_ROTATION;*/
        }
    }

    static constexpr float MOVESPEED = 30.f;
    static constexpr float ROTATIONSPEED = 10.f;
    static constexpr float SCALINGSPEED = 20.f;

    virtual void OnUpdate(engine::Timestep dt) override
    {
        float deltaTime = static_cast<float>(dt);

        engine::WorldManager::SetActiveWorld(m_scene.GetWorld().GetID());

        m_scene.GetWorld().GetSystem<engine::TransformSystem>()->Update();

        if (engine::Input::IsKeyDown(ENGINE_KEY_UP))
        {
            m_controller->Transform().Position().y += MOVESPEED * deltaTime;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_DOWN))
        {
            m_controller->Transform().Position().y -= MOVESPEED * deltaTime;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_LEFT))
        {
            m_controller->Transform().Position().x -= MOVESPEED * deltaTime;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_RIGHT))
        {
            m_controller->Transform().Position().x += MOVESPEED * deltaTime;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_Z))
        {
            m_controller->Transform().RotationAngle() -= ROTATIONSPEED * deltaTime;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_X))
        {
            m_controller->Transform().RotationAngle() += ROTATIONSPEED * deltaTime;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_C))
        {
            m_controller->Transform().Scale() -= SCALINGSPEED * deltaTime;
        }
        if (engine::Input::IsKeyDown(ENGINE_KEY_V))
        {
            m_controller->Transform().Scale() += SCALINGSPEED * deltaTime;
        }
        if (engine::Input::IsKeyPressed(ENGINE_KEY_S))
        {
            if (m_controller == m_gos.begin())
                m_controller = m_gos.end() - 1;
            else
                --m_controller;
        }
        if (engine::Input::IsKeyPressed(ENGINE_KEY_D))
        {
            ++m_controller;
            if (m_controller == m_gos.end()) m_controller = m_gos.begin();
        }


        if (engine::Input::IsKeyPressed(ENGINE_KEY_TAB))
        {
            SelectNewTarget();
        }

        if (engine::Input::IsKeyPressed(ENGINE_KEY_R))
        {
            RootGameObject().Transform().Position() = { 0.f, 0.f, 0.f };
            RootGameObject().Transform().RotationAngle() = 0.f;
            RootGameObject().Transform().Scale() = { 1.0f, 1.0f, 1.f };
        }


        auto view = m_scene.GetWorld().GetComponentView<engine::Transform3D>();

        for (auto[transform] : view)
        {
            //auto& transform = m_scene.GetWorld().GetComponent<engine::Transform3D>(ent);

            /*LOG_INFO("ent {0}: position ({1},{2})  parent : {3} childs : {4}"
                , ent
                , transform.GetGlobalPosition().x
                , transform.GetGlobalPosition().y
                , static_cast<engine::GameObject>(transform.GetParentId()).GetID()
                , transform.GetChildCount());*/

            /*LOG_INFO("ent {0}: rotation : {1},  globalRotation : {2}"
                , ent
                , transform.GetRotationAngle()
                , transform.GetGlobalRotationDeg()
                );*/

            /*LOG_INFO("ent {0}: scale ({1},{2}) "
                , ent
                , transform.GetGlobalScale().x
                , transform.GetGlobalScale().y
            );*/

            // rttr code below
            //auto rttrProps = transform.get_type().get_properties();
            //rttrProps[0].set_value(transform, glm::vec3{ 100, 0, 100 });
        }
    }

    virtual void OnImGuiRender() override;
    
};

