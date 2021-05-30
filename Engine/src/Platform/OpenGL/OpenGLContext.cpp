#include "pch.h"

#include "Engine/Core/Base.h"

#include "Platform/OpenGL/OpenGLContext.h"

//#include "../../Sandbox/vendor/gl3w/GL/glcorearb.h"
//#include "../../Sandbox/vendor/gl3w/GL/gl3w.h"

namespace engine
{
    OpenGLContext::OpenGLContext(SDL_Window* window)
        : m_windowHandle(window)
    {
        ENGINE_ASSERT_MSG(m_windowHandle, "Window handle is null!");
    }

    void OpenGLContext::Init()
    {
        bool err = gl3wInit() != 0;
        if (err)
        {
            fprintf(stderr, "Failed to initialize OpenGL loader!\n");
            //return 1;
        }
    }

    void OpenGLContext::SwapBuffers()
    {

    }
}