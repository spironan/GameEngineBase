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

#include <imgui.h>
#include "Engine/Core/Layer.h"
#include "Engine/Events/Event.h"
#include <imgui_impl_sdl.h>

namespace engine
{
    class ImGuiLayer : public Layer
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
        void OnEvent(Event& e) override;

        void Begin();
        void End();

        void BlockEvents(bool block) { m_blockEvents = block; }
    private:
		SDL_Window* window;
		ImGuiIO io;
        bool m_blockEvents;
    };
}