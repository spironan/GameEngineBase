/************************************************************************************//*!
\file           SandboxApp.cpp
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 05, 2021
\brief          Customer side of the project that utilizes the functions of the Engine.
                An Editor will be a use case for game engine.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/

// single-include file to get all the relevant functions from our engine.
#include "Engine.h" 
#include "Engine/ECS/ECS_Test.h"
#include <iostream>

#include "Editor/Editor.h"

//TestLayers
#include "TestLayers/InputDebugLayer.h"
#include "TestLayers/TransformTestLayer.h"

#include "Engine/Platform/Vulkan/VulkanContext.h"
#include "Engine/Debug/cvars.h"

class EditorLayer : public engine::Layer
{
private:
    Editor m_editor;
    bool m_demo = true;

public:

    EditorLayer() : Layer{"EditorLayer"}
    {
    }

    virtual void OnUpdate(engine::Timestep dt) override
    {
    }

    //virtual void OnRender(engine::Timestep dt) override
    //{
    //    engine::Renderer2D::BeginScene(camera);
    //    
    //    /*engine::Renderer::DrawQuad(0, 0.5);
    //    engine::Renderer::DrawQuad(0, 0.5);
    //    engine::Renderer::DrawQuad(0, 0.5);
    //    engine::Renderer::DrawQuad(0, 0.5);
    //    engine::Renderer::DrawQuad(0, 0.5);
    //    engine::Renderer::DrawQuad(0, 0.5);*/
    //    //engine::TileMapManager

    //    getcomponent<tilemap>().draw();

    //    engine::TileMapRenderer::Draw(tilemap);

    //    engine::Renderer::EndScene();
    //}

    virtual void OnImGuiRender() override
    {
        m_editor.ShowAllWidgets();
        //ImGui::ShowDemoWindow(&m_demo);
    }
};

class SceneCamera : public engine::Layer
{
private:
    AutoCVar_Vec3 cvar_pos{ "quad.position", "Position X, Y, Z", { 0.5f,0.5f,0.0f }, CVarFlags::EditFloatDrag };
    AutoCVar_Vec3 cvar_col{ "quad.color", "Position X, Y, Z", { 1.0f,1.0f,1.0f }, CVarFlags::EditFloatDrag };
    engine::OrthographicCamera cam{-1,1,-1,1};
public:

    SceneCamera() : Layer{ "SceneCamera" }
    {
        engine::Window& x = engine::Application::Get().GetWindow();
        int width = x.GetSize().first;
        int height = x.GetSize().second;
        cam.SetProjection(0, width, 0, height);
    }

    virtual void OnUpdate(engine::Timestep dt) override
    {


        engine::VulkanContext* x =  reinterpret_cast<engine::VulkanContext*>(engine::Application::Get().GetWindow().GetRenderingContext());
        DebugCamera& cam = *x->getRenderer()->getCam();

        //engine::Renderer2D::BeginScene(cam);
        
     

        //engine::Renderer2D::EndScene();

        //cam.CVAR_ortho.Set(true);
        if (engine::Input::IsMouseButtonDown(ENGINE_MOUSE_BUTTON_RIGHT))
		{
			cam.enable_movement(false);
		}

		if (engine::Input::IsMouseButtonReleased(ENGINE_MOUSE_BUTTON_RIGHT))
		{
			cam.enable_movement(true);
		}


		if (engine::Input::IsKeyDown(ENGINE_KEY_W) || engine::Input::IsKeyDown(ENGINE_KEY_UP))
		{
			cam.move_fowards();
		}
		else if (engine::Input::IsKeyReleased(ENGINE_KEY_W) || engine::Input::IsKeyReleased(ENGINE_KEY_UP))
		{
			cam.move_fowards(false);
		}

		if (engine::Input::IsKeyDown(ENGINE_KEY_S) || engine::Input::IsKeyDown(ENGINE_KEY_DOWN))
		{
			cam.move_backwards();
		}
		else if (engine::Input::IsKeyReleased(ENGINE_KEY_S) || engine::Input::IsKeyReleased(ENGINE_KEY_DOWN))
		{
			cam.move_backwards(false);
		}

		if (engine::Input::IsKeyDown(ENGINE_KEY_A) || engine::Input::IsKeyDown(ENGINE_KEY_LEFT))
		{
			cam.move_left();
		}
		else if (engine::Input::IsKeyReleased(ENGINE_KEY_A) || engine::Input::IsKeyReleased(ENGINE_KEY_LEFT))
		{
			cam.move_left(false);
		}

		if (engine::Input::IsKeyDown(ENGINE_KEY_D) || engine::Input::IsKeyDown(ENGINE_KEY_RIGHT))
		{
			cam.move_right();
		}
		else if (engine::Input::IsKeyReleased(ENGINE_KEY_D) || engine::Input::IsKeyReleased(ENGINE_KEY_RIGHT))
		{
			cam.move_right(false);
		}

		if (engine::Input::IsKeyDown(ENGINE_KEY_Q))
		{
			cam.move_down();
		}
		else if (engine::Input::IsKeyReleased(ENGINE_KEY_Q))
		{
			cam.move_down(false);
		}
		if (engine::Input::IsKeyDown(ENGINE_KEY_E))
		{
			cam.move_up();
		}

		else if (engine::Input::IsKeyReleased(ENGINE_KEY_E))
		{
			cam.move_up(false);
		}
		if (engine::Input::IsKeyDown(ENGINE_KEY_LEFT_SHIFT))
		{
			cam.slow_camera(true);
		}
		else if (engine::Input::IsKeyReleased(ENGINE_KEY_LEFT_SHIFT))
		{
			cam.slow_camera(false);
		}

        auto mDelta = engine::Input::GetMouseDelta();
        if (mDelta.first | mDelta.second)
        {
            cam.update_mouse_relative(mDelta.first, mDelta.second);
        }	

        cam.update_camera(dt);    
       
    }

    virtual void OnImGuiRender() override
    {
        CVarSystem::Get()->DrawImguiEditor();
        
        //temporary rendering
        engine::Renderer2D::BeginScene(cam);

        auto col = cvar_col.Get();
        engine::Renderer2D::DrawQuad({ cvar_pos.Get().x,cvar_pos.Get().y, 1.0f }, {  100.0f,100.0f}, { col.x,col.y,col.z,1.0f });

        for (size_t i = 0; i < 20; i++)
        {
            for (size_t x = 0; x < 20; x++)
            {
                engine::Renderer2D::DrawQuad({ 50.0f* i, 50.0f * x }, { 10.0f,10.0f }, { col.x,col.y,col.z,1.0f });
            }
        }
        engine::Renderer2D::EndScene();

    }
};

class ExampleLayer : public engine::Layer
{
public :
    ExampleLayer(): Layer("Example")
    {
    }

    void OnUpdate(engine::Timestep dt) override
    {
    }

    void OnEvent(engine::Event& e) override
    {
        engine::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<engine::KeyPressedEvent>(ENGINE_BIND_EVENT_FN(ExampleLayer::CloseApp));
    }
    
    bool CloseApp(engine::KeyPressedEvent& e)
    {
        if (e.GetKeyCode() == ENGINE_KEY_ESCAPE)
        {
            engine::Application::Get().Close();
            return true;
        }

        return false;
    }
};

class EditorSceneLayer : public engine::Layer
{
private:
    engine::World& world;
public:

    EditorSceneLayer() : Layer{ "EditorSceneLayer" },
        world(engine::WorldManager::CreateWorld())
    {
        auto& ts = world.RegisterSystem<engine::TransformSystem>();
        
        auto* root = new engine::GameObject();

        for (int i = 0; i < 10; ++i)
        {
            auto* ent = new engine::GameObject();
        }

    }

    virtual void OnUpdate(engine::Timestep dt) override
    {
        world.GetSystem<engine::TransformSystem>()->Update();
    }

};

class Sandbox : public engine::Application
{
public:
    Sandbox(engine::CommandLineArgs args)
        : Application{ "Sandbox" , args }
    {
        // Logging
        LOG_TRACE("Trace Log!");
        LOG_INFO("Info Log!");
        LOG_WARN("Warning Log!");
        LOG_ERROR("Error Log!");
        LOG_CRITICAL("Critical Log!");
        
        ////Default Samples
        PushLayer(new ExampleLayer());      // contains ability to quit progrom with esc
        PushOverlay(new ExampleLayer());

        //Actual Scene Layer
        //PushLayer(new EditorSceneLayer());
        PushOverlay(new EditorLayer());

        // DEBUG/TEST LAYERS
        //PushLayer(new InputDebugLayer());
        PushLayer(new TransformTestLayer());
        //PushOverlay(new SceneCamera());

        // one actual layer - gameplay logic
        // one ui layer - game ui
        // one imgui layer - imgui stuff
            // one heirarchy layer - 
            // one inspector layer - 

    };

    virtual ~Sandbox() = default;
};

engine::Application* engine::CreateApplication(engine::CommandLineArgs args)
{
    return OONEW Sandbox{ args };
}
