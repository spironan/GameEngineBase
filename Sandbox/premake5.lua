project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir("%{wks.location}/bin/"..outputdir.. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/"..outputdir.."/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        "%{wks.location}/Engine/src",
        "%{wks.location}/Engine/vendor/spdlog/include",
        "%{wks.location}/Engine/vendor/sdl2/include"
    }

    links
    {
        "Engine"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "ENGINE_PLATFORM_WINDOWS"
        }
        
        --enable this post build command for 64 bit system
        architecture "x86_64"
        postbuildcommands
        {
            {"{COPY} %{wks.location}Engine/vendor/sdl2/lib/x64/SDL2.dll ../bin/" .. outputdir .. "/Sandbox"}
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
