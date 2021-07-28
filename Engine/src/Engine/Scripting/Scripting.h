#pragma once

#include <vector>

#include "Engine/ECS/Component.h"
//#include "Engine/Scripting/ScriptInfo.h"

namespace engine
{
    // forward declaration
    class ScriptInfo;

    class Scripting final : public Component
    {
    public:


    private:
        std::vector<uint32_t> scriptList;
        std::vector<ScriptInfo> scriptInfoList;
    };
}