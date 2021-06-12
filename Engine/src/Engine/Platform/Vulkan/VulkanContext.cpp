#include "pch.h"

#include "Engine/Core/Base.h"

#include "Engine/Platform/Vulkan/VulkanContext.h"

#include "vk_engine.h"

namespace engine
{
    static VulkanEngine vkEngine;

    VulkanContext::VulkanContext(SDL_Window* window)
        : m_windowHandle(window)
    {
        ENGINE_ASSERT_MSG(m_windowHandle, "Window handle is null!");

        vkEngine.SetWindow(window);
    }
    
    VulkanContext::~VulkanContext()
    {
        // do nothing for now
        vkEngine.cleanup();
    }

    void VulkanContext::Init()
    {
        vkEngine.init();
        
        //OpenGL + SDL code
        //m_glContext = SDL_GL_CreateContext(m_windowHandle);
        //SDL_GL_MakeCurrent(m_windowHandle, m_glContext);
        
        //bool status = (gl3wInit() == 0);
        //ENGINE_ASSERT_MSG(status, "Failed to initialize OpenGL loader!(gl3w)\n");
        
        //LOG_ENGINE_INFO("OpenGL Info:");
        //LOG_ENGINE_INFO("  Vendor   : {0}", glGetString(GL_VENDOR));
        //LOG_ENGINE_INFO("  Renderer : {0}", glGetString(GL_RENDERER));
        //LOG_ENGINE_INFO("  Version  : {0}", glGetString(GL_VERSION));
    }

    void VulkanContext::OnUpdateBegin()
    {
        //empty for now
    }

    void VulkanContext::SwapBuffers()
    {
        vkEngine.RenderFrame();
    }

    void VulkanContext::InitImGui()
    {
        ImGui_ImplSDL2_InitForVulkan(m_windowHandle);
        vkEngine.init_imgui();
    }

    void VulkanContext::OnImGuiBegin()
    {
        ImGui_ImplVulkan_NewFrame();
    }

    void VulkanContext::OnImGuiEnd()
    {
        // Vulkan will call internally

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void VulkanContext::OnImGuiShutdown()
    {
        vkDeviceWaitIdle(vkEngine._device);
        ImGui_ImplVulkan_Shutdown();
    }

    bool VulkanContext::SetVSync(bool enable)
    {
        // vulkan does not currently support vsync yet
        return false;
    }

}