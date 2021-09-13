#pragma once

#include "UtilityLayers/SceneBaseLayer.h"

#include "Editor/Editor.h"
#include "Editor/EditorFileGroup.h"
#include "Seralizer.h"
#include "Engine/Renderer/EditorCamera.h"

#include "Controller.h"

class EditorLayer : public engine::Layer
{
private:
    Editor m_editor;
    bool m_demo = true;
    Serializer m_serializer;
    Controller m_controller;

public:

    EditorLayer(engine::Application& app)
        : engine::Layer{ "EditorLayer" }
        , m_controller{ app }
    {
        LOG_WARN("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        LOG_ERROR("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
        LOG_TRACE("cccccccccccccccccccccccccccccccccccccc");
        
        //Register callbacks
        m_editor.PlayButton += [this]() { m_controller.Simulate(); };
        m_editor.StopButton += [this]() { m_controller.Stop(); };
        m_editor.PauseButton += [this]() { m_controller.Pause(); };
        m_editor.UnpauseButton += [this]() { m_controller.UnPause(); };
        m_editor.NextButton += [this]() { m_controller.Next(); };
    }

    virtual void OnUpdate(engine::Timestep dt) override
    {
        engine::EditorCamera::g_editorCam->OnUpdate(dt);
    }

    //TODO: Remove temporary code
    bool DoMwheel(engine::MouseScrolledEvent& e)
    {
        float delta = e.GetY();
        engine::EditorCamera::g_editorCam->UpdateScroll(delta);
        return true;
    }

    void OnEvent(engine::Event& e)
    {
        engine::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<engine::MouseScrolledEvent>(ENGINE_BIND_EVENT_FN(DoMwheel));
    }
    //TODO: end of todo

    virtual void OnImGuiRender() override
    {
        m_editor.ShowAllWidgets();
        //ImGui::ShowDemoWindow(&m_demo);
    }

};