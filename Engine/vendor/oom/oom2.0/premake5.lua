project "oom"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    -- output directory
    targetdir("%{wks.location}/bin/"..outputdir.. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/"..outputdir.."/%{prj.name}")

    files
    {
        "**.hpp",
        "**.cpp",
    }

    filter "system:windows"
        systemversion "latest"

    filter "system:linux"
        pic "On"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"