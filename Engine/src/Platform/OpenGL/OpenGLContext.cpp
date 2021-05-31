#include "pch.h"

#include "Engine/Core/Base.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <GL/gl3w.h>
#include <sdl2/SDL.h>

namespace engine
{
    static SDL_GLContext m_glContext;

    OpenGLContext::OpenGLContext(SDL_Window* window)
        : m_windowHandle(window)
    {
        ENGINE_ASSERT_MSG(m_windowHandle, "Window handle is null!");
    }
    
    OpenGLContext::~OpenGLContext()
    {
        SDL_GL_DeleteContext(m_glContext);
    }

    void OpenGLContext::Init()
    {
        //OpenGL + SDL code
        m_glContext = SDL_GL_CreateContext(m_windowHandle);
        SDL_GL_MakeCurrent(m_windowHandle, m_glContext);
        
        bool status = (gl3wInit() == 0);
        ENGINE_ASSERT_MSG(status, "Failed to initialize OpenGL loader!(gl3w)\n");

        LOG_ENGINE_INFO("OpenGL Info:");
        LOG_ENGINE_INFO("  Vendor   : {0}", glGetString(GL_VENDOR));
        LOG_ENGINE_INFO("  Renderer : {0}", glGetString(GL_RENDERER));
        LOG_ENGINE_INFO("  Version  : {0}", glGetString(GL_VERSION));
    }

    void OpenGLContext::SwapBuffers()
    {
        SDL_GL_SwapWindow(m_windowHandle);
    }

    void OpenGLContext::InitImGui()
    {
        //ImGui_ImplSDL2_InitForOpenGL(m_window, renderer);
        //ImGui_ImplOpenGL3_Init("#version 450");
    }

}