#include "pch.h"
#include "Window.h"

#include "Engine/Core/Base.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Engine
{
    Window* Window::Create(const WindowProperties& properties)
    {
    #ifdef ENGINE_PLATFORM_WINDOWS
        return new Windowswindow{ properties };
    #else
        ENGINE_ASSERT_MSG(false, "Unsupported platform!");
        return nullptr;
    #endif
    }
}