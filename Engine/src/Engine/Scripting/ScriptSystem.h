#pragma once

#include "Engine/ECS/System.h"
#include "Scripting.h"

namespace engine
{
    class ScriptSystem final : public System
    {
    public:
        /*-----------------------------------------------------------------------------*/
        /* Constructors & Destructor                                                   */
        /*-----------------------------------------------------------------------------*/
        explicit ScriptSystem(ECS_Manager& _ECS_Manager) : System{ _ECS_Manager }, isPlaying{ false } {};
        ~ScriptSystem();

        /*-----------------------------------------------------------------------------*/
        /* Compiling                                                                   */
        /*-----------------------------------------------------------------------------*/
        void Compile();

        /*-----------------------------------------------------------------------------*/
        /* Mode Functions                                                              */
        /*-----------------------------------------------------------------------------*/
        void StartPlay();
        void StopPlay();

        /*-----------------------------------------------------------------------------*/
        /* Function Invoking                                                           */
        /*-----------------------------------------------------------------------------*/
        void InvokeFunctionAll(const char* functionName);

        /*-----------------------------------------------------------------------------*/
        /* Debugging Tools                                                             */
        /*-----------------------------------------------------------------------------*/
        void DebugPrintInfo();
        void DebugPrint();

    private:
        bool isPlaying;
    };
}