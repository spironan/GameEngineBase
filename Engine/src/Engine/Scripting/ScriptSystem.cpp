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
#include "Engine/Core/Input.h"

#include "Engine/ECS/WorldManager.h"
#include "Engine/ECS/GameObject.h"

namespace engine
{
    static std::string s_ProjDir = "../Scripting/Scripting.csproj";
    static std::string s_OutputDir = "../Sandbox/scripting";
    static std::string s_OutputFileName = "scripting";
    static std::string s_ErrorsLogFile = "../Sandbox/scripting/errors.log";
    static std::string s_WarningsLogFile = "../Sandbox/scripting/warnings.log";

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

        if (!fs::exists(s_ProjDir))
        {
            LOG_ERROR("Script Compiling Error: Expected Scripting project at \"" + s_ProjDir + "\" does not exist");
            return;
        }

        // unload current system info, needed to allow for rebuild
        ScriptUtility::g_SystemInfo.Reset();

        // get all script filenames
        //std::string scriptNames = "";
        //for (const auto& entry : fs::recursive_directory_iterator(s_AssetDir))
        //{
        //    // std::cout << entry.path().generic_string() << std::endl;
        //    if (entry.path().extension() == ".cs")
        //    {
        //        scriptNames += " " + entry.path().parent_path().generic_string() + "\\" + entry.path().filename().generic_string();
        //    }
        //}
        
        // find msbuild path
        FILE* msbuildPathFile = _popen("\"C:\\Program Files (x86)\\Microsoft Visual Studio\\Installer\\vswhere.exe\" -latest -prerelease -products * -requires Microsoft.Component.MSBuild -find MSBuild\\**\\Bin\\MSBuild.exe", "r");
        if (msbuildPathFile == nullptr)
        {
            LOG_ERROR("Script Compiling Error: failed to find msbuild");
            return;
        }
        char buffer[1024];
        fgets(buffer, 1024, msbuildPathFile);
        std::string msbuildPath(buffer);
        msbuildPath = msbuildPath.substr(0, msbuildPath.size() - 1);
        _pclose(msbuildPathFile);

        // execute build command
        std::string command("\"\"" + msbuildPath + "\" " + s_ProjDir + " -noLogo -verbosity:quiet -t:Clean;Build -fl1 -flp1:logfile=" + s_ErrorsLogFile + ";errorsonly -fl2 -flp2:logfile=" + s_WarningsLogFile + ";warningsonly -p:Configuration=\"Debug OpenGL\";Platform=x64;OutputPath=" + s_OutputDir + ";AssemblyName=" + s_OutputFileName + "\"");
        FILE* compileResult = _popen(command.c_str(), "r");
        if (!compileResult)
        {
            LOG_ERROR("Script Compiling Error: failed to build");
            return;
        }
        _pclose(compileResult);

        std::string line;

        std::ifstream ifsWarnings(s_WarningsLogFile);
        if (!ifsWarnings)
        {
            LOG_ERROR("Script Compiling Error: Warning log file not generated");
            return;
        }
        while (std::getline(ifsWarnings, line))
        {
            LOG_WARN(line);
        }
        ifsWarnings.close();

        std::ifstream ifsErrors(s_ErrorsLogFile);
        if (!ifsErrors)
        {
            LOG_ERROR("Script Compiling Error: Error log file not generated");
            return;
        }
        bool hasError = false;
        while (std::getline(ifsErrors, line))
        {
            LOG_ERROR(line);
            hasError = true;
        }
        ifsErrors.close();
        if (hasError)
            return;

        //FILE* result = _popen(command.c_str(), "r");
        //if (result == nullptr)
        //{
        //    LOG_ERROR("Script Compiling Error: failed to build scripting.dll");
        //    return;
        //}

        //bool hasError = false;
        //while (fgets(buffer, 1024, result))
        //{
        //    LOG_ERROR(buffer);
        //    hasError = true;
        //}
        //fclose(result);
        //if (hasError)
        //    return;

        // load all system info for later use
        LOG_ENGINE_TRACE("Script Compiling Successful");
        ScriptUtility::g_SystemInfo.Initialize((s_OutputDir + "/" + s_OutputFileName + ".dll").c_str(), componentMap);
    }

    bool ScriptSystem::IsSetUp()
    {
        return ScriptUtility::g_SystemInfo.IsSetUp();
    }

    std::vector<ScriptClassInfo> const& ScriptSystem::GetScriptClassList()
    {
        return ScriptUtility::g_SystemInfo.classInfoList;
    }

    /*-----------------------------------------------------------------------------*/
    /* Mode Functions                                                              */
    /*-----------------------------------------------------------------------------*/
    bool ScriptSystem::StartPlay()
    {
        if (!ScriptUtility::g_SystemInfo.IsSetUp())
        {
            LOG_ERROR("Fix Compile Time Errors before entering play mode");
            return false;
        }
        if (isPlaying)
            return false;
        for (auto& scripting : m_ECS_Manager.GetComponentDenseArray<Scripting>())
        {
            scripting.SetUpPlay();
            for (auto const& component : componentMap)
            {
                if (component.second.Has(scripting.GetEntity()))
                {
                    std::string::size_type separator = component.first.find_last_of('.');
                    std::string name = component.first.substr(separator + 1, component.first.size() - separator - 1);
                    std::string name_space = component.first.substr(0, separator);
                    scripting.AddComponentInterface(name_space.c_str(), name.c_str());
                }
            }
        }
        for (auto& scripting : m_ECS_Manager.GetComponentDenseArray<Scripting>())
        {
            scripting.StartPlay();
        }
        isPlaying = true;
        return true;
    }

    bool ScriptSystem::StopPlay()
    {
        if (!isPlaying)
            return false;
        for (auto& scripting : m_ECS_Manager.GetComponentDenseArray<Scripting>())
        {
            scripting.StopPlay();
        }
        isPlaying = false;
        return true;
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

    /*-----------------------------------------------------------------------------*/
    /* Input Functions for C#                                                      */
    /*-----------------------------------------------------------------------------*/

    bool AnyKey()
    {
        return Input::IsAnyKeyDown();
    }

    bool AnyKeyDown()
    {
        return Input::IsAnyKeyPressed();
    }

    bool AnyKeyUp()
    {
        return Input::IsAnyKeyReleased();
    }

    bool GetKey(int key)
    {
        return Input::IsKeyDown(static_cast<KeyCode>(key));
    }

    bool GetKeyDown(int key)
    {
        return Input::IsKeyPressed(static_cast<KeyCode>(key));
    }

    bool GetKeyUp(int key)
    {
        return Input::IsKeyReleased(static_cast<KeyCode>(key));
    }

    void GetMousePosition(int* x, int* y)
    {
        std::pair<int, int> mousePos = Input::GetMousePosition();
        *x = mousePos.first;
        *y = mousePos.second;
    }

    void GetMouseDelta(int* x, int* y)
    {
        std::pair<int, int> mouseDelta = Input::GetMouseDelta();
        *x = mouseDelta.first;
        *y = mouseDelta.second;
    }

    bool AnyMouseButton()
    {
        return Input::IsAnyMouseButtonDown();
    }

    bool AnyMouseButtonDown()
    {
        return Input::IsAnyMouseButtonPressed();
    }

    bool AnyMouseButtonUp()
    {
        return Input::IsAnyMouseButtonReleased();
    }

    bool GetMouseButton(int button)
    {
        return Input::IsMouseButtonDown(static_cast<MouseCode>(button));
    }

    bool GetMouseButtonDown(int button)
    {
        return Input::IsMouseButtonPressed(static_cast<MouseCode>(button));
    }

    bool GetMouseButtonUp(int button)
    {
        return Input::IsMouseButtonReleased(static_cast<MouseCode>(button));
    }
}