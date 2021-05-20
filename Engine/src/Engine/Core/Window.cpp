#include "pch.h"
#include "Window.h"

#include "Engine/Core/Base.h"
#include "Platform/Windows/WindowsWindow.h"

namespace engine
{
    Window* Window::Create(const WindowProperties& properties)
    {
    #ifdef ENGINE_PLATFORM_WINDOWS
        return new WindowsWindow{ properties };
    #else
        ENGINE_ASSERT_MSG(false, "Unsupported platform!");
        return nullptr;
    #endif
    }
}