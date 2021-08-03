#pragma once

#include <vector>

#include "Engine/ECS/Component.h"
#include "ScriptInfo.h"

namespace engine
{
    class Scripting final : public Component
    {
    public:
        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        Scripting() = delete;
        Scripting(Scripting const&) = default;
        Scripting(Scripting&&) = default;
        Scripting(Entity entityID, bool active = true);

        virtual ~Scripting() override;

        Scripting& operator=(Scripting const&) = default;
        Scripting& operator=(Scripting&&) = default;

        /*-----------------------------------------------------------------------------*/
        /* Script Functions                                                            */
        /*-----------------------------------------------------------------------------*/
        uint32_t AddScript(const char* _namespace, const char* _type, bool callAwake = true);
        uint32_t GetScript(const char* _namespace, const char* _type);
        void RemoveScript(const char* _namespace, const char* _type);

        /*-----------------------------------------------------------------------------*/
        /* Script Info Functions                                                       */
        /*-----------------------------------------------------------------------------*/
        ScriptInfo& AddScriptInfo(ScriptClassInfo const& classInfo);
        ScriptInfo* GetScriptInfo(ScriptClassInfo const& classInfo);
        std::vector<ScriptInfo>& GetScriptInfoAll();
        void RemoveScriptInfo(ScriptClassInfo const& classInfo);

        /*-----------------------------------------------------------------------------*/
        /* Mode Functions                                                              */
        /*-----------------------------------------------------------------------------*/
        void SetUpPlay();
        void StartPlay();
        void StopPlay();

        /*-----------------------------------------------------------------------------*/
        /* Function Invoking                                                           */
        /*-----------------------------------------------------------------------------*/
        void InvokeFunctionAll(const char* functionName);

        /*-----------------------------------------------------------------------------*/
        /* Getters                                                                     */
        /*-----------------------------------------------------------------------------*/
        uint32_t GetGameObjectPtr();

        /*-----------------------------------------------------------------------------*/
        /* Debugging Tools                                                             */
        /*-----------------------------------------------------------------------------*/
        void DebugPrintInfo();
        void DebugPrint();

    private:
        uint32_t gameObjPtr;
        std::vector<uint32_t> scriptList;
        std::vector<ScriptInfo> scriptInfoList;
    };
}

/*-----------------------------------------------------------------------------*/
/* Script Functions for C#                                                     */
/*-----------------------------------------------------------------------------*/
extern "C"
{
    __declspec(dllexport) uint32_t AddScript(int id, const char* _namespace, const char* _type);
    __declspec(dllexport) uint32_t GetScript(int id, const char* _namespace, const char* _type);
    __declspec(dllexport) void RemoveScript(int id, const char* _namespace, const char* _type);

    __declspec(dllexport) uint32_t CreateEntity();
    __declspec(dllexport) void DestroyEntity(int id);
}