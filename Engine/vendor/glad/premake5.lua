project "glad"
    kind "StaticLib"
    language "C"
    staticruntime "off"

    -- output directory
    targetdir("%{wks.location}/bin/"..outputdir.. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/"..outputdir.."/%{prj.name}")

    files
    {
        "include/glad/**.h",
        "**.c",
    }

    includedirs
    {
        "%{IncludeDir.glad}",
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