#pragma once

#include "UtilityLayers/SceneBaseLayer.h"

#include "Editor/Editor.h"
////for getting filepath
#include "Editor/EditorFileGroup.h"

class EditorLayer : public SceneBaseLayer
{
private:
    Editor m_editor;
    bool m_demo = true;

public:

    EditorLayer() : SceneBaseLayer{ "EditorLayer" }
    {
        LOG_WARN("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        LOG_ERROR("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
        LOG_TRACE("cccccccccccccccccccccccccccccccccccccc");

        using namespace engine;
        TextureDatabase::AddTexture("Ouroboros_Folder", TextureLoader::LoadFromFilePath("Icons/Ouroboros_Folder.png"));
        TextureDatabase::AddTexture("Ouroboros_GenericFile", TextureLoader::LoadFromFilePath("Icons/Ouroboros_GenericFile.png"));
        TextureDatabase::AddTexture("Ouroboros_MP3", TextureLoader::LoadFromFilePath("Icons/Ouroboros_MP3.png"));
        TextureDatabase::AddTexture("Ouroboros_PNG", TextureLoader::LoadFromFilePath("Icons/Ouroboros_PNG.png"));
        TextureDatabase::AddTexture("Ouroboros_WAV", TextureLoader::LoadFromFilePath("Icons/Ouroboros_WAV.png"));
        TextureDatabase::AddTexture("Ouroboros_Prefab", TextureLoader::LoadFromFilePath("Icons/Ouroboros_Prefab.png"));

        TextureDatabase::AddTexture("Ouroboros_Search_Icon_Black", TextureLoader::LoadFromFilePath("Icons/Ouroboros_Search_Icon_Black.png"));

        TextureDatabase::AddTexture("Ouroboros_Log_Icon_Black", TextureLoader::LoadFromFilePath("Icons/Ouroboros_Log_Icon_Black.png"));
        TextureDatabase::AddTexture("Ouroboros_Warning_Icon_Yellow", TextureLoader::LoadFromFilePath("Icons/Ouroboros_Warning_Icon_Yellow.png"));
        TextureDatabase::AddTexture("Ouroboros_Error_Icon_Red", TextureLoader::LoadFromFilePath("Icons/Ouroboros_Error_Icon_Red.png"));

    }

    virtual void OnUpdate(engine::Timestep dt) override
    {
    }

    virtual void OnImGuiRender() override
    {
        m_editor.ShowAllWidgets();
        //ImGui::ShowDemoWindow(&m_demo);
    }
};