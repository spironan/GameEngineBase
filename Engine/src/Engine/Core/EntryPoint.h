/************************************************************************************//*!
\file           EntryPoint.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 05, 2021
\brief          Main Entry point to the program.
                Will hide away this from the Sandbox and they just have to implement
                a version of the CreateApplication function.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Application.h"
#include "Log.h"

extern Engine::Application* Engine::CreateApplication(Engine::CommandLineArgs args);

int main(int argc, char** argv)
{
    // Initialize logging system
    Engine::Log::Init();

    auto app = Engine::CreateApplication({argc, argv});
    app->Run();
    delete app;
}