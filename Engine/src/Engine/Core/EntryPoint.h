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

#include "Engine/Core/Application.h"
#include "Engine/Core/Base.h"

extern engine::Application* engine::CreateApplication(engine::CommandLineArgs args);

int main(int argc, char** argv)
{
    // Memory Leak Checker in Debug builds
#ifdef ENGINE_DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    // Uncomment to cause a break on allocation for debugging
    //_CrtSetBreakAlloc(/*Allocation Number here*/);
#endif

    // Initialize logging system
    engine::Log::Init();

    auto app = engine::CreateApplication({argc, argv});
    app->Run();
    delete app;

}