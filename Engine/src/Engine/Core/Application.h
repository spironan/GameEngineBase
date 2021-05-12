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

#include "pch.h"

namespace Engine
{
    struct CommandLineArgs
    {
        int Count = 0;
        char** Args = nullptr;

        const char* operator[](int index) const
        {
            return Args[index];
        }
    };

    class Application
    {
    public:
        Application(const std::string& name = "Engine App", CommandLineArgs args = CommandLineArgs{});
        virtual ~Application();

        void Run();

        CommandLineArgs GetCommandLineArgs() const { return _commandLineArgs; }

    private:
        CommandLineArgs _commandLineArgs;
        bool _running;
    };

    Application* CreateApplication(CommandLineArgs commandLineArgs);
}

