-- premake 5 uses Lua scripting
-- single line comment
-- indentations are arbitary [tabs and spaces are not considered]
-- Always use forward slash '/' : premake auto converts to the appropriate slash 

--include "./vendor/premake/premake_customization/solution_items.lua"
include "dependencies.lua"

workspace "GameEngineBase"
    
    architecture "x86_64"
    startproject "Sandbox" -- set startup project

    configurations
    {
        "Debug",
        "Release",
        "Production"
    }

    flags
    {
        -- enable multicore compilation
        "MultiProcessorCompile"
    }

-- where the files are output to
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--group "Dependencies"
--    include "vendor/premake"
--    include "Engine/vendor/sdl2"
--group ""

include "Engine"
include "Sandbox"