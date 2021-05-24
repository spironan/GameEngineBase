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

#include <SDL.h>

class ExampleLayer : public engine::Layer
{
public :
    ExampleLayer(): Layer("Example")
    {
    }

    void OnUpdate(engine::Timestep dt) override
    {
        //LOG_INFO("ExampleLayer::Update {0}s {1}ms", dt.GetSeconds(), dt.GetMilliSeconds());

        if (engine::Input::IsKeyDown(engine::Key::W))
        {
            LOG_TRACE("key W down!");
        }
        if (engine::Input::IsKeyPressed(engine::Key::W))
        {
            LOG_TRACE("key W pressed!");
        }
        if (engine::Input::IsKeyReleased(engine::Key::W))
        {
            LOG_TRACE("key W released!");
        }

        if (engine::Input::IsMouseButtonDown(engine::Mouse::ButtonLeft))
        {
            LOG_TRACE("Mouse button Left is Down!");
        }
        if (engine::Input::IsMouseButtonPressed(engine::Mouse::ButtonLeft))
        {
            LOG_TRACE("Mouse button Left is Pressed!");
        }
        if (engine::Input::IsMouseButtonReleased(engine::Mouse::ButtonLeft))
        {
            LOG_TRACE("Mouse button Left is Released!");
        }

        if (engine::Input::IsMouseButtonDown(engine::Mouse::ButtonRight))
        {
            LOG_TRACE("Mouse button Right is Down!");
        }
        if (engine::Input::IsMouseButtonPressed(engine::Mouse::ButtonRight))
        {
            LOG_TRACE("Mouse button Right is Pressed!");
        }
        if (engine::Input::IsMouseButtonReleased(engine::Mouse::ButtonRight))
        {
            LOG_TRACE("Mouse button Right is Released!");
        }


        if (engine::Input::IsMouseButtonDown(engine::Mouse::ButtonMiddle))
        {
            LOG_TRACE("Mouse button Middle is Down!");
        }
        if (engine::Input::IsMouseButtonPressed(engine::Mouse::ButtonMiddle))
        {
            LOG_TRACE("Mouse button Middle is Pressed!");
        }
        if (engine::Input::IsMouseButtonReleased(engine::Mouse::ButtonMiddle))
        {
            LOG_TRACE("Mouse button Middle is Released!");
        }

        if (engine::Input::IsMouseButtonDown(engine::Mouse::ButtonLast))
        {
            LOG_TRACE("Mouse button Last is Down!");
        }
        if (engine::Input::IsMouseButtonPressed(engine::Mouse::ButtonLast))
        {
            LOG_TRACE("Mouse button Last is Pressed!");
        }
        if (engine::Input::IsMouseButtonReleased(engine::Mouse::ButtonLast))
        {
            LOG_TRACE("Mouse button Last is Released!");
        }

        //LOG_TRACE("{0}, {1}", engine::Input::GetMousePosition().first, engine::Input::GetMousePosition().second);
        //LOG_TRACE("{0}, {1}", engine::Input::GetMouseX(), engine::Input::GetMouseY());
    }

    void OnEvent(engine::Event& e) override
    {
        //LOG_TRACE("{0}", e);
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
        
        PushLayer(new ExampleLayer());
    };

    virtual ~Sandbox() = default;
};

engine::Application* engine::CreateApplication(engine::CommandLineArgs args)
{
    return new Sandbox{ args };
}