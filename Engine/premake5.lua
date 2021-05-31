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
        "vendor/gl3w/GL/**.h",
        "vendor/gl3w/GL/**.c",
        "vendor/ImGui/**.h",
        "vendor/ImGui/**.cpp"
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
        --"%{wks.location}/Engine/vendor/sdl2/include/sdl2",--for imgui
        "%{wks.location}/Engine/vendor/rttr/include", --rttr
        "%{wks.location}/Engine/vendor/gl3w",
        "%{wks.location}/Engine/vendor/glm", -- GL maths
        "%{wks.location}/Engine/vendor/ImGui", --Dear ImGui
        "%{wks.location}/Engine/vendor/vkbootstrap", --Bootstrap
        "%{wks.location}/Engine/vendor/tinyobjloader", --tiny obj
        "%{wks.location}/Engine/vendor/vma", --Vulkan Memory Allocator
        "%{wks.location}/Engine/vendor/stb_image", --simple image library
		
		"C:/VulkanSDK/1.2.154.1/Include" -- Vulkan is fun
    }

    -- library diretories
    libdirs 
    {
        "vendor/sdl2/lib/x64",       
        "vendor/rttr/lib",		
		-- "vendor/tinyobjloader/lib" includes in release/debugs
        -- "vendor/vkbootstrap/lib" includes in release/debugs
		
		"C:/VulkanSDK/1.2.154.1/Lib" -- Vulkan is fun
    }

    -- linking External libraries 
    -- NOTE: do not put their extensions.
    links
    {
        "SDL2",
        "SDL2main",
        "SDL2test",
        "opengl32",
        "vulkan-1",
		"vkbootstrap",
		"tinyobjloader"
    }
    
    --Disable PCH beyond this point
	filter "files:vendor/**/**.cpp"
		flags { "NoPCH" }
	filter "files:vendor/**/**.c"
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
		
		-- library diretories
		libdirs 
		{
			"vendor/vkbootstrap/lib/Debug",
			"vendor/tinyobjloader/lib/Debug"
		}

    filter "configurations:Release"
        defines "ENGINE_RELEASE"
        optimize "On"
		
		-- library diretories
		libdirs 
		{
			"vendor/vkbootstrap/lib/Release",
			"vendor/tinyobjloader/lib/Release"
		}

    filter "configurations:Production"
        defines "ENGINE_PRODUCTION"
        optimize "On"
