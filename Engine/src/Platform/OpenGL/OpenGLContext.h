#pragma once

#include "Engine/Renderer/GraphicsContext.h"

struct SDL_Window;

namespace engine
{
    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(SDL_Window* window);
        virtual ~OpenGLContext();

        void Init() override;
        void SwapBuffers() override;

    private:
        SDL_Window* m_windowHandle;
    };
}