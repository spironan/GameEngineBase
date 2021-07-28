#include "pch.h"

#include "ScriptInfo.h"
#include "Scripting.h"
#include "ScriptSystem.h"

#include <filesystem>
namespace fs = std::filesystem;

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-gc.h>
#include <mono/metadata/threads.h>

#include "Engine/Core/Log.h"

namespace engine
{
    static std::string s_AssetDir = "../Scripting/src";
    static std::string s_OutputDir = "scripting.dll";

    struct ScriptSystem::ScriptSystemInfo
    {
        MonoDomain* domain;
        MonoAssembly* assembly;
        MonoImage* image;
        std::vector<ScriptClassInfo> classInfoList;

        ScriptSystemInfo() : domain { nullptr }, assembly{ nullptr }, image{ nullptr } {};

        void reset()
        {
            domain = nullptr;
            assembly = nullptr;
            image = nullptr;
            classInfoList.clear();
        }
    };

    /*-----------------------------------------------------------------------------*/
    /* Helper Functions                                                            */
    /*-----------------------------------------------------------------------------*/
    static bool CheckBaseClass(MonoClass* _class, MonoClass* desiredBase)
    {
        while (_class != nullptr)
        {
            if (_class == desiredBase)
                return true;
            _class = mono_class_get_parent(_class);
        }
        return false;
    }

    ScriptSystem::ScriptSystem(ECS_Manager& _ECS_Manager) : System{ _ECS_Manager }, systemInfo{ new ScriptSystemInfo() }
    {

    }

    ScriptSystem::~ScriptSystem()
    {
        if(mono_domain_get())
            mono_jit_cleanup(mono_domain_get());
        delete systemInfo;
    }

    void ScriptSystem::Compile()
    {
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
        systemInfo->reset();

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
        systemInfo->domain = mono_domain_create_appdomain((char*)"scripts", NULL);
        if (!mono_domain_set(systemInfo->domain, false))
        {
            LOG_ENGINE_ERROR("Script Loading Error: failed to set scripting domain");
        }

        systemInfo->assembly = mono_domain_assembly_open(systemInfo->domain, s_OutputDir.c_str());
        if (systemInfo->assembly == nullptr)
        {
            LOG_ENGINE_ERROR("Script Loading Error: failed to open assembly using " + s_OutputDir);
            systemInfo->reset();
            return;
        }
        systemInfo->image = mono_assembly_get_image(systemInfo->assembly);

        // get all script class info
        MonoClass* baseScriptClass = mono_class_from_name(systemInfo->image, "Ouroboros", "MonoBehaviour");
        const MonoTableInfo* tableInfo = mono_image_get_table_info(systemInfo->image, MONO_TABLE_TYPEDEF);
        unsigned int tableRows = mono_table_info_get_rows(tableInfo);
        for (unsigned int i = 1; i < tableRows; ++i)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(tableInfo, i, cols, MONO_TYPEDEF_SIZE);
            const char* name = mono_metadata_string_heap(systemInfo->image, cols[MONO_TYPEDEF_NAME]);
            const char* name_space = mono_metadata_string_heap(systemInfo->image, cols[MONO_TYPEDEF_NAMESPACE]);
            MonoClass* _class = mono_class_from_name(systemInfo->image, name_space, name);

            if (_class != baseScriptClass && CheckBaseClass(_class, baseScriptClass))
            {
                systemInfo->classInfoList.push_back(ScriptClassInfo(name_space, name));
            }
        }
    }
}