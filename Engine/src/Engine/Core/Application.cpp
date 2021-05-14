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

namespace Engine
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name, CommandLineArgs args)
        :_commandLineArgs{ args }, _running{ true }
    {
        ENGINE_ASSERT_MSG(!s_Instance, "Application already exist!");
        s_Instance = this;
        //TODO : use window to display name of application
        m_Window = Window::Create(WindowProperties{ name });
        //Binds window callback to call Application::OnEvent
        m_Window->SetEventCallback(ENGINE_BIND_EVENT_FN(Application::OnEvent));
    }

    Application::~Application()
    {
        delete m_Window;
    }

    void Application::Run()
    {

//#define BASIC_DEBUG_LOGS
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

//#define EVENTS_DEBUG_LOG
#ifdef  EVENTS_DEBUG_LOG
        //Debug log for events
        std::vector<Engine::Event*> events;

        //windows events
        Engine::WindowResizeEvent           resizeEvent{ 1280,720 };                        events.push_back(&resizeEvent);
        Engine::WindowCloseEvent            closeEvent{};                                   events.push_back(&closeEvent);
        Engine::WindowFocusEvent            focusEvent{};                                   events.push_back(&focusEvent);
        Engine::WindowLoseFocusEvent        loseFocusEvent{};                               events.push_back(&loseFocusEvent);
        Engine::WindowMovedEvent            movedEvent{};                                   events.push_back(&movedEvent);
                                                                                                             
        //keyboard events                                                                                    
        Engine::KeyPressedEvent             keyPressed{ 50 , 1 };                           events.push_back(&keyPressed);
        Engine::KeyReleasedEvent            keyReleased{ 50 };                              events.push_back(&keyReleased);
        Engine::KeyTypedEvent               keyTyped{ 50 };                                 events.push_back(&keyTyped);
                                                                                                             
        //mouse events                                                                                       
        Engine::MouseMovedEvent             mouseMoved{ 10, 20 };                           events.push_back(&mouseMoved);
        Engine::MouseButtonPressedEvent     mousePressed{ Engine::Mouse::Button0 };         events.push_back(&mousePressed);
        Engine::MouseButtonReleasedEvent    mouseButtonReleased{ Engine::Mouse::Button0 };  events.push_back(&mouseButtonReleased);
        Engine::MouseScrolledEvent          mouseScrolled{ 20, 10 };                        events.push_back(&mouseScrolled);

        std::cout << "EVENTS DEBUG" << std::endl;

        std::cout << "INDIVIDUAL EVENT VIEW" << std::endl;

        for (Engine::Event* e : events)
        {
            std::cout << "[Event : " << e->ToString() << "]" << std::endl;

            std::cout << "EVENT CATEGORY NONE : \t\t\t";
            if (e->IsInCategory(Engine::EventCategory::None))
            {
                std::cout << "[YES]";
            }
            else
            {
                std::cout << "[NO]";
            }
            std::cout << std::endl;

            std::cout << "EVENT CATEGORY APPLICATION : \t\t";
            if (e->IsInCategory(Engine::EventCategory::Application))
            {
                std::cout << "[YES]";
            }
            else
            {
                std::cout << "[NO]";
            }
            std::cout << std::endl;

            std::cout << "EVENT CATEGORY INPUT : \t\t\t";
            if (e->IsInCategory(Engine::EventCategory::Input))
            {
                std::cout << "[YES]";
            }
            else
            {
                std::cout << "[NO]";
            }
            std::cout << std::endl;

            std::cout << "EVENT CATEGORY KEYBOARD : \t\t";
            if (e->IsInCategory(Engine::EventCategory::Keyboard))
            {
                std::cout << "[YES]";
            }
            else
            {
                std::cout << "[NO]";
            }
            std::cout << std::endl;

            std::cout << "EVENT CATEGORY MOUSE : \t\t\t";
            if (e->IsInCategory(Engine::EventCategory::Mouse))
            {
                std::cout << "[YES]";
            }
            else
            {
                std::cout << "[NO]";
            }
            std::cout << std::endl;

            std::cout << "EVENT CATEGORY MOUSEBUTTON : \t\t";
            if (e->IsInCategory(Engine::EventCategory::MouseButton))
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
        for (Engine::Event* e : events)
        {
            if (e->IsInCategory(Engine::EventCategory::None))
            {
                LOG_ENGINE_TRACE(e->ToString());
            }
        }

        std::cout << "EVENT CATEGORY APPLICATION" << std::endl;
        for (Engine::Event* e : events)
        {
            if (e->IsInCategory(Engine::EventCategory::Application))
            {
                LOG_ENGINE_TRACE(e->ToString());
            }
        }

        std::cout << "EVENT CATEGORY INPUT" << std::endl;
        for (Engine::Event* e : events)
        {
            if (e->IsInCategory(Engine::EventCategory::Input))
            {
                LOG_ENGINE_TRACE(e->ToString());
            }
        }

        std::cout << "EVENT CATEGORY KEYBOARD" << std::endl;
        for (Engine::Event* e : events)
        {
            if (e->IsInCategory(Engine::EventCategory::Keyboard))
            {
                LOG_ENGINE_TRACE(e->ToString());
            }
        }

        std::cout << "EVENT CATEGORY MOUSE" << std::endl;
        for (Engine::Event* e : events)
        {
            if (e->IsInCategory(Engine::EventCategory::Mouse))
            {
                LOG_ENGINE_TRACE(e->ToString());
            }
        }

        std::cout << "EVENT CATEGORY MOUSEBUTTON" << std::endl;
        for (Engine::Event* e : events)
        {
            if (e->IsInCategory(Engine::EventCategory::MouseButton))
            {
                LOG_ENGINE_TRACE(e->ToString());
            }
        }

#endif  //EVENT_DEBUG_LOG

        while (_running)
        {
            m_Window->OnUpdate();
        }
    }

    void Application::Close()
    {
        _running = false;
    }
    
    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(ENGINE_BIND_EVENT_FN(Application::OnWindowClose));
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        _running = false;
        return true;
    }

}