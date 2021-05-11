-- premake 5 uses Lua scripting
-- single line comment
-- indentations are arbitary.
-- Always use forward slash, premake converts to the appropriate slash when built
workspace "GameEngineBase"
    architecture "x64"
    -- set startup project
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
-- Include directories relative to root folder(solution directory)
-- this is an example for SPDLOG but its not actually used.
IncludeDir = {}
IncludeDir["SPDLOG"] = "%{prj.name}/vendor/spdlog/include"

project "Engine"
    location "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    -- Engine output directory
    targetdir("bin/"..outputdir.. "/%{prj.name}")
    objdir("bin-int/"..outputdir.."/%{prj.name}")

    -- Engine's files
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    -- Engine's defines 
    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
    }

    -- Engine's include directories
    includedirs
    {
        "%{prj.name}/src",
        --"%{prj.name}/vendor/spdlog/include"
        "%{IncludeDir.SPDLOG}"
    }

    --precompiled headers
    pchheader ("pch.h")
    pchsource ("%{prj.name}/src/Engine/pch.cpp")

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "ENGINE_PLATFORM_WINDOWS"
        }

        -- postbuildcommands
        -- {
        --     ("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputdir.. "/Sandbox")
        -- }

    filter "configurations:Debug"
        defines "ENGINE_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "ENGINE_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "ENGINE_DIST"
        optimize "On"
    
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir("bin/"..outputdir.. "/%{prj.name}")
    objdir("bin-int/"..outputdir.."/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Engine/src",
        "%{prj.name}/src",
        "Engine/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "ENGINE_PLATFORM_WINDOWS"
        }

        links
        {
            "Engine"
        }

    filter "configurations:Debug"
        defines "ENGINE_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "ENGINE_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "ENGINE_DIST"
        optimize "On"
