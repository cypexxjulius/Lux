project "EngineLayer"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs 
	{ 
		"%{wks.location}/LuxEngine/src",	
		"%{wks.location}/LuxEngine/lib/glm"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines "LX_PLATFORM_WINDOWS"
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

