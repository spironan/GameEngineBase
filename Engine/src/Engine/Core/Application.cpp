/************************************************************************************//*!
\file           Application.cpp
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
#include "pch.h"
#include "Application.h"

#include "Engine/Core/Input.h"

#include "Engine/Renderer/GraphicsContext.h"
#include "Engine/Renderer/Renderer2D.h"

namespace engine
{
    Application* Application::s_instance = nullptr;

    Application::Application(const std::string& name, CommandLineArgs args)
        : m_commandLineArgs{ args }
        , m_running{ true }
    {
        ENGINE_PROFILE_FUNCTION();

        ENGINE_ASSERT_MSG(!s_instance, "Application already exist!");
        s_instance = this;
        m_window = Window::Create(WindowProperties{ name });

        engine::Renderer2D::Init();

        //Binds window callback to call Application::OnEvent
        m_window->SetEventCallback(ENGINE_BIND_EVENT_FN(Application::OnEvent));
        //Retrieve renderer from window
        m_renderer = static_cast<GraphicsContext*>(m_window->GetRenderingContext());

        //m_imGuiLayer = new ImGuiLayer();
        m_imGuiLayer = MemoryManager::NewOnStack<ImGuiLayer>();
        m_layerStack.PushOverlay(m_imGuiLayer);

        /*Initialize Input Management*/
        Input::Init();
    }

    Application::~Application()
    {
        ENGINE_PROFILE_FUNCTION();

        /*Shutdown Input Management*/
        Input::ShutDown();

        m_layerStack.PopOverlay(m_imGuiLayer);
        //delete m_imGuiLayer;
        m_imGuiLayer->~ImGuiLayer();

        m_window->~Window();
    }

    void Application::Run()
    {
        ENGINE_PROFILE_FUNCTION();

        //#define DEBUG_APP_LOGS
        #ifdef DEBUG_APP_LOGS
        {
            ENGINE_PROFILE_SCOPE("Application Debug");
            
            #define BASIC_DEBUG_LOGS
            #ifdef BASIC_DEBUG_LOGS
                    // Testing debug
                    bool pass = true;
                    ENGINE_ASSERT(true);
                    //ENGINE_ASSERT(false);
                    ENGINE_ASSERT_MSG(pass, "failed test case");
                    //ENGINE_ASSERT_MSG(!pass, "failed test case");

                    ENGINE_VERIFY(pass);
                    //ENGINE_VERIFY(!pass);
                    ENGINE_VERIFY_MSG(pass, "failed test case");
                    //ENGINE_VERIFY_MSG(!pass, "failed test case");

                    LOG_ENGINE_TRACE("Trace Log!");
                    LOG_ENGINE_INFO("Info Log!");
                    LOG_ENGINE_WARN("Warning Log!");
                    LOG_ENGINE_ERROR("Error Log!");
                    LOG_ENGINE_CRITICAL("Critical Log!");
            #endif //BASIC_DEBUG_LOG

            #define EVENTS_DEBUG_LOG
            #ifdef  EVENTS_DEBUG_LOG
                    //Debug log for events
                    std::vector<engine::Event*> events;

                    //windows events
                    engine::WindowResizeEvent           resizeEvent{ 1280,720 };                        events.push_back(&resizeEvent);
                    engine::WindowCloseEvent            closeEvent{};                                   events.push_back(&closeEvent);
                    engine::WindowFocusEvent            focusEvent{};                                   events.push_back(&focusEvent);
                    engine::WindowLoseFocusEvent        loseFocusEvent{};                               events.push_back(&loseFocusEvent);
                    engine::WindowMovedEvent            movedEvent{};                                   events.push_back(&movedEvent);
                                                                                                             
                    //keyboard events                                                                                    
                    engine::KeyPressedEvent             keyPressed{ 50 , 1 };                           events.push_back(&keyPressed);
                    engine::KeyReleasedEvent            keyReleased{ 50 };                              events.push_back(&keyReleased);
                    engine::KeyTypedEvent               keyTyped{ 50 };                                 events.push_back(&keyTyped);
                                                                                                             
                    //mouse events                                                                                       
                    engine::MouseMovedEvent             mouseMoved{ 10, 20 };                           events.push_back(&mouseMoved);
                    engine::MouseButtonPressedEvent     mousePressed{ engine::mouse::Button0 };         events.push_back(&mousePressed);
                    engine::MouseButtonReleasedEvent    mouseButtonReleased{ engine::mouse::Button0 };  events.push_back(&mouseButtonReleased);
                    engine::MouseScrolledEvent          mouseScrolled{ 20, 10 };                        events.push_back(&mouseScrolled);

                    std::cout << "EVENTS DEBUG" << std::endl;

                    std::cout << "INDIVIDUAL EVENT VIEW" << std::endl;

                    for (engine::Event* e : events)
                    {
                        std::cout << "[Event : " << e->ToString() << "]" << std::endl;

                        std::cout << "EVENT CATEGORY NONE : \t\t\t";
                        if (e->IsInCategory(engine::EVENT_CATEGORY::NONE))
                        {
                            std::cout << "[YES]";
                        }
                        else
                        {
                            std::cout << "[NO]";
                        }
                        std::cout << std::endl;

                        std::cout << "EVENT CATEGORY APPLICATION : \t\t";
                        if (e->IsInCategory(engine::EVENT_CATEGORY::APPLICATION))
                        {
                            std::cout << "[YES]";
                        }
                        else
                        {
                            std::cout << "[NO]";
                        }
                        std::cout << std::endl;

                        std::cout << "EVENT CATEGORY INPUT : \t\t\t";
                        if (e->IsInCategory(engine::EVENT_CATEGORY::INPUT))
                        {
                            std::cout << "[YES]";
                        }
                        else
                        {
                            std::cout << "[NO]";
                        }
                        std::cout << std::endl;

                        std::cout << "EVENT CATEGORY KEYBOARD : \t\t";
                        if (e->IsInCategory(engine::EVENT_CATEGORY::KEYBOARD))
                        {
                            std::cout << "[YES]";
                        }
                        else
                        {
                            std::cout << "[NO]";
                        }
                        std::cout << std::endl;

                        std::cout << "EVENT CATEGORY MOUSE : \t\t\t";
                        if (e->IsInCategory(engine::EVENT_CATEGORY::MOUSE))
                        {
                            std::cout << "[YES]";
                        }
                        else
                        {
                            std::cout << "[NO]";
                        }
                        std::cout << std::endl;

                        std::cout << "EVENT CATEGORY MOUSEBUTTON : \t\t";
                        if (e->IsInCategory(engine::EVENT_CATEGORY::MOUSEBUTTON))
                        {
                            std::cout << "[YES]";
                        }
                        else
                        {
                            std::cout << "[NO]";
                        }
                        std::cout << std::endl;

                    }

                    std::cout << "\n\n\n\n";

                    std::cout << "Event CATEGORIC VIEW" << std::endl;

                    std::cout << "EVENT CATEGORY NONE" << std::endl;
                    for (engine::Event* e : events)
                    {
                        if (e->IsInCategory(engine::EVENT_CATEGORY::NONE))
                        {
                            LOG_ENGINE_TRACE(e->ToString());
                        }
                    }

                    std::cout << "EVENT CATEGORY APPLICATION" << std::endl;
                    for (engine::Event* e : events)
                    {
                        if (e->IsInCategory(engine::EVENT_CATEGORY::APPLICATION))
                        {
                            LOG_ENGINE_TRACE(e->ToString());
                        }
                    }

                    std::cout << "EVENT CATEGORY INPUT" << std::endl;
                    for (engine::Event* e : events)
                    {
                        if (e->IsInCategory(engine::EVENT_CATEGORY::INPUT))
                        {
                            LOG_ENGINE_TRACE(e->ToString());
                        }
                    }

                    std::cout << "EVENT CATEGORY KEYBOARD" << std::endl;
                    for (engine::Event* e : events)
                    {
                        if (e->IsInCategory(engine::EVENT_CATEGORY::KEYBOARD))
                        {
                            LOG_ENGINE_TRACE(e->ToString());
                        }
                    }

                    std::cout << "EVENT CATEGORY MOUSE" << std::endl;
                    for (engine::Event* e : events)
                    {
                        if (e->IsInCategory(engine::EVENT_CATEGORY::MOUSE))
                        {
                            LOG_ENGINE_TRACE(e->ToString());
                        }
                    }

                    std::cout << "EVENT CATEGORY MOUSEBUTTON" << std::endl;
                    for (engine::Event* e : events)
                    {
                        if (e->IsInCategory(engine::EVENT_CATEGORY::MOUSEBUTTON))
                        {
                            LOG_ENGINE_TRACE(e->ToString());
                        }
                    }

            #endif  //EVENT_DEBUG_LOG
        }
        #endif  //DEBUG_APP_LOGS
        
        while (m_running)
        {
            ENGINE_PROFILE_SCOPE("Runloop");

            // Process all adding and removing of layers
            ProcessLayers();

            /*Calculate dt*/
            Timestep dt{ m_window->CalcDeltaTime() };

            /*Update Input Management here*/
            Input::Update();

            /* Process input events */
            m_window->ProcessEvents();

            //whatever the renderer needs to call at the beggining if each frame e.g. clear color
            m_renderer->OnUpdateBegin();

            // Layerstack update : layers gets drawn first followed by overlays
            // starting with the standard layers
            {
                ENGINE_PROFILE_SCOPE("LayerStack OnUpdate");

                for (Layer* layer : m_layerStack)
                {
                    layer->OnUpdateBegin(dt);
                    layer->OnUpdate(dt);
                    layer->OnUpdateEnd(dt);
                }
            }

            // followed by imgui updates
            m_imGuiLayer->Begin();
            {
                ENGINE_PROFILE_SCOPE("LayerStack OnImGuiUpdate");

                for (Layer* layer : m_layerStack)
                {
                    layer->OnImGuiRender();
                }
            }
            m_imGuiLayer->End();

            m_window->SwapBuffers();
            //m_window->OnUpdate(dt);

        }
    }

    void Application::Close()
    {
        m_running = false;
    }
    
    void Application::OnEvent(Event& e)
    {
        ENGINE_PROFILE_FUNCTION();

        //Log events
        //LOG_ENGINE_INFO("{0}", e);

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(ENGINE_BIND_EVENT_FN(Application::OnWindowClose));

        for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it)
        {
            (*it)->OnEvent(e);

            // if event is handled, stop propogating
            if (e.Handled) break;
        }

    }

    void Application::PushLayer(Layer* layer)
    {
        m_addLayerQueue.emplace(layer);
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_addOverlayQueue.emplace(overlay);
    }

    void Application::PopLayer(Layer* layer)
    {
        m_removeLayerQueue.emplace(layer);
    }

    void Application::PopOverlay(Layer* overlay)
    {
        m_removeOverlayQueue.emplace(overlay);
    }

    void Application::ProcessLayers()
    {
        ENGINE_PROFILE_FUNCTION();

        while (!m_removeLayerQueue.empty())
        {
            auto& layer = m_removeLayerQueue.front();
            m_layerStack.PopLayer(layer);
            m_removeLayerQueue.pop();
        }

        while (!m_removeOverlayQueue.empty())
        {
            auto& layer = m_removeOverlayQueue.front();
            m_layerStack.PopOverlay(layer);
            m_removeOverlayQueue.pop();
        }

        while (!m_addLayerQueue.empty())
        {
            auto& layer = m_addLayerQueue.front();
            m_layerStack.PushLayer(layer);
            m_addLayerQueue.pop();
        }

        while (!m_addOverlayQueue.empty())
        {
            auto& layer = m_addOverlayQueue.front();
            m_layerStack.PushOverlay(layer);
            m_addOverlayQueue.pop();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_running = false;
        return true;
    }

}