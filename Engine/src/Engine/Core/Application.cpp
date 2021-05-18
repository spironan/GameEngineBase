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

namespace engine
{
    Application* Application::s_instance = nullptr;

    Application::Application(const std::string& name, CommandLineArgs args)
        :m_commandLineArgs{ args }, m_running{ true }
    {
        ENGINE_PROFILE_FUNCTION();

        ENGINE_ASSERT_MSG(!s_instance, "Application already exist!");
        s_instance = this;
        m_window = Window::Create(WindowProperties{ name });
        //Binds window callback to call Application::OnEvent
        m_window->SetEventCallback(ENGINE_BIND_EVENT_FN(Application::OnEvent));
    }

    Application::~Application()
    {
        ENGINE_PROFILE_FUNCTION();

        delete m_window;
    }

    void Application::Run()
    {
        ENGINE_PROFILE_FUNCTION();

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
                    engine::MouseButtonPressedEvent     mousePressed{ engine::Mouse::Button0 };         events.push_back(&mousePressed);
                    engine::MouseButtonReleasedEvent    mouseButtonReleased{ engine::Mouse::Button0 };  events.push_back(&mouseButtonReleased);
                    engine::MouseScrolledEvent          mouseScrolled{ 20, 10 };                        events.push_back(&mouseScrolled);

                    std::cout << "EVENTS DEBUG" << std::endl;

                    std::cout << "INDIVIDUAL EVENT VIEW" << std::endl;

                    for (engine::Event* e : events)
                    {
                        std::cout << "[Event : " << e->ToString() << "]" << std::endl;

                        std::cout << "EVENT CATEGORY NONE : \t\t\t";
                        if (e->IsInCategory(engine::EVENT_CATEGORY::None))
                        {
                            std::cout << "[YES]";
                        }
                        else
                        {
                            std::cout << "[NO]";
                        }
                        std::cout << std::endl;

                        std::cout << "EVENT CATEGORY APPLICATION : \t\t";
                        if (e->IsInCategory(engine::EVENT_CATEGORY::Application))
                        {
                            std::cout << "[YES]";
                        }
                        else
                        {
                            std::cout << "[NO]";
                        }
                        std::cout << std::endl;

                        std::cout << "EVENT CATEGORY INPUT : \t\t\t";
                        if (e->IsInCategory(engine::EVENT_CATEGORY::Input))
                        {
                            std::cout << "[YES]";
                        }
                        else
                        {
                            std::cout << "[NO]";
                        }
                        std::cout << std::endl;

                        std::cout << "EVENT CATEGORY KEYBOARD : \t\t";
                        if (e->IsInCategory(engine::EVENT_CATEGORY::Keyboard))
                        {
                            std::cout << "[YES]";
                        }
                        else
                        {
                            std::cout << "[NO]";
                        }
                        std::cout << std::endl;

                        std::cout << "EVENT CATEGORY MOUSE : \t\t\t";
                        if (e->IsInCategory(engine::EVENT_CATEGORY::Mouse))
                        {
                            std::cout << "[YES]";
                        }
                        else
                        {
                            std::cout << "[NO]";
                        }
                        std::cout << std::endl;

                        std::cout << "EVENT CATEGORY MOUSEBUTTON : \t\t";
                        if (e->IsInCategory(engine::EVENT_CATEGORY::MouseButton))
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
                        if (e->IsInCategory(engine::EVENT_CATEGORY::None))
                        {
                            LOG_ENGINE_TRACE(e->ToString());
                        }
                    }

                    std::cout << "EVENT CATEGORY APPLICATION" << std::endl;
                    for (engine::Event* e : events)
                    {
                        if (e->IsInCategory(engine::EVENT_CATEGORY::Application))
                        {
                            LOG_ENGINE_TRACE(e->ToString());
                        }
                    }

                    std::cout << "EVENT CATEGORY INPUT" << std::endl;
                    for (engine::Event* e : events)
                    {
                        if (e->IsInCategory(engine::EVENT_CATEGORY::Input))
                        {
                            LOG_ENGINE_TRACE(e->ToString());
                        }
                    }

                    std::cout << "EVENT CATEGORY KEYBOARD" << std::endl;
                    for (engine::Event* e : events)
                    {
                        if (e->IsInCategory(engine::EVENT_CATEGORY::Keyboard))
                        {
                            LOG_ENGINE_TRACE(e->ToString());
                        }
                    }

                    std::cout << "EVENT CATEGORY MOUSE" << std::endl;
                    for (engine::Event* e : events)
                    {
                        if (e->IsInCategory(engine::EVENT_CATEGORY::Mouse))
                        {
                            LOG_ENGINE_TRACE(e->ToString());
                        }
                    }

                    std::cout << "EVENT CATEGORY MOUSEBUTTON" << std::endl;
                    for (engine::Event* e : events)
                    {
                        if (e->IsInCategory(engine::EVENT_CATEGORY::MouseButton))
                        {
                            LOG_ENGINE_TRACE(e->ToString());
                        }
                    }

            #endif  //EVENT_DEBUG_LOG
        }

        while (m_running)
        {
            ENGINE_PROFILE_SCOPE("Runloop");

            m_window->OnUpdate();
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
        LOG_ENGINE_INFO("{0}", e);

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(ENGINE_BIND_EVENT_FN(Application::OnWindowClose));
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_running = false;
        return true;
    }

}