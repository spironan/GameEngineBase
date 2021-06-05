#include "pch.h"

#include "Engine/Core/Base.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <GL/gl3w.h>
#include <sdl2/SDL.h>

#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

namespace engine
{
    static SDL_GLContext s_glContext;

    OpenGLContext::OpenGLContext(SDL_Window* window)
        : m_windowHandle(window)
    {
        ENGINE_ASSERT_MSG(m_windowHandle, "Window handle is null!");
    }
    
    OpenGLContext::~OpenGLContext()
    {
        SDL_GL_DeleteContext(s_glContext);
    }

    void OpenGLContext::Init()
    {
        //OpenGL + SDL code
        s_glContext = SDL_GL_CreateContext(m_windowHandle);
        SDL_GL_MakeCurrent(m_windowHandle, s_glContext);
        
        bool status = (gl3wInit() == 0);
        ENGINE_ASSERT_MSG(status, "Failed to initialize OpenGL loader!(gl3w)\n");

        LOG_ENGINE_INFO("OpenGL Info:");
        LOG_ENGINE_INFO("  Vendor   : {0}", glGetString(GL_VENDOR));
        LOG_ENGINE_INFO("  Renderer : {0}", glGetString(GL_RENDERER));
        LOG_ENGINE_INFO("  Version  : {0}", glGetString(GL_VERSION));
        
        //set clear color
        glClearColor(0.2f, 0.3f, 0.3f, 1);
    }

    void OpenGLContext::OnUpdateBegin()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGLContext::SwapBuffers()
    {
        SDL_GL_SwapWindow(m_windowHandle);
    }

    void OpenGLContext::InitImGui()
    {
        ImGui_ImplSDL2_InitForOpenGL(m_windowHandle, s_glContext);
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    void OpenGLContext::OnImGuiBegin()
    {
        ImGui_ImplOpenGL3_NewFrame();
    }

    void OpenGLContext::OnImGuiEnd()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }
    }

    void OpenGLContext::OnImGuiShutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
    }

    bool OpenGLContext::SetVSync(bool enable)
    {
        ENGINE_ASSERT_MSG(SDL_GL_SetSwapInterval(enable) == 0, "setting the swap interval {0} is not supported", enable);
        return enable;
    }

}