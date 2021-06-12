project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir("%{wks.location}/bin/"..outputdir.. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/"..outputdir.."/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
        -- temporary fix using gl3w
        --"%{wks.location}/Sandbox/vendor/gl3w/GL/**.h",
        --"%{wks.location}/Sandbox/vendor/gl3w/GL/**.c",
        --"%{wks.location}/Sandbox/vendor/ImGui/**.h",
        --"%{wks.location}/Sandbox/vendor/ImGui/**.cpp"
    }

    includedirs
    {
        "src",
        "%{wks.location}/Engine/src",
        "%{wks.location}/Engine/vendor/spdlog/include",
        "%{wks.location}/Engine/vendor/sdl2/include/sdl2",
        "%{wks.location}/Engine/vendor/rttr/include", --rttr
        "%{wks.location}/Engine/vendor/mono/include/mono-2.0", --mono
        "%{wks.location}/Engine/vendor/gl3w",
        "%{wks.location}/Engine/vendor/ImGui", --Dear ImGui
        "%{wks.location}/SandBox/vendor/rpj" --rapidjson
    }

    -- library diretories
    libdirs 
    {
        "%{wks.location}/Engine/vendor/sdl2/lib/x64",
        "%{wks.location}/Engine/vendor/rttr/lib",
        "%{wks.location}/Engine/vendor/mono/lib"

    }

    links
    {
        "opengl32",
        "SDL2",
        "SDL2main",
        "SDL2test",
        "Engine",
        "mono-2.0-sgen",
        "MonoPosixHelper"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "off"
        systemversion "latest"

        defines
        {
            --"ENGINE_PLATFORM_WINDOWS"
            "SANDBOX_PLATFORM_WINDOWS"
        }
        
        --enable this post build command for 64 bit system
        architecture "x86_64"
        postbuildcommands
        {
            {"{COPY} %{wks.location}Engine/vendor/sdl2/lib/x64/SDL2.dll ../bin/" .. outputdir .. "/Sandbox"},
            {"{COPY} %{wks.location}Engine/vendor/mono/lib/mono ../bin/" .. outputdir .. "/Sandbox"},
            {"{COPY} %{wks.location}Engine/vendor/mono/dll/mono-2.0-sgen.dll ../bin/" .. outputdir .. "/Sandbox"}
        }

    filter "configurations:Debug"
        defines "ENGINE_DEBUG"
        symbols "On"

        architecture "x86_64"
        postbuildcommands
        {
            {"{COPY} %{wks.location}Engine/vendor/rttr/dll/rttr_core_d.dll ../bin/" .. outputdir .. "/Sandbox"},
        }

        links
        {
            "rttr_core_d"
        }
        
    filter "configurations:Release"
        defines "ENGINE_RELEASE"
        optimize "On"

        architecture "x86_64"
        postbuildcommands
        {
            {"{COPY} %{wks.location}Engine/vendor/rttr/dll/rttr_core.dll ../bin/" .. outputdir .. "/Sandbox"}
        }

        links
        {
            "rttr_core"
        }
        
    filter "configurations:Production"
        defines "ENGINE_PRODUCTION"
        optimize "On"
        
        architecture "x86_64"
        postbuildcommands
        {
            {"{COPY} %{wks.location}Engine/vendor/rttr/dll/rttr_core.dll ../bin/" .. outputdir .. "/Sandbox"}
        }

        links
        {
            "rttr_core"
        }
        