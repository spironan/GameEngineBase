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

    // Helper Function For temporary text file editor
    void SetScriptFieldValueFromString(engine::ScriptFieldValue& value, std::string stringValue)
    {
        engine::ScriptValueType type = value.GetValueType();
        switch (type)
        {
        case engine::ScriptValueType::BOOL:
            value.SetValue<bool>((std::atoi(stringValue.c_str()) != 0));
            break;
        case engine::ScriptValueType::INT:
            value.SetValue<int>(std::atoi(stringValue.c_str()));
            break;
        case engine::ScriptValueType::FLOAT:
            value.SetValue<float>(std::stof(stringValue.c_str()));
            break;
        case engine::ScriptValueType::STRING:
            value.SetValue<std::string>(stringValue);
            break;
        case engine::ScriptValueType::GAMEOBJECT:
            value.SetValue<engine::Entity>((engine::Entity)(std::atoi(stringValue.c_str())));
            break;
        case engine::ScriptValueType::CLASS:
        {
            std::vector<engine::ScriptFieldInfo> convertedValue = value.GetValue<std::vector<engine::ScriptFieldInfo>>();
            std::string fieldString;
            std::string fieldName;
            std::string::size_type separator = stringValue.find_first_of('|');
            while (true)
            {
                fieldString = stringValue.substr(0, separator);
                if (separator < stringValue.size())
                    stringValue = stringValue.substr(separator + 1, stringValue.size() - separator - 1);
                else
                    stringValue = "";

                separator = fieldString.find_first_of(' ');
                fieldName = fieldString.substr(0, separator);
                fieldString = fieldString.substr(separator + 1, fieldString.size() - separator - 1);
                for (unsigned int i = 0; i < convertedValue.size(); ++i)
                {
                    if (convertedValue[i].name != fieldName)
                        continue;
                    SetScriptFieldValueFromString(convertedValue[i].value, fieldString);
                    break;
                }

                if (stringValue.size() <= 0)
                {
                    break;
                }
                else
                {
                    separator = stringValue.find_first_of('|');
                    if (separator == std::string::npos)
                    {
                        separator = stringValue.size();
                    }
                }
            }
            value.SetValue<std::vector<engine::ScriptFieldInfo>>(convertedValue);
        }
        break;
        }
    }

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
        ss->InvokeFunctionAll("Update");

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
    }
};