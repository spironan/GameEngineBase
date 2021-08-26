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
        //, m_scene.GetWorld(){ engine::WorldManager::CreateWorld() }
        , isPlaying{ false }
    {
    }

    void Init() final override
    {
        auto& ss = m_scene.GetWorld().RegisterSystem<engine::ScriptSystem>();

        RootGameObject().AddComponent<engine::Scripting>();

        ss->Compile();
        if (!ss->IsSetUp())
            return;

        // read values from temporary text file editor to initialize objects
        std::fstream file("test.txt");
        if (!file)
        {
            LOG_ENGINE_ERROR("No test.txt file found");
            return;
        }
        std::string line;
        engine::GameObject m_obj{};
        RootGameObject().AddChild(m_obj);
        goList.emplace_back(m_obj);
        engine::Scripting* entityScripting = &(goList[0].AddComponent<engine::Scripting>());
        engine::ScriptInfo* scriptInfo = nullptr;
        engine::ScriptFieldInfo* multiLineField = nullptr;
        while (std::getline(file, line))
        {
            if (line.size() <= 0)
                continue;

            if (line == "OBJECTEND")
            {
                engine::GameObject new_obj{};
                RootGameObject().AddChild(new_obj);
                goList.push_back(new_obj);
                entityScripting = &(goList[goList.size() - 1].AddComponent<engine::Scripting>());
                scriptInfo = nullptr;
                multiLineField = nullptr;
            }
            else if (line == "SCRIPTEND")
            {
                scriptInfo = nullptr;
                multiLineField = nullptr;
            }
            else if (line == "LISTEND")
            {
                multiLineField = nullptr;
            }
            else if (scriptInfo == nullptr)
            {
                std::string name_space("");
                std::string name(line);
                std::string::size_type separator = line.find_first_of('/');
                if (separator != std::string::npos)
                {
                    name_space = line.substr(0, separator);
                    name = line.substr(separator + 1, line.size() - separator - 1);
                }
                scriptInfo = &(entityScripting->AddScriptInfo(engine::ScriptClassInfo(name_space, name)));
            }
            else if (multiLineField != nullptr)
            {
                switch (multiLineField->value.GetValueType())
                {
                case engine::ScriptValueType::LIST:
                {
                    std::vector<engine::ScriptClassInfo> genericsList = scriptInfo->classInfo.GetFieldGenericTypeParams(multiLineField->name.c_str());
                    if (genericsList.size() <= 0)
                        break;
                    engine::ScriptClassInfo valueClass = genericsList[0];
                    std::vector<engine::ScriptFieldValue> value = multiLineField->value.GetValue<std::vector<engine::ScriptFieldValue>>();
                    if (valueClass.name == "Boolean")
                    {
                        engine::ScriptFieldValue entry(false);
                        SetScriptFieldValueFromString(entry, line);
                        value.push_back(entry);
                    }
                    else if (valueClass.name == "Int32")
                    {
                        engine::ScriptFieldValue entry(0);
                        SetScriptFieldValueFromString(entry, line);
                        value.push_back(entry);
                    }
                    else if (valueClass.name == "Single")
                    {
                        engine::ScriptFieldValue entry(0.0f);
                        SetScriptFieldValueFromString(entry, line);
                        value.push_back(entry);
                    }
                    else if (valueClass.name == "String")
                    {
                        engine::ScriptFieldValue entry(std::string(""));
                        SetScriptFieldValueFromString(entry, line);
                        value.push_back(entry);
                    }
                    else
                    {
                        engine::ScriptFieldValue entry(valueClass.GetScriptFieldInfoAll());
                        SetScriptFieldValueFromString(entry, line);
                        value.push_back(entry);
                    }
                    multiLineField->value.SetValue(value);
                }
                break;
                }
            }
            else
            {
                std::string::size_type separator = line.find_first_of(' ');
                std::string fieldName = line.substr(0, separator);
                line = line.substr(separator + 1, line.size() - separator - 1);
                engine::ScriptFieldInfo* fieldInfo = scriptInfo->FindFieldInfo(fieldName);
                if (fieldInfo != nullptr)
                {
                    if (fieldInfo->value.GetValueType() == engine::ScriptValueType::LIST)
                    {
                        multiLineField = fieldInfo;
                    }
                    else
                    {
                        SetScriptFieldValueFromString(fieldInfo->value, line);
                    }
                }
            }
        }
    }

    virtual void OnUpdate(engine::Timestep dt) override
    {
        engine::WorldManager::SetActiveWorld(m_scene.GetWorld().GetID());
        m_scene.GetWorld().GetSystem<engine::TransformSystem>()->Update();
        auto ss = m_scene.GetWorld().GetSystem<engine::ScriptSystem>();
        ss->InvokeFunctionAll("Update");

        if (engine::Input::IsKeyPressed(engine::KeyCode::SPACE))
        {
            ss->Compile();
        }

        if (engine::Input::IsKeyPressed(engine::KeyCode::ENTER))
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

        if (engine::Input::IsKeyPressed(engine::KeyCode::BACKSPACE))
        {
            if (!ss->IsSetUp())
                return;
            // re-read and apply changed values from temporary text file editor
            std::fstream file("test.txt");
            if (!file)
            {
                LOG_ENGINE_ERROR("No test.txt file found");
                return;
            }
            std::string line;
            size_t goIndex = 0;
            engine::Scripting* entityScripting = nullptr;
            if (goList.size() <= 0)
            {
                engine::GameObject new_obj{};
                RootGameObject().AddChild(new_obj);
                goList.push_back(new_obj);
                entityScripting = &(goList[0].AddComponent<engine::Scripting>());
            }
            entityScripting = &(goList[0].GetComponent<engine::Scripting>());
            engine::ScriptInfo* scriptInfo = nullptr;
            engine::ScriptFieldInfo* multiLineField = nullptr;
            size_t multiLineFieldIndex = 0;
            while (std::getline(file, line))
            {
                if (line.size() <= 0)
                    continue;

                if (line == "OBJECTEND")
                {
                    ++goIndex;
                    if (goIndex >= goList.size())
                    {
                        goList.push_back(engine::GameObject{});
                        entityScripting = &(goList[goIndex].AddComponent<engine::Scripting>());
                    }
                    else
                    {
                        entityScripting = &(goList[goIndex].GetComponent<engine::Scripting>());
                    }
                    scriptInfo = nullptr;
                    multiLineField = nullptr;
                    multiLineFieldIndex = 0;
                }
                else if (line == "SCRIPTEND")
                {
                    scriptInfo = nullptr;
                    multiLineField = nullptr;
                    multiLineFieldIndex = 0;
                }
                else if (line == "LISTEND")
                {
                    if (multiLineField != nullptr)
                    {
                        std::vector<engine::ScriptFieldValue> value = multiLineField->value.GetValue<std::vector<engine::ScriptFieldValue>>();
                        if (value.size() > multiLineFieldIndex)
                        {
                            value.erase(value.begin() + multiLineFieldIndex, value.end());
                            multiLineField->value.SetValue(value);
                        }

                        multiLineField = nullptr;
                        multiLineFieldIndex = 0;
                    }
                }
                else if (scriptInfo == nullptr)
                {
                    std::string name_space("");
                    std::string name(line);
                    std::string::size_type separator = line.find_first_of('/');
                    if (separator != std::string::npos)
                    {
                        name_space = line.substr(0, separator);
                        name = line.substr(separator + 1, line.size() - separator - 1);
                    }
                    scriptInfo = entityScripting->GetScriptInfo(engine::ScriptClassInfo(name_space, name));
                    if (scriptInfo == nullptr)
                        entityScripting->AddScriptInfo(engine::ScriptClassInfo(name_space, name));
                }
                else if (multiLineField != nullptr)
                {
                    switch (multiLineField->value.GetValueType())
                    {
                    case engine::ScriptValueType::LIST:
                    {
                        std::vector<engine::ScriptClassInfo> genericsList = scriptInfo->classInfo.GetFieldGenericTypeParams(multiLineField->name.c_str());
                        if (genericsList.size() <= 0)
                            break;
                        engine::ScriptClassInfo valueClass = genericsList[0];
                        std::vector<engine::ScriptFieldValue> value = multiLineField->value.GetValue<std::vector<engine::ScriptFieldValue>>();
                        if (valueClass.name == "Boolean")
                        {
                            engine::ScriptFieldValue entry(false);
                            SetScriptFieldValueFromString(entry, line);
                            if (multiLineFieldIndex < value.size())
                                value[multiLineFieldIndex] = entry;
                            else
                                value.push_back(entry);
                        }
                        else if (valueClass.name == "Int32")
                        {
                            engine::ScriptFieldValue entry(0);
                            SetScriptFieldValueFromString(entry, line);
                            if (multiLineFieldIndex < value.size())
                                value[multiLineFieldIndex] = entry;
                            else
                                value.push_back(entry);
                        }
                        else if (valueClass.name == "Single")
                        {
                            engine::ScriptFieldValue entry(0.0f);
                            SetScriptFieldValueFromString(entry, line);
                            if (multiLineFieldIndex < value.size())
                                value[multiLineFieldIndex] = entry;
                            else
                                value.push_back(entry);
                        }
                        else if (valueClass.name == "String")
                        {
                            engine::ScriptFieldValue entry(std::string(""));
                            SetScriptFieldValueFromString(entry, line);
                            if (multiLineFieldIndex < value.size())
                                value[multiLineFieldIndex] = entry;
                            else
                                value.push_back(entry);
                        }
                        else
                        {
                            engine::ScriptFieldValue entry(valueClass.GetScriptFieldInfoAll());
                            SetScriptFieldValueFromString(entry, line);
                            if (multiLineFieldIndex < value.size())
                                value[multiLineFieldIndex] = entry;
                            else
                                value.push_back(entry);
                        }
                        multiLineField->value.SetValue(value);
                        ++multiLineFieldIndex;
                    }
                    break;
                    }
                }
                else
                {
                    std::string::size_type separator = line.find_first_of(' ');
                    std::string fieldName = line.substr(0, separator);
                    line = line.substr(separator + 1, line.size() - separator - 1);
                    engine::ScriptFieldInfo* fieldInfo = scriptInfo->FindFieldInfo(fieldName);
                    if (fieldInfo != nullptr)
                    {
                        if (fieldInfo->value.GetValueType() == engine::ScriptValueType::LIST)
                        {
                            multiLineField = fieldInfo;
                            multiLineFieldIndex = 0;
                        }
                        else
                        {
                            SetScriptFieldValueFromString(fieldInfo->value, line);
                        }
                    }
                }
            }

            if (goList.size() > goIndex + 1)
            {
                for (size_t i = goIndex + 1; i < goList.size(); ++i)
                {
                    goList[i].Destroy();
                }
                goList.erase(goList.begin() + goIndex + 1, goList.end());
            }
        }

        if (engine::Input::IsKeyPressed(engine::KeyCode::TAB))
        {
            if (isPlaying)
                ss->DebugPrint();
            else
                ss->DebugPrintInfo();
        }
    }
};