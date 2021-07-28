#pragma once

#include "Engine/ECS/System.h"

namespace engine
{
    class ScriptSystem final : public System
    {
        // forward declaration
        struct ScriptSystemInfo;

        /*-----------------------------------------------------------------------------*/
        /* Constructors & Destructor                                                   */
        /*-----------------------------------------------------------------------------*/
    public:
        explicit ScriptSystem(ECS_Manager& _ECS_Manager);
        ~ScriptSystem();

        /*-----------------------------------------------------------------------------*/
        /* Compiling                                                                   */
        /*-----------------------------------------------------------------------------*/
    public:
        void Compile();

    private:
        ScriptSystemInfo* systemInfo;
    };
}