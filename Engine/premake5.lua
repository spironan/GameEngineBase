-- Core Engine Project
project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    -- Engine output directory
    targetdir("%{wks.location}/bin/"..outputdir.. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/"..outputdir.."/%{prj.name}")

    --precompiled headers for engine
    pchheader "pch.h"
    pchsource "src/pch.cpp"
    
    --Disable PCH for vendor files
    filter "files:vendor/**/**.cpp"
        flags { "NoPCH" }
    filter "files:vendor/**/**.c"
        flags { "NoPCH" }
    filter {}   -- resets the filter

    -- Engine's files
    files
    {
        "src/**.h",
        "src/**.cpp",
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
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.SDL}",
        "%{IncludeDir.rttr}",
        "%{IncludeDir.gl3w}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.vkbootstrap}",
        "%{IncludeDir.tinyobjloader}",
        "%{IncludeDir.vma}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.VulkanSDK}",
    }

    -- library diretories
    libdirs 
    {
        "%{LibraryDir.SDL}",
        --"%{LibraryDir.rttr}",
        --"%{LibraryDir.VulkanSDK}",
    }

    -- linking External libraries 
    -- NOTE: do not put their extensions.
    links
    {
        "gl3w",
        "ImGui",
        "SDL2",
        "SDL2main",
        "SDL2test",
        "opengl32",
        --"vulkan-1",
        "%{Library.Vulkan}",
        "vkbootstrap",
        "tinyobjloader"
    }

    -- Graphic's Dependent defines
    filter "platforms:OpenGL"
        defines "GRAPHICS_CONTEXT_OPENGL"
        
    filter "platforms:Vulkan"
        defines "GRAPHICS_CONTEXT_VULKAN"

        postbuildcommands
        {
            {"call assets/shaders/compileShaders.bat"} -- SPIRV post build command
        }

        -- includedirs
        -- {
        --     "%{wks.location}/Engine/vendor/vkbootstrap",    -- Bootstrap
        --     "%{wks.location}/Engine/vendor/tinyobjloader",  -- tiny obj
        --     "%{wks.location}/Engine/vendor/vma",            -- Vulkan Memory Allocator
        --     "%{wks.location}/Engine/vendor/stb_image",      -- simple image library
        --     "C:/VulkanSDK/1.2.154.1/Include"                -- Vulkan is fun
        -- }

        -- libdirs
        -- {
        --     "C:/VulkanSDK/1.2.154.1/Lib" -- Vulkan is fun
        -- }

        -- links
        -- {    
        --     "vulkan-1",
        --     "vkbootstrap",
        --     "tinyobjloader"
        -- }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "off"
        systemversion "latest"

        defines
        {
            "ENGINE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "ENGINE_DEBUG"
        symbols "On"
    

    -- filter { "configurations:Debug", "platforms:Vulkan" }
    --     defines "ENGINE_DEBUG"
    --     symbols "On"
        -- library diretories
        libdirs 
        {
            "%{LibraryDir.tinyobjloader}/Debug",
            "%{LibraryDir.vkbootstrap}/Debug",
            
            --"vendor/vkbootstrap/lib/Debug",
            --"vendor/tinyobjloader/lib/Debug"
        }
        

    filter "configurations:Release"
        defines "ENGINE_RELEASE"
        optimize "On"
    
    -- filter { "configurations:Release", "platforms:Vulkan" }
    --     defines "ENGINE_RELEASE"
    --     optimize "On"
        -- library diretories
        libdirs 
        {
            "%{LibraryDir.tinyobjloader}/Release",
            "%{LibraryDir.vkbootstrap}/Release",

            --"vendor/vkbootstrap/lib/Release",
            --"vendor/tinyobjloader/lib/Release"
        }

    filter "configurations:Production"
        defines "ENGINE_PRODUCTION"
        optimize "On"
        
        libdirs 
        {
            "%{LibraryDir.tinyobjloader}/Release",
            "%{LibraryDir.vkbootstrap}/Release",
        }