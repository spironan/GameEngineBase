-- Dependencies throughout project

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["SPDLOG"]    = "%{wks.location}/Engine/vendor/spdlog/include"
IncludeDir["SDL"]       = "%{wks.location}/Engine/vendor/sdl2/include"
IncludeDir["RTTR"]       = "%{wks.location}/Engine/vendor/rttr/include/"
IncludeDir["RAPIDJSON"]       = "%{wks.location}/Engine/vendor/"
IncludeDir["MONO"]       = "%{wks.location}/Engine/vendor/mono/include/mono-2.0"