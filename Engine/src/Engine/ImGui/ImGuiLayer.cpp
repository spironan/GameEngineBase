#include "pch.h"

#include "Engine/ImGui/ImGuiLayer.h"

#include "Engine/Core/Application.h"
#include <sdl2/SDL.h>

//#include <imgui.h>

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

        //// Setup Dear ImGui context
        //IMGUI_CHECKVERSION();
        //ImGui::CreateContext();
        //ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        ////io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        ////io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        ////io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

        //io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", 18.0f);
        //io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", 18.0f);

        //// Setup Dear ImGui style
        //ImGui::StyleColorsDark();
        ////ImGui::StyleColorsClassic();

        //// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        //ImGuiStyle& style = ImGui::GetStyle();
        //if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        //{
        //    style.WindowRounding = 0.0f;
        //    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        //}

        //SetDarkThemeColors();

#ifdef ENGINE_PLATFORM_WINDOWS
        SDL_Window* window = static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow());
#endif

        /*Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());*/

        //// Setup Platform/Renderer bindings
        //ImGui_ImplGlfw_InitForOpenGL(window, true);
        //ImGui_ImplOpenGL3_Init("#version 410");

    }

    void ImGuiLayer::OnDetach()
    {
        ENGINE_PROFILE_FUNCTION();

        /*ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();*/
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

        /*ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();*/
    }

    void ImGuiLayer::End()
    {
        ENGINE_PROFILE_FUNCTION();

        //ImGuiIO& io = ImGui::GetIO();
        //Application& app = Application::Get();
        //io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        //// Rendering
        //ImGui::Render();
        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        //{
        //    GLFWwindow* backup_current_context = glfwGetCurrentContext();
        //    ImGui::UpdatePlatformWindows();
        //    ImGui::RenderPlatformWindowsDefault();
        //    glfwMakeContextCurrent(backup_current_context);
        //}
    }

}