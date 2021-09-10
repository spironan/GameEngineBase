/************************************************************************************//*!
\file           Application.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 05, 2021
\brief          Core Application Loop and functionality. 
                Will be inherited by Sandbox project.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Core/Window.h"

#include "Engine/Core/Events/ApplicationEvent.h"
#include "Engine/Core/Events/KeyEvent.h"
#include "Engine/Core/Events/MouseEvent.h"

#include "Engine/Core/Timestep.h"

#include "Engine/Core/LayerStack.h"

#include "Engine/ImGui/ImGuiLayer.h"

#include <deque>
#include <queue>

//forward declare main
int main(int argc, char** argv);

namespace engine
{
    /********************************************************************************//*!
     @brief     Defines what makes up a command line argument.
    *//*********************************************************************************/
    struct CommandLineArgs
    {
        int Count = 0;
        char** Args = nullptr;

        /****************************************************************************//*!
         @brief     subscript operator overload for convinience when wanting to 
                    access arguments directly
         
         @note      make sure you do not use an invalid index that is larger then the
                    possible number of arguments.

         @param     the index you want to retrieve the command line argument from.
        *//*****************************************************************************/
        const char* operator[](int index) const
        {
            ENGINE_ASSERT(index < Count);
            return Args[index];
        }
    };

    /********************************************************************************//*!
     @brief     Application entails the core loop and the where and how each system is 
                being used within.
    *//*********************************************************************************/
    class Application
    {
    public:
        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        Application(const std::string& name = "Engine App", CommandLineArgs args = CommandLineArgs{});
        virtual ~Application();

        /*-----------------------------------------------------------------------------*/
        /* Getters                                                                     */
        /*-----------------------------------------------------------------------------*/
        /****************************************************************************//*!
         @brief     Retrieve the static instance of the create application.
         
         @note      Function will break if an Application is not yet created and 
                    this function gets called.

         @return    returns the static instance of the created application.
                    crashes if s_instance is null due to dereferencing.
        *//*****************************************************************************/
        static Application& Get() { return*s_instance; }
        /****************************************************************************//*!
         @brief     Retrieve window that the application holds.

         @note      continue to call GetNativeWindow() to retrieve the actual window.

         @return    returns a generic window reference
        *//*****************************************************************************/
        Window& GetWindow() const { return *m_window; }
        /****************************************************************************//*!
         @brief     Retrieve the command line arguments passed to the application.

         @return    returns the command line arguments struct held by the application.
        *//*****************************************************************************/
        CommandLineArgs GetCommandLineArgs() const { return m_commandLineArgs; }

        /*-----------------------------------------------------------------------------*/
        /* Functions                                                                   */
        /*-----------------------------------------------------------------------------*/
        /****************************************************************************//*!
         @brief     Describes the applications way of closing down
        *//*****************************************************************************/
        void Close();
        /****************************************************************************//*!
         @brief     Describes what happens when any type of event gets called
        *//*****************************************************************************/
        void OnEvent(Event& e);
        /****************************************************************************//*!
         @brief     Adds a layer to the core layerstack and calls the layers OnAttach

         @param     layer
                    The layer that will be added to the topmost layer of the layerstack.
                    topmost layer is always behind overlay so all overlays will always
                    render ontop of any normal layer.
        *//*****************************************************************************/
        void PushLayer(LayerStack::value_type layer);
        /****************************************************************************//*!
         @brief     Adds a layer to the core layerstack as an OVERLAY and 
                    calls the layers OnAttach.
                    A layer added as an overlay will render at the topmost layer.
                    Everytime a new overlay is added it will become the topmost layer.
                    
         @param     overlay
                    The overlay that will be added to the topmost overlay of the layerstack.
        *//*****************************************************************************/
        void PushOverlay(LayerStack::value_type overlay);

        void PopLayer(LayerStack::value_type layer);
        void PopOverlay(LayerStack::value_type overlay);

    private:
        /****************************************************************************//*!
         @brief     Describes the applications core run loop
        *//*****************************************************************************/
        void Run();

        void ProcessLayers();

        bool OnWindowClose(WindowCloseEvent& e);

    private:
        bool m_running;
        CommandLineArgs m_commandLineArgs;
        Window* m_window;
        GraphicsContext* m_renderer;
        std::shared_ptr<ImGuiLayer> m_imGuiLayer;
        LayerStack m_layerStack;
        
        std::queue<LayerStack::value_type> m_addLayerQueue;
        std::queue<LayerStack::value_type> m_addOverlayQueue;
        std::queue<LayerStack::value_type> m_removeLayerQueue;
        std::queue<LayerStack::value_type> m_removeOverlayQueue;

        static Application* s_instance;
        friend int ::main(int argc, char** argv);
    };

    /****************************************************************************//*!
     @brief     Function prototype, implementation to be defined in client
    *//*****************************************************************************/
    Application* CreateApplication(CommandLineArgs commandLineArgs);
}
