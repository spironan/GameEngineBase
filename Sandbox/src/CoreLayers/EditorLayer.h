#pragma once

#include "UtilityLayers/SceneBaseLayer.h"

#include "Editor/Editor.h"
////for getting filepath
#include "Editor/EditorFileGroup.h"
#include "Seralizer.h"
#include "Engine/Renderer/EditorCamera.h"
class EditorLayer : public SceneBaseLayer
{
private:
    Editor m_editor;
    bool m_demo = true;
	Serializer m_serializer;
public:

    EditorLayer() : SceneBaseLayer{ "EditorLayer" }
    {
        LOG_WARN("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        LOG_ERROR("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
        LOG_TRACE("cccccccccccccccccccccccccccccccccccccc");

        using namespace engine;
        AssetManager::ImportAsset("Icons/Ouroboros_Folder.png");

        AssetManager::ImportAsset("Icons/Ouroboros_Folder.png");
        AssetManager::ImportAsset("Icons/Ouroboros_GenericFile.png");
        AssetManager::ImportAsset("Icons/Ouroboros_MP3.png");
        AssetManager::ImportAsset("Icons/Ouroboros_PNG.png");
        AssetManager::ImportAsset("Icons/Ouroboros_WAV.png");
        AssetManager::ImportAsset("Icons/Ouroboros_Prefab.png");
        
        AssetManager::ImportAsset("Icons/Ouroboros_Search_Icon_Black.png");
        
        AssetManager::ImportAsset("Icons/Ouroboros_Log_Icon_Black.png");
        AssetManager::ImportAsset("Icons/Ouroboros_Warning_Icon_Yellow.png");
        AssetManager::ImportAsset("Icons/Ouroboros_Error_Icon_Red.png");

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