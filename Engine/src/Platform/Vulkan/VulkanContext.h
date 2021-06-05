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
        void OnUpdateBegin() override;
        void SwapBuffers() override;

        void InitImGui() override;
        void OnImGuiBegin() override;
        void OnImGuiEnd() override;
        void OnImGuiShutdown() override;

        bool SetVSync(bool enable) override;
    private:
        SDL_Window* m_windowHandle;
    };
}