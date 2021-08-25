-- Dependencies throughout project

VULKAN_SDK = os.getenv("VULKAN_SDK")

-- Include directories relative to root folder (solution directory)
IncludeDir                  = {}
IncludeDir["glad"]          = "%{wks.location}/Engine/vendor/glad/include"
IncludeDir["glm"]           = "%{wks.location}/Engine/vendor/glm"
IncludeDir["oom"]           = "%{wks.location}/Engine/vendor/oom"   -- custom maths library
IncludeDir["ImGui"]         = "%{wks.location}/Engine/vendor/ImGui"
IncludeDir["mono"]          = "%{wks.location}/Engine/vendor/mono/include/mono-2.0"
IncludeDir["rttr"]          = "%{wks.location}/Engine/vendor/rttr/include"
IncludeDir["SDL"]           = "%{wks.location}/Engine/vendor/sdl2/include"
IncludeDir["spdlog"]        = "%{wks.location}/Engine/vendor/spdlog/include"
IncludeDir["stb_image"]     = "%{wks.location}/Engine/vendor/stb_image"
IncludeDir["tinyobjloader"] = "%{wks.location}/Engine/vendor/tinyobjloader"
IncludeDir["vkbootstrap"]   = "%{wks.location}/Engine/vendor/vkbootstrap"
IncludeDir["vma"]           = "%{wks.location}/Engine/vendor/vma"
IncludeDir["VulkanSDK"]     = "%{VULKAN_SDK}/Include"

-- Sandbox side dependencies
IncludeDir["rapidjson"]     = "%{wks.location}/Engine/vendor/rpj"

-- Paths to various external libraries directories
LibraryDir                  = {}
LibraryDir["mono"]          = "%{wks.location}/Engine/vendor/mono/lib"
LibraryDir["rttr"]          = "%{wks.location}/Engine/vendor/rttr/lib"
LibraryDir["SDL"]           = "%{wks.location}/Engine/vendor/sdl2/lib/x64"
LibraryDir["tinyobjloader"] = "%{wks.location}/Engine/vendor/tinyobjloader/lib"
LibraryDir["vkbootstrap"]   = "%{wks.location}/Engine/vendor/vkbootstrap/lib"
LibraryDir["VulkanSDK"]     = "%{VULKAN_SDK}/Lib"
LibraryDir["oom"]           = "%{wks.location}/Engine/vendor/oom/lib"   -- custom maths library directory

-- Paths to libraries that will be used
Library                     = {}
Library["Vulkan"]           = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"