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

#include "ScriptUtilityAPI.h"
SCRIPT_API_ANCHOR(Scripting)

#include <filesystem>
namespace fs = std::filesystem;

#include "Engine/Core/Log.h"
#include "Engine/Core/Input.h"

#include "Engine/ECS/GameObject.h"

#include "Engine/Scene/SceneManager.h"

namespace engine
{
    bool ScriptSystem::s_IsPlaying = false;
    std::unordered_map<std::string, ScriptSystem::RegisteredComponent> ScriptSystem::s_ComponentMap{};

    static std::string s_ProjDir = "../Scripting/Scripting.csproj";
    static std::string s_OutputDir = "../Sandbox/scripting";
    static std::string s_OutputFileName = "scripting";
    static std::string s_OutputFilePath = s_OutputDir + "/" + s_OutputFileName + ".dll";
    static std::string s_ErrorsLogFile = "../Sandbox/scripting/errors.log";
    static std::string s_WarningsLogFile = "../Sandbox/scripting/warnings.log";

    /*-----------------------------------------------------------------------------*/
    /* Compiling                                                                   */
    /*-----------------------------------------------------------------------------*/

    void ScriptSystem::Initialize()
    {
        if (!fs::exists(s_OutputFilePath))
            return;
        ScriptUtility::g_SystemInfo.Initialize(s_OutputFilePath.c_str(), s_ComponentMap);
    }

    void ScriptSystem::Compile()
    {
        if (s_IsPlaying)
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

        // load all system info for later use
        LOG_ENGINE_TRACE("Script Compiling Successful");
        ScriptUtility::g_SystemInfo.Initialize(s_OutputFilePath.c_str(), s_ComponentMap);

        ScriptSystem* ss = SceneManager::GetActiveWorld().TryGetSystem<ScriptSystem>();
        if (ss != nullptr)
            ss->RefreshScriptInfoAll();
    }

    void ScriptSystem::CleanUp()
    {
        if (mono_domain_get())
            mono_jit_cleanup(mono_domain_get());
    }

    bool ScriptSystem::IsSetUp()
    {
        return ScriptUtility::g_SystemInfo.IsSetUp();
    }

    std::vector<ScriptClassInfo> const& ScriptSystem::GetScriptClassList()
    {
        return ScriptUtility::g_SystemInfo.classInfoList;
    }

    void ScriptSystem::RefreshScriptInfoAll()
    {
        for (auto& scripting : m_ECS_Manager.GetComponentDenseArray<Scripting>())
        {
            scripting.RefreshScriptInfoAll();
        }
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
        if (s_IsPlaying)
            return false;
        for (auto& scripting : m_ECS_Manager.GetComponentDenseArray<Scripting>())
        {
            scripting.SetUpPlay();
        }
        for (auto& scripting : m_ECS_Manager.GetComponentDenseArray<Scripting>())
        {
            scripting.StartPlay();
        }
        s_IsPlaying = true;
        return true;
    }

    bool ScriptSystem::StopPlay()
    {
        if (!s_IsPlaying)
            return false;
        for (auto& scripting : m_ECS_Manager.GetComponentDenseArray<Scripting>())
        {
            scripting.StopPlay();
        }
        s_IsPlaying = false;
        return true;
    }

    /*-----------------------------------------------------------------------------*/
    /* Function Invoking                                                           */
    /*-----------------------------------------------------------------------------*/
    void ScriptSystem::InvokeFunctionAll(const char* functionName, int paramCount, void** params)
    {
        if (!s_IsPlaying)
            return;
        for (auto& scripting : m_ECS_Manager.GetComponentDenseArray<Scripting>())
        {
            scripting.InvokeFunctionAll(functionName, paramCount, params);
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
}