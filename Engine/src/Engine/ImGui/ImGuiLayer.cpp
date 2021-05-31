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
#include <imgui_impl_opengl3.h>

namespace engine
{
    ImGuiLayer::ImGuiLayer()
        : m_blockEvents { true }
        , Layer("ImGuiLayer")
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

#ifdef ENGINE_PLATFORM_WINDOWS
        SDL_Window* window = static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow());
        GraphicsContext* renderer = static_cast<GraphicsContext*>(Application::Get().GetWindow().GetNativeRenderer());
#endif

        // Setup Platform/Renderer bindings
        ImGui_ImplSDL2_InitForOpenGL(window, renderer);
        ImGui_ImplOpenGL3_Init("#version 450");

    }

    void ImGuiLayer::OnDetach()
    {
        ENGINE_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnEvent(Event& e)
    {
        if (m_blockEvents)
        {
            /*ImGuiIO& io = ImGui::GetIO();
            e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;*/
        }
    }

    void ImGuiLayer::Begin()
    {
        ENGINE_PROFILE_FUNCTION();

#ifdef ENGINE_PLATFORM_WINDOWS
        SDL_Window* window = static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow());
        SDL_Renderer* renderer = static_cast<SDL_Renderer*>(Application::Get().GetWindow().GetNativeRenderer());
#endif
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        ENGINE_PROFILE_FUNCTION();

        ImGuiIO& io = ImGui::GetIO();

        //figure out what the below 2 lines do?
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }
    }

}