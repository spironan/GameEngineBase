project "Scripting"
	kind "SharedLib"
	language "C#"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.cs", 
	}
	
	links
	{
		"System.Numerics",
	}