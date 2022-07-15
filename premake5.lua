workspace "Lux"
	architecture "x64"

	startproject "LuxEngine"

	
	
	configurations 
	{
		"Debug",
		"Release",
		"Dist"
	}

    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories

IncludeDirs = {}
IncludeDirs["GLFW"]			= "LuxEngine/lib/GLFW/include"
IncludeDirs["Glad"]			= "LuxEngine/lib/Glad/include"
IncludeDirs["glm"]			= "LuxEngine/lib/glm"
IncludeDirs["stb_image"]	= "LuxEngine/lib/stb"
IncludeDirs["msdf_atlas"]	= "LuxEngine/lib/msdf"
IncludeDirs["msdf_core"]	= "LuxEngine/lib/msdf/msdfgen"


include "LuxEngine/lib/GLFW"
include "LuxEngine/lib/Glad"
include "LuxEngine/lib/msdf"


		
project "EngineLayer"
	location "EngineLayer"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("binInt/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{ 
		"LuxEngine/src",	
		"LuxEngine/lib/glm"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines "LUX_PLATFORM_WINDOWS"
	filter "configurations:Debug"
		defines "LUX_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "LUX_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "LUX_DIST"
		runtime "Release"
		optimize "on"



project "LuxEngine"
	location "LuxEngine"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("binInt/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "%{prj.name}/pch/pch.cpp"

	targetdir "../EngineLayer"


	files
	{
		"%{prj.name}/pch/**.h",
		"%{prj.name}/pch/**.cpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}
	

	includedirs 
	{ 
		"%{prj.name}/src",
		"%{prj.name}/pch",
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.Glad}",
		"%{IncludeDirs.stb_image}",
		"%{IncludeDirs.glm}",
		"%{IncludeDirs.msdf_atlas}",
		"%{IncludeDirs.msdf_core}",
	}
	
	forceincludes  
	{ 
		"pch.h",
	}

	links
	{
		"GLFW",
		"GLAD",
		"MSDF_ATLAS",
		"MSDF_CORE",
		"EngineLayer",
		"LuxEngine/lib/freetype/freetype.lib",
		"opengl32.lib"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"LUX_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",
			"LUX_ENABLE_ASSERTS",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines "LUX_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "LUX_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "LUX_DIST"
		runtime "Release"
		optimize "on"


