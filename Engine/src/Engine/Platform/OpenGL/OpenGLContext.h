/************************************************************************************//*!
\file           OpenGLContext.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Jun 17, 2021
\brief          Describes the OpenGL Implementation of the Graphical Context 
                and Capabilities
                
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
    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(SDL_Window* window);
        virtual ~OpenGLContext();

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