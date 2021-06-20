/************************************************************************************//*!
\file           VulkanContext.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Jun 17, 2021
\brief          Describes the Vulkan Implementation of the Graphically Context
                and capabilities.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
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

        void SetWindowResized() override;
        bool SetVSync(bool enable) override;
    private:
        SDL_Window* m_windowHandle;
    };
}