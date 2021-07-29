#include "pch.h"

#include "ScriptUtility.h"
#include "Scripting.h"
#include "ScriptSystem.h"

#include <filesystem>
namespace fs = std::filesystem;

#include "Engine/Core/Log.h"
#include "Engine/ECS/ECS_Manager.h"

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

        // unload current app domain
        MonoDomain* oldDomain = mono_domain_get();
        if (oldDomain != nullptr && oldDomain != mono_get_root_domain())
        {
            if (!mono_domain_set(mono_get_root_domain(), false))
            {
                LOG_ENGINE_ERROR("Script Compiling Error: failed to set root domain");
            }
            mono_domain_unload(oldDomain);
            // Trigger C# garbage collection, not necessary but good point to clean up stuff
            mono_gc_collect(mono_gc_max_generation());
        }
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

        // load scripting dll
        if (mono_domain_get() == nullptr)
        {
            MonoDomain* rootDomain = mono_jit_init("root");
            mono_thread_set_main(mono_thread_current());
        }
        ScriptUtility::g_SystemInfo.domain = mono_domain_create_appdomain((char*)"scripts", NULL);
        if (!mono_domain_set(ScriptUtility::g_SystemInfo.domain, false))
        {
            LOG_ENGINE_ERROR("Script Loading Error: failed to set scripting domain");
        }

        ScriptUtility::g_SystemInfo.assembly = mono_domain_assembly_open(ScriptUtility::g_SystemInfo.domain, s_OutputDir.c_str());
        if (ScriptUtility::g_SystemInfo.assembly == nullptr)
        {
            LOG_ENGINE_ERROR("Script Loading Error: failed to open assembly using " + s_OutputDir);
            ScriptUtility::g_SystemInfo.Reset();
            return;
        }
        ScriptUtility::g_SystemInfo.image = mono_assembly_get_image(ScriptUtility::g_SystemInfo.assembly);

        // get all script class info
        MonoClass* baseScriptClass = ScriptUtility::GetBaseScriptMonoClass();
        const MonoTableInfo* tableInfo = mono_image_get_table_info(ScriptUtility::g_SystemInfo.image, MONO_TABLE_TYPEDEF);
        unsigned int tableRows = mono_table_info_get_rows(tableInfo);
        for (unsigned int i = 1; i < tableRows; ++i)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(tableInfo, i, cols, MONO_TYPEDEF_SIZE);
            const char* name = mono_metadata_string_heap(ScriptUtility::g_SystemInfo.image, cols[MONO_TYPEDEF_NAME]);
            const char* name_space = mono_metadata_string_heap(ScriptUtility::g_SystemInfo.image, cols[MONO_TYPEDEF_NAMESPACE]);
            MonoClass* _class = mono_class_from_name(ScriptUtility::g_SystemInfo.image, name_space, name);

            if (_class != baseScriptClass && ScriptUtility::CheckBaseClass(_class, baseScriptClass))
            {
                ScriptUtility::g_SystemInfo.classInfoList.push_back(ScriptClassInfo(name_space, name));
            }
        }
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
    }

    void ScriptSystem::StopPlay()
    {
        if (!isPlaying)
            return;
        for (auto& scripting : m_ECS_Manager.GetComponentDenseArray<Scripting>())
        {
            scripting.StopPlay();
        }
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
}