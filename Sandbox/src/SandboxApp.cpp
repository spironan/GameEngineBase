/************************************************************************************//*!
\file           SandboxApp.cpp
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Jul 22, 2021
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
#include "Editor/Editor.h"
//for getting filepath
#include "Editor/EditorFileGroup.h"
//TestLayers
#include "TestLayers/InputDebugLayer.h"
#include "TestLayers/TransformTestLayer.h"
#include "TestLayers/GameObjectTestLayer.h"
#include "TestLayers/PhysicsTestLayer.h"
#include "TestLayers/RenderingTestLayer.h"
#include "TestLayers/ScriptingTestLayer.h"

#include "Engine/Platform/Vulkan/VulkanContext.h"
#include "Engine/Debug/cvars.h"

#include "Engine/Asset/TextureLoader.h"
#include "Engine/Asset/AssetsManager.h"

class EditorLayer : public engine::Layer
{
private:
    Editor m_editor;
    bool m_demo = true;

public:

    EditorLayer() : Layer{"EditorLayer"}
    {
		LOG_WARN("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
		using namespace engine;
		TextureDatabase::AddTexture("Ouroboros_Folder",TextureLoader::LoadFromFilePath("Icons/Ouroboros_Folder.png"));
		TextureDatabase::AddTexture("Ouroboros_GenericFile", TextureLoader::LoadFromFilePath("Icons/Ouroboros_GenericFile.png"));
		TextureDatabase::AddTexture("Ouroboros_MP3", TextureLoader::LoadFromFilePath("Icons/Ouroboros_MP3.png"));
		TextureDatabase::AddTexture("Ouroboros_PNG", TextureLoader::LoadFromFilePath("Icons/Ouroboros_PNG.png"));
		TextureDatabase::AddTexture("Ouroboros_WAV", TextureLoader::LoadFromFilePath("Icons/Ouroboros_WAV.png"));
		TextureDatabase::AddTexture("Ouroboros_Prefab", TextureLoader::LoadFromFilePath("Icons/Ouroboros_Prefab.png"));

		TextureDatabase::AddTexture("Ouroboros_Search_Icon_Black", TextureLoader::LoadFromFilePath("Icons/Ouroboros_Search_Icon_Black.png"));
		
		TextureDatabase::AddTexture("Ouroboros_Log_Icon_Black", TextureLoader::LoadFromFilePath("Icons/Ouroboros_Log_Icon_Black.png"));
		TextureDatabase::AddTexture("Ouroboros_Warning_Icon_Yellow", TextureLoader::LoadFromFilePath("Icons/Ouroboros_Warning_Icon_Yellow.png"));
		TextureDatabase::AddTexture("Ouroboros_Error_Icon_Red", TextureLoader::LoadFromFilePath("Icons/Ouroboros_Error_Icon_Red.png"));

	}

    virtual void OnUpdate(engine::Timestep dt) override
    {
    }

    virtual void OnImGuiRender() override
    {
        m_editor.ShowAllWidgets();
        //ImGui::ShowDemoWindow(&m_demo);
    }
};

class SceneCamera : public engine::Layer
{
private:

public:

    SceneCamera() : Layer{ "SceneCamera" }
    {
    }

    virtual void OnUpdate(engine::Timestep dt) override
    {
        engine::VulkanContext* x =  reinterpret_cast<engine::VulkanContext*>(engine::Application::Get().GetWindow().GetRenderingContext());
        DebugCamera& cam = *x->getRenderer()->getCam();
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

		cam.update_camera((float)dt);
       
    }

    virtual void OnImGuiRender() override
    {
        CVarSystem::Get()->DrawImguiEditor();
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
    engine::World& m_world;

    engine::GameObject m_root;

public:

    EditorSceneLayer() 
        : Layer{ "EditorSceneLayer" }
        , m_world(engine::WorldManager::CreateWorld())
        , m_root {}
    {
        engine::WorldManager::SetActiveWorld(m_world.GetID());

        auto& ts = m_world.RegisterSystem<engine::TransformSystem>();
        
        for (int i = 0; i < 10; ++i)
        {
            engine::GameObject ent{};
            m_root.AddChild(ent);
        }
    }

    ~EditorSceneLayer()
    {
    }

    virtual void OnUpdate(engine::Timestep dt) override
    {
        engine::WorldManager::SetActiveWorld(m_world.GetID());
        m_world.GetSystem<engine::TransformSystem>()->Update();
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
        PushLayer(new EditorSceneLayer());
        PushOverlay(new EditorLayer());

        // DEBUG/TEST LAYERS
        //PushLayer(new InputDebugLayer());
        //PushLayer(new TransformTestLayer());
        //PushOverlay(new SceneCamera());
        //PushLayer(new GameObjectTestLayer());
        //PushLayer(new PhysicsTestLayer());
        //PushLayer(new ScriptingTestLayer());

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
