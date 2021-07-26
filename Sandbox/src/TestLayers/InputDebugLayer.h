/************************************************************************************//*!
\file           InputDebugLayer.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Jul 22, 2021
\brief          Describes a Test scene used to test The Input Systems
                Functionality and print out debug messages for all supported inputs.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include <Engine.h>

/****************************************************************************//*!
 @brief     Describes a Test scene used to test The Input Systems
            Functionality and print out debug messages for all supported inputs.
*//*****************************************************************************/
class InputDebugLayer : public engine::Layer
{
public:
    InputDebugLayer() : Layer("InputDebugLayer")
    {
    }

    void OnUpdate(engine::Timestep dt) override
    {
        //LOG_INFO("ExampleLayer::Update {0}s {1}ms", dt.GetSeconds(), dt.GetMilliSeconds());
        // Commenting this out for now until engine::Input::GetMouseDelta() no longer consumes the information
        /*engine::Input::GetMouseDelta();
        std::pair<int, int> pos = engine::Input::GetMouseDelta();
        LOG_INFO("{0}, {1}", pos.first , pos.second);*/


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
            LOG_TRACE("mouse button Left is Down!");
        }
        if (engine::Input::IsMouseButtonPressed(engine::Mouse::ButtonLeft))
        {
            LOG_TRACE("mouse button Left is Pressed!");
        }
        if (engine::Input::IsMouseButtonReleased(engine::Mouse::ButtonLeft))
        {
            LOG_TRACE("mouse button Left is Released!");
        }

        if (engine::Input::IsMouseButtonDown(engine::Mouse::ButtonRight))
        {
            LOG_TRACE("mouse button Right is Down!");
        }
        if (engine::Input::IsMouseButtonPressed(engine::Mouse::ButtonRight))
        {
            LOG_TRACE("mouse button Right is Pressed!");
        }
        if (engine::Input::IsMouseButtonReleased(engine::Mouse::ButtonRight))
        {
            LOG_TRACE("mouse button Right is Released!");
        }


        if (engine::Input::IsMouseButtonDown(engine::Mouse::ButtonMiddle))
        {
            LOG_TRACE("mouse button Middle is Down!");
        }
        if (engine::Input::IsMouseButtonPressed(engine::Mouse::ButtonMiddle))
        {
            LOG_TRACE("mouse button Middle is Pressed!");
        }
        if (engine::Input::IsMouseButtonReleased(engine::Mouse::ButtonMiddle))
        {
            LOG_TRACE("mouse button Middle is Released!");
        }

        if (engine::Input::IsMouseButtonDown(engine::Mouse::ButtonLast))
        {
            LOG_TRACE("mouse button Last is Down!");
        }
        if (engine::Input::IsMouseButtonPressed(engine::Mouse::ButtonLast))
        {
            LOG_TRACE("mouse button Last is Pressed!");
        }
        if (engine::Input::IsMouseButtonReleased(engine::Mouse::ButtonLast))
        {
            LOG_TRACE("mouse button Last is Released!");
        }

        LOG_TRACE("{0}, {1}", engine::Input::GetMousePosition().first, engine::Input::GetMousePosition().second);
        LOG_TRACE("{0}, {1}", engine::Input::GetMouseX(), engine::Input::GetMouseY());
    }

    void OnEvent(engine::Event& e) override
    {
        LOG_TRACE("{0}", e);
        if (e.GetEventType() == engine::EVENT_TYPE::MOUSEMOVED)
        {
            LOG_TRACE("{0}", e);
        }
    }
};