#pragma once

#include "Engine/Renderer/GraphicsContext.h"

struct SDL_Window;

namespace engine
{
    class VulkanContext : public GraphicsContext
    {
    public:
        VulkanContext(SDL_Window* window);
        virtual ~VulkanContext();

        void Init() override;
        void SwapBuffers() override;
        void InitImGui() override;

    private:
        SDL_Window* m_windowHandle;
    };
}