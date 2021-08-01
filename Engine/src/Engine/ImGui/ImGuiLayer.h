/************************************************************************************//*!
\file           ImGuiLayer.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 25, 2021
\brief          Implements a basic ImGuiLayer that sets up for the core ImGui Features
                Required into the main application.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/Core/Layer.h"
#include "Engine/Core/Events/Event.h"

//forward declaration
struct SDL_Window;

namespace engine
{
    //forward declaration
    class GraphicsContext;

    class ImGuiLayer final : public Layer
    {
    public:
        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        ImGuiLayer();
        ~ImGuiLayer() = default;

        /*-----------------------------------------------------------------------------*/
        /* Functions                                                                   */
        /*-----------------------------------------------------------------------------*/
        void OnAttach() override;
        void OnDetach() override;
        
        //void OnRestart();

        void Begin();
        void End();

        void BlockEvents(bool block) { m_blockEvents = block; }

    private:
        bool m_blockEvents;
        SDL_Window* m_window;
        GraphicsContext* m_renderer;

        //bool m_restart = false;
    };
}