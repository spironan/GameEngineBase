/************************************************************************************//*!
\file           ScriptSystem.cpp
\project        <PROJECT_NAME>
\author         Solomon Tan Teng Shue, t.tengshuesolomon, 620010020
\par            email: t.tengshuesolomon\@digipen.edu
\date           August 4, 2021
\brief          Defines the Script System that is responsible for C# compiling during runtime
                and for executing instructions for all scripts in all existing GameObjects

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"

#include "ScriptUtility.h"
#include "Scripting.h"
#include "ScriptSystem.h"

#include <filesystem>
namespace fs = std::filesystem;

#include "Engine/Core/Log.h"
#include "Engine/ECS/WorldManager.h"
#include "Engine/ECS/GameObject.h"

namespace engine
{
    static std::string s_AssetDir = "../Scripting/src";
    static std::string s_OutputDir = "scripting.dll";

    ScriptSystem::~ScriptSystem()
    {
        if(mono_domain_get())
            mono_jit_cleanup(mono_domain_get());
    }

    /*-----------------------------------------------------------------------------*/
    /* Compiling                                                                   */
    /*-----------------------------------------------------------------------------*/

    void ScriptSystem::Compile()
    {
        if (isPlaying)
        {
            LOG_WARN("Script Warning: you are currently in play mode");
            return;
        }

        if (!fs::exists(s_AssetDir))
        {
            LOG_ERROR("Script Compiling Error: Expected Scripting folder \"" + s_AssetDir + "\" does not exist");
            return;
        }

        // unload current system info, needed to allow for rebuild
        ScriptUtility::g_SystemInfo.Reset();

        // get all script filenames
        std::string scriptNames = "";
        for (const auto& entry : fs::recursive_directory_iterator(s_AssetDir))
        {
            // std::cout << entry.path().generic_string() << std::endl;
            if (entry.path().extension() == ".cs")
            {
                scriptNames += " " + entry.path().parent_path().generic_string() + "\\" + entry.path().filename().generic_string();
            }
        }

        // execute build command
        std::string command = "C:\\Windows\\Microsoft.NET\\Framework64\\v3.5\\csc.exe /t:library /out:" + s_OutputDir + scriptNames;
        FILE* result = _popen(command.c_str(), "r");
        if (result != nullptr)
        {
            char buffer[1024];
            //ignore copyright message
            for (unsigned int i = 0; i < 4; ++i)
            {
                fgets(buffer, 1024, result);
            }
            bool compileError = false;
            while (fgets(buffer, 1024, result))
            {
                LOG_ERROR(buffer);
                compileError = true;
            }
            if (!compileError)
            {
                LOG_ENGINE_TRACE("Script Compiling Successful");
            }
            fclose(result);
        }

        // load all system info for later use
        ScriptUtility::g_SystemInfo.Initialize(s_OutputDir.c_str());
    }

    /*-----------------------------------------------------------------------------*/
    /* Mode Functions                                                              */
    /*-----------------------------------------------------------------------------*/
    void ScriptSystem::StartPlay()
    {
        if (isPlaying)
            return;
        for (auto& scripting : m_ECS_Manager.GetComponentDenseArray<Scripting>())
        {
            scripting.SetUpPlay();
        }
        for (auto& scripting : m_ECS_Manager.GetComponentDenseArray<Scripting>())
        {
            scripting.StartPlay();
        }
        isPlaying = true;
    }

    void ScriptSystem::StopPlay()
    {
        if (!isPlaying)
            return;
        for (auto& scripting : m_ECS_Manager.GetComponentDenseArray<Scripting>())
        {
            scripting.StopPlay();
        }
        isPlaying = false;
    }

    /*-----------------------------------------------------------------------------*/
    /* Function Invoking                                                           */
    /*-----------------------------------------------------------------------------*/
    void ScriptSystem::InvokeFunctionAll(const char* functionName)
    {
        for (auto& scripting : m_ECS_Manager.GetComponentDenseArray<Scripting>())
        {
            scripting.InvokeFunctionAll(functionName);
        }
    }

    /*-----------------------------------------------------------------------------*/
    /* Debugging Tools                                                             */
    /*-----------------------------------------------------------------------------*/
    void ScriptSystem::DebugPrintInfo()
    {
        for (auto& scripting : m_ECS_Manager.GetComponentDenseArray<Scripting>())
        {
            scripting.DebugPrintInfo();
        }
        std::cout << std::endl;
    }
    void ScriptSystem::DebugPrint()
    {
        for (auto& scripting : m_ECS_Manager.GetComponentDenseArray<Scripting>())
        {
            scripting.DebugPrint();
        }
        std::cout << std::endl;
    }

    /*-----------------------------------------------------------------------------*/
    /* Entity Functions for C#                                                     */
    /*-----------------------------------------------------------------------------*/

    uint32_t CreateEntity()
    {
        engine::GameObject instance{ engine::WorldManager::GetActiveWorld().CreateEntity() };
        auto& scripting = instance.AddComponent<engine::Scripting>();
        scripting.SetUpPlay();
        // scripting.StartPlay();
        return scripting.GetGameObjectPtr();
    }

    void DestroyEntity(int id)
    {
        engine::WorldManager::GetActiveWorld().DestroyEntity(id);
    }
}