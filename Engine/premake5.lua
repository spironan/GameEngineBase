-- Core Engine Project
project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

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
        "src/**.cpp",
        "%{wks.location}/Engine/vendor/gl3w/GL/**.h",
        "%{wks.location}/Engine/vendor/gl3w/GL/**.c",
        "%{wks.location}/Engine/vendor/ImGui/**.h",
        "%{wks.location}/Engine/vendor/ImGui/**.cpp"
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
        "%{wks.location}/Engine/vendor/sdl2/include/sdl2",--for imgui
        "%{wks.location}/Engine/vendor/rttr/include", --rttr
        "%{wks.location}/Engine/vendor/gl3w",
        "%{wks.location}/Engine/vendor/ImGui", --Dear ImGui

    }

    -- library diretories
    libdirs 
    {
        "vendor/sdl2/lib/x64",
        "vendor/rttr/lib"
    }

    -- linking External libraries 
    -- NOTE: do not put their extensions.
    links
    {
        "SDL2",
        "SDL2main",
        "SDL2test",
        "opengl32"
    }
    
    --Disable PCH beyond this point
    flags { "NoPCH" }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "off"
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
