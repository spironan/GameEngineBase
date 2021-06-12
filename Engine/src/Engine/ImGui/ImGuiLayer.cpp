/************************************************************************************//*!
\file           ImGuiLayer.cpp
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
#include "pch.h"

#include "Engine/ImGui/ImGuiLayer.h"
#include "Engine/Renderer/GraphicsContext.h"
#include "Engine/Core/Application.h"

#include <sdl2/SDL.h>

#include <imgui.h>
#include <imgui_impl_sdl.h>

namespace engine
{
    ImGuiLayer::ImGuiLayer()
        : m_blockEvents { true }
        , Layer{ "ImGuiLayer" }
#ifdef ENGINE_PLATFORM_WINDOWS
        , m_window { static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow())  }
#endif
        , m_renderer{ static_cast<GraphicsContext*>(Application::Get().GetWindow().GetRenderingContext()) }
    {
    }

    void ImGuiLayer::OnAttach()
    {
        ENGINE_PROFILE_FUNCTION();

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

        //io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", 18.0f);
        //io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", 18.0f);

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        
        //SetDarkThemeColors();

        // Setup Platform/Renderer bindings
        m_renderer->InitImGui();
    }

    void ImGuiLayer::OnDetach()
    {
        ENGINE_PROFILE_FUNCTION();

        m_renderer->OnImGuiShutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    //void ImGuiLayer::OnEvent(Event& e)
    //{
    //    if (m_blockEvents)
    //    {
    //        ImGuiIO& io = ImGui::GetIO();
    //        e.Handled |= e.IsInCategory(EVENT_CATEGORY::MOUSE) & io.WantCaptureMouse;
    //        e.Handled |= e.IsInCategory(EVENT_CATEGORY::KEYBOARD) & io.WantCaptureKeyboard;

    //        //int key = event->key.keysym.scancode;
    //        switch (e.GetEventType())
    //        {
    //            case engine::EVENT_TYPE::KEYPRESSED:
    //            case engine::EVENT_TYPE::KEYTYPED:
    //            {
    //                int key = static_cast<engine::KeyPressedEvent&>(e).GetKeyCode();
    //                io.KeysDown[key] = 1;
    //                //AddInputCharacter(key);
    //                /*if (key != 0)
    //                    io.InputQueueCharacters.push_back(key <= IM_UNICODE_CODEPOINT_MAX ? (ImWchar)key : IM_UNICODE_CODEPOINT_INVALID);*/
    //                //io.InputQueueCharacters[key] = 1;
    //            }
    //            break;
    //            case engine::EVENT_TYPE::KEYRELEASED:
    //            {
    //                int key = static_cast<engine::KeyPressedEvent&>(e).GetKeyCode();
    //                io.KeysDown[key] = 0;
    //            }
    //            break;
    //            case engine::EVENT_TYPE::MOUSESCROLLED:
    //            {
    //                io.MouseWheel = static_cast<engine::MouseScrolledEvent&>(e).GetY();
    //            }
    //            break;

    //        }

    //        //if ( == )
    //        //{

    //        //    //IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
    //        //    /*io.KeysDown[key] = (event->type == SDL_KEYDOWN);
    //        //    io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
    //        //    io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
    //        //    io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);*/
    //        //}

    //        /*if (e.GetEventType() == engine::EVENT_TYPE::MOUSESCROLLED)
    //        {
    //            io.MouseWheel = static_cast<engine::MouseScrolledEvent&>(e).GetY();
    //        }*/
    //    }
    //}

    void ImGuiLayer::Begin()
    {
        ENGINE_PROFILE_FUNCTION();

        m_renderer->OnImGuiBegin();

#ifdef ENGINE_PLATFORM_WINDOWS
        ImGui_ImplSDL2_NewFrame(m_window);
#endif

        ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        ENGINE_PROFILE_FUNCTION();

        ////figure out what the below 3 lines do?
        /*ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());*/

        // Rendering
        ImGui::Render();
        
        m_renderer->OnImGuiEnd();
    }

}