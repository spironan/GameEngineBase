/************************************************************************************//*!
\file           ScriptingTestLayer.h
\project        <PROJECT_NAME>
\author         Solomon Tan Teng Shue, t.tengshuesolomon, 620010020
\par            email: t.tengshuesolomon\@digipen.edu
\date           August 3, 2021
\brief          Test scene for testing scripting functionality between C# and C++

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/

#pragma once

#include "UtilityLayers/SceneBaseLayer.h"

class ScriptingTestLayer : public SceneBaseLayer
{
private:
    std::vector<engine::GameObject> goList;
    bool isPlaying;

public:
    ScriptingTestLayer() 
        : SceneBaseLayer{ "ScriptingTestLayer" }
        , isPlaying{ false }
    {
    }

    void Init() final override
    {
        auto& ss = m_scene.GetWorld().RegisterSystem<engine::ScriptSystem>();
    }

    virtual void OnUpdate(engine::Timestep dt) override
    {
        engine::WorldManager::SetActiveWorld(m_scene.GetWorld().GetID());
        m_scene.GetWorld().GetSystem<engine::TransformSystem>()->Update();
        auto ss = m_scene.GetWorld().GetSystem<engine::ScriptSystem>();
        ss->InvokeFunctionAll("Update", 1, (float)dt.GetSeconds());

        if (engine::Input::IsKeyPressed(engine::KeyCode::P))
        {
            if (!isPlaying)
            {
                if(ss->StartPlay())
                    isPlaying = true;
            }
            else
            {
                if(ss->StopPlay())
                    isPlaying = false;
            }
        }

        if (engine::Input::IsKeyPressed(engine::KeyCode::O))
        {
            if (isPlaying)
                ss->DebugPrint();
            else
                ss->DebugPrintInfo();
        }

        m_scene.GetWorld().ProcessDeletions();
    }
};