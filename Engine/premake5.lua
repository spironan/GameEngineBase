-- Core Engine Project
project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    -- Engine output directory
    targetdir("%{wks.location}/bin/"..outputdir.. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/"..outputdir.."/%{prj.name}")

    --precompiled headers
    pchheader "pch.h"
    pchsource "src/pch.cpp"

    -- Engine's files
    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    -- Engine's defines 
    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    -- Engine's include directories
    includedirs
    {
        "src",
        "vendor/spdlog/include",
        "vendor/sdl2/include",

    }

    -- library diretories
    libdirs 
    {
        "vendor/sdl2/lib/x64",
        "vendor/rttr/lib",
        "vendor/mono/lib"
    }

    -- linking External libraries 
    -- NOTE: do not put their extensions.
    links
    {
        "SDL2",
        "SDL2main",
        "SDL2test"
    }
    
    --Disable PCH beyond this point
    flags { "NoPCH" }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            --"ENGINE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "ENGINE_DEBUG"
        symbols "On"


    filter "configurations:Release"
        defines "ENGINE_RELEASE"
        optimize "On"


    filter "configurations:Production"
        defines "ENGINE_PRODUCTION"
        optimize "On"
