#include "lib"

IncludeDirs = {}
IncludeDirs["glfw"]			= "lib/glfw/include"
IncludeDirs["glad"]			= "lib/glad/include"
IncludeDirs["glm"]			= "lib/glm"
IncludeDirs["stb_image"]	= "lib/stb"
IncludeDirs["msdf_atlas"]	= "lib/msdf"
IncludeDirs["msdf_core"]	= "lib/msdf/msdfgen"
IncludeDirs["fmt"]			= "lib/fmt/include"

group "Dependencies"
	include "lib"
group ""

project "LuxEngine"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
    

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "pch/pch.cpp"

	files
	{
		"pch/**.h",
		"pch/**.cpp",
		"src/**.h",
		"src/**.cpp",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"FMT_HEADER_ONLY"
	}

	includedirs
	{
		"src",
		"pch",
		"%{IncludeDirs.glfw}",
		"%{IncludeDirs.glad}",
		"%{IncludeDirs.stb_image}",
		"%{IncludeDirs.glm}",
		"%{IncludeDirs.msdf_atlas}",
		"%{IncludeDirs.msdf_core}",
		"%{IncludeDirs.fmt}"
	}

	forceincludes  
	{ 
		"pch.h",
	}


	links
	{
		"glfw",
		"freetype",
		"glad",
		"msdf_atlas_gen",
		"msdfgen",
		"EngineLayer",
		"opengl32.lib"
	}


	filter "system:linux"
		pic "On"
		systemversion "latest"

		defines
		{
			"LX_UNIX"
		}

	filter "system:windows"
		systemversion "latest"

		defines 
		{ 
			"LX_WIN",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines "LX_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "LX_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "LX_DIST"
		runtime "Release"
		optimize "on"
