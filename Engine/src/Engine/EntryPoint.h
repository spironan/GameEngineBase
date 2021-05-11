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

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
    Engine::Log::Init();
    
    bool pass = true;
    ENGINE_ASSERT(true);
    //ENGINE_ASSERT(false);
    ENGINE_ASSERT_MSG(pass,"failed test case");
    ENGINE_ASSERT_MSG(!pass, "failed test case");

    ENGINE_VERIFY(pass);
    //ENGINE_VERIFY(!pass);
    ENGINE_VERIFY_MSG(pass, "failed test case");
    //ENGINE_VERIFY_MSG(!pass, "failed test case");

    LOG_ENGINE_TRACE("Trace Log!");
    LOG_ENGINE_INFO("Info Log!");
    LOG_ENGINE_WARN("Warning Log!");
    LOG_ENGINE_ERROR("Error Log!");
    LOG_ENGINE_CRITICAL("Critical Log!");

    auto app = Engine::CreateApplication();
    app->Run();
    delete app;
}