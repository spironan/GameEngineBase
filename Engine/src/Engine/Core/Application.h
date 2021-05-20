/************************************************************************************//*!
\file           Application.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 05, 2021
\brief          Core Application Loop and functionality. 
                Will be inherited by Sandbox project.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Core/Window.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"

#include "Engine/Core/Timestep.h"

#include "Engine/Core/LayerStack.h"

namespace engine
{
    struct CommandLineArgs
    {
        int Count = 0;
        char** Args = nullptr;

        const char* operator[](int index) const
        {
            ENGINE_ASSERT(index < Count);
            return Args[index];
        }
    };

    class Application
    {
    public:
        Application(const std::string& name = "Engine App", CommandLineArgs args = CommandLineArgs{});
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        void Close();

        CommandLineArgs GetCommandLineArgs() const { return m_commandLineArgs; }

        static Application& Get() { return*s_instance; }

    private:
        bool OnWindowClose(WindowCloseEvent& e);

    private:
        CommandLineArgs m_commandLineArgs;
        bool m_running;
        Window* m_window;

        LayerStack m_layerStack;

        double m_lastFrameTime;
        
        static Application* s_instance;
    };

    // To be defined in CLIENT
    Application* CreateApplication(CommandLineArgs commandLineArgs);
}

