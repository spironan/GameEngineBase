/************************************************************************************//*!
\file           Window.cpp
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 24, 2021
\brief          Describes a general abstracted window and its interface.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"
#include "Window.h"

#include "Engine/Core/Base.h"
#include "Engine/Platform/Windows/WindowsWindow.h"

namespace engine
{
    Window* Window::Create(const WindowProperties& properties)
    {
    #ifdef ENGINE_PLATFORM_WINDOWS
        //return new WindowsWindow{ properties };
        return MemoryManager::NewOnStack<WindowsWindow>(properties);
#else
        ENGINE_ASSERT_MSG(false, "Unsupported platform!");
        return nullptr;
    #endif
    }
}