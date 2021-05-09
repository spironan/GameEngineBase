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

#include "Engine.h" // single-include file to get all the relevant functions from our engine.

class Sandbox : public Engine::Application
{
public:
    Sandbox() = default;
    virtual ~Sandbox() = default;
};

Engine::Application* Engine::CreateApplication()
{
    return new Sandbox();
}