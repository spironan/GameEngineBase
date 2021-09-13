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
#include <Engine.h>
// specially include this file only at the entry point of the engine.
#include <EntryPoint.h>
// custom math library.
#include <oom/oom.hpp>

//Editor Layer
#include "CoreLayers/EditorLayer.h"
//Scripting Layer
#include "CoreLayers/ScriptingLayer.h"

// for debugging
#include "TestLayers/InputDebugLayer.h"
#include "DebugLayer.h" 
#include "AssetManagementLayer.h" 

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

class Sandbox : public engine::Application
{
public:
    Sandbox(engine::CommandLineArgs args)
        : Application{ "Sandbox" , args }
    {
        // Client Side Logging
        LOG_TRACE("Trace Log!");
        LOG_INFO("Info Log!");
        LOG_WARN("Warning Log!");
        LOG_ERROR("Error Log!");
        LOG_CRITICAL("Critical Log!");

        PushLayer(std::make_shared<AssetManagementLayer>());
        
        ////Default Samples Layers (inherit from engine::Layer)
        PushLayer(std::make_shared<ExampleLayer>());      // contains ability to quit progrom with esc
        PushOverlay(std::make_shared<ExampleLayer>());
        //// For Input Debugging
        //PushOverlay(std::make_shared<InputDebugLayer>());

        // Manually turn on either one for now.
        
        // Debug Layers! Dont mess with the order!
        PushOverlay(std::make_shared<DebugLayer>(*this));

        // Official Original Layer
        // Editor Layer
        PushOverlay(std::make_shared<EditorLayer>(*this));
        // Scripting Layer
        PushOverlay(std::make_shared<ScriptingLayer>());
        // Set Default EDITOR Camera
        //editorController->SetEditorCamera();


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
