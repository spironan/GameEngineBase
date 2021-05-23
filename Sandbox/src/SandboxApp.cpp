/************************************************************************************//*!
\file           SandboxApp.cpp
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 05, 2021
\brief          Customer side of the project that utilizes the functions of the Engine.
                An Editor will be a use case for game engine.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/

#include "Engine.h" // single-include file to get all the relevant functions from our engine.

#include <iostream>

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_sdl.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>

#include "Editor/Editor.h"
#include <GL/gl3w.h>
#define NOMINMAX


class EditorLayer : public engine::Layer
{
private:
	Editor m_editorObject;
	SDL_Window* window;
	ImGuiIO io; 
	SDL_GLContext gl_context;
public:
	EditorLayer() : Layer("Editor"), m_editorObject{}
	{

	}
	void OnAttach() override
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
		{
			printf("Error: %s\n", SDL_GetError());
			exit(-1);
		}

		// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		// Create window with graphics context
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
		window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
		gl_context = SDL_GL_CreateContext(window);
		SDL_GL_MakeCurrent(window, gl_context);
		SDL_GL_SetSwapInterval(1); // Enable vsync

		bool err = gl3wInit() != 0;

		if (err)
		{
			fprintf(stderr, "Failed to initialize OpenGL loader!\n");
			exit(1);
		}

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

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

		// Setup Platform/Renderer backends
		ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
		ImGui_ImplOpenGL3_Init(glsl_version);




	}
	void OnUpdate(engine::Timestep dt) override
	{
		// Our state
		// Main loop
		static bool show_demo_window = true;
		static bool show_another_window = false;
		static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		static bool done = false;
		while (!done)
		{
			// Poll and handle events (inputs, window resize, etc.)
			// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
			// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
			// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
			// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				ImGui_ImplSDL2_ProcessEvent(&event);
				if (event.type == SDL_QUIT)
					done = true;
				if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
					done = true;
			}

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame(window);
			ImGui::NewFrame();

			// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
			//if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
			m_editorObject.TestFunction();


			// Rendering
			ImGui::Render();
			glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
			glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Update and Render additional Platform Windows
			// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
			//  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
				SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
			}

			SDL_GL_SwapWindow(window);
		}
	}
	void OnEvent(engine::Event& e) override
	{
		//LOG_TRACE("{0}", e);
	}
	void OnDetach() override
	{
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		SDL_GL_DeleteContext(gl_context);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
};



class ExampleLayer : public engine::Layer
{
public :
    ExampleLayer(): Layer("Example")
    {
    }

    void OnUpdate(engine::Timestep dt) override
    {
        //LOG_INFO("ExampleLayer::Update {0}s {1}ms", dt.GetSeconds(), dt.GetMilliSeconds());

        if (engine::Input::IsKeyDown(engine::Key::W))
        {
            LOG_TRACE("key W down!");
        }
        if (engine::Input::IsKeyPressed(engine::Key::W))
        {
            LOG_TRACE("key W pressed!");
        }
        if (engine::Input::IsKeyReleased(engine::Key::W))
        {
            LOG_TRACE("key W released!");
        }

        if (engine::Input::IsMouseButtonDown(engine::Mouse::ButtonLeft))
        {
            LOG_TRACE("Mouse button Left is Down!");
        }
        if (engine::Input::IsMouseButtonPressed(engine::Mouse::ButtonLeft))
        {
            LOG_TRACE("Mouse button Left is Pressed!");
        }
        if (engine::Input::IsMouseButtonReleased(engine::Mouse::ButtonLeft))
        {
            LOG_TRACE("Mouse button Left is Released!");
        }

        if (engine::Input::IsMouseButtonDown(engine::Mouse::ButtonRight))
        {
            LOG_TRACE("Mouse button Right is Down!");
        }
        if (engine::Input::IsMouseButtonPressed(engine::Mouse::ButtonRight))
        {
            LOG_TRACE("Mouse button Right is Pressed!");
        }
        if (engine::Input::IsMouseButtonReleased(engine::Mouse::ButtonRight))
        {
            LOG_TRACE("Mouse button Right is Released!");
        }


        if (engine::Input::IsMouseButtonDown(engine::Mouse::ButtonMiddle))
        {
            LOG_TRACE("Mouse button Middle is Down!");
        }
        if (engine::Input::IsMouseButtonPressed(engine::Mouse::ButtonMiddle))
        {
            LOG_TRACE("Mouse button Middle is Pressed!");
        }
        if (engine::Input::IsMouseButtonReleased(engine::Mouse::ButtonMiddle))
        {
            LOG_TRACE("Mouse button Middle is Released!");
        }

        if (engine::Input::IsMouseButtonDown(engine::Mouse::ButtonLast))
        {
            LOG_TRACE("Mouse button Last is Down!");
        }
        if (engine::Input::IsMouseButtonPressed(engine::Mouse::ButtonLast))
        {
            LOG_TRACE("Mouse button Last is Pressed!");
        }
        if (engine::Input::IsMouseButtonReleased(engine::Mouse::ButtonLast))
        {
            LOG_TRACE("Mouse button Last is Released!");
        }

        //LOG_TRACE("{0}, {1}", engine::Input::GetMousePosition().first, engine::Input::GetMousePosition().second);
        //LOG_TRACE("{0}, {1}", engine::Input::GetMouseX(), engine::Input::GetMouseY());
    }

    void OnEvent(engine::Event& e) override
    {
        //LOG_TRACE("{0}", e);
    }
};

class Sandbox : public engine::Application
{
public:
    Sandbox(engine::CommandLineArgs args)
        : Application{ "Sandbox" , args }
    {
        LOG_TRACE("Trace Log!");
        LOG_INFO("Info Log!");
        LOG_WARN("Warning Log!");
        LOG_ERROR("Error Log!");
        LOG_CRITICAL("Critical Log!");
        
        PushLayer(new EditorLayer());
    };

    virtual ~Sandbox() = default;
};

engine::Application* engine::CreateApplication(engine::CommandLineArgs args)
{
    return new Sandbox{ args };
}