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

#include <iostream>

#include "Editor/Editor.h"

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

        cam.update_camera(dt);    
       
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
        //LOG_INFO("ExampleLayer::Update {0}s {1}ms", dt.GetSeconds(), dt.GetMilliSeconds());
        // Commenting this out for now until engine::Input::GetMouseDelta() no longer consumes the information
        //std::pair<int, int> pos = engine::Input::GetMouseDelta();
        //LOG_INFO("{0}, {1}", pos.first , pos.second);
        
        // New way to do check keys.
        if (engine::Input::IsKeyPressed(ENGINE_KEY_0))
        {
            LOG_TRACE("key 0 Pressed ");
        }

        if (engine::Input::IsAnyKeyDown())
        {
            for (engine::KeyCode key : engine::Input::GetKeysDown())
            {
                LOG_TRACE("Key {0} down", key);
            }
        }

        if (engine::Input::IsAnyKeyPressed())
        {
            for (engine::KeyCode key : engine::Input::GetKeysPressed())
            {
                LOG_TRACE("Key {0} pressed", key);
            }
        }

        if (engine::Input::IsAnyKeyReleased())
        {
            for (engine::KeyCode key : engine::Input::GetKeysReleased())
            {
                LOG_TRACE("Key {0} released", key);
            }
        }

        if (engine::Input::IsAnyMouseButtonDown())
        {
            for (engine::MouseCode mousecode : engine::Input::GetMouseButtonsDown())
            {
                LOG_TRACE("Mouse Button {0} Down", mousecode);
            }
        }

        if (engine::Input::IsAnyMouseButtonPressed())
        {
            for (engine::MouseCode mousecode : engine::Input::GetMouseButtonsPressed())
            {
                LOG_TRACE("Mouse Button {0} Pressed", mousecode);
            }
        }

        if (engine::Input::IsAnyMouseButtonReleased())
        {
            for (engine::MouseCode mousecode : engine::Input::GetMouseButtonsReleased())
            {
                LOG_TRACE("Mouse Button {0} Released", mousecode);
            }
        }

        //if (engine::Input::IsKeyDown(engine::key::W))
        //{
        //    LOG_TRACE("key W down!");
        //}
        //if (engine::Input::IsKeyPressed(engine::key::W))
        //{
        //    LOG_TRACE("key W pressed!");
        //}
        //if (engine::Input::IsKeyReleased(engine::key::W))
        //{
        //    LOG_TRACE("key W released!");
        //}

        //if (engine::Input::IsMouseButtonDown(engine::mouse::ButtonLeft))
        //{
        //    LOG_TRACE("mouse button Left is Down!");
        //}
        //if (engine::Input::IsMouseButtonPressed(engine::mouse::ButtonLeft))
        //{
        //    LOG_TRACE("mouse button Left is Pressed!");
        //}
        //if (engine::Input::IsMouseButtonReleased(engine::mouse::ButtonLeft))
        //{
        //    LOG_TRACE("mouse button Left is Released!");
        //}

        //if (engine::Input::IsMouseButtonDown(engine::mouse::ButtonRight))
        //{
        //    LOG_TRACE("mouse button Right is Down!");
        //}
        //if (engine::Input::IsMouseButtonPressed(engine::mouse::ButtonRight))
        //{
        //    LOG_TRACE("mouse button Right is Pressed!");
        //}
        //if (engine::Input::IsMouseButtonReleased(engine::mouse::ButtonRight))
        //{
        //    LOG_TRACE("mouse button Right is Released!");
        //}


        //if (engine::Input::IsMouseButtonDown(engine::mouse::ButtonMiddle))
        //{
        //    LOG_TRACE("mouse button Middle is Down!");
        //}
        //if (engine::Input::IsMouseButtonPressed(engine::mouse::ButtonMiddle))
        //{
        //    LOG_TRACE("mouse button Middle is Pressed!");
        //}
        //if (engine::Input::IsMouseButtonReleased(engine::mouse::ButtonMiddle))
        //{
        //    LOG_TRACE("mouse button Middle is Released!");
        //}

        /*if (engine::Input::IsMouseButtonDown(engine::mouse::ButtonLast))
        {
            LOG_TRACE("mouse button Last is Down!");
        }
        if (engine::Input::IsMouseButtonPressed(engine::mouse::ButtonLast))
        {
            LOG_TRACE("mouse button Last is Pressed!");
        }
        if (engine::Input::IsMouseButtonReleased(engine::mouse::ButtonLast))
        {
            LOG_TRACE("mouse button Last is Released!");
        }*/

        //LOG_TRACE("{0}, {1}", engine::Input::GetMousePosition().first, engine::Input::GetMousePosition().second);
        //LOG_TRACE("{0}, {1}", engine::Input::GetMouseX(), engine::Input::GetMouseY());
    }

    void OnEvent(engine::Event& e) override
    {
        //LOG_TRACE("{0}", e);
        /*if (e.GetEventType() == engine::EVENT_TYPE::MOUSEMOVED)
        {
            LOG_TRACE("{0}", e);
        }*/
    }
};

class Sandbox : public engine::Application
{
public:
    Sandbox(engine::CommandLineArgs args)
        : Application{ "Sandbox" , args }
    {
        LOG_TRACE("Trace Log!");
        LOG_INFO("Info Log!");
        LOG_WARN("Warning Log!");
        LOG_ERROR("Error Log!");
        LOG_CRITICAL("Critical Log!");
        
        //debug layer
        PushLayer(new ExampleLayer());
        PushOverlay(new EditorLayer());
        PushOverlay(new SceneCamera());
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
