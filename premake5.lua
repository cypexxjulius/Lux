workspace "Lux"
	architecture "x64"

	startproject "LuxEngine"

	
	
	configurations 
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


-- Include directories

include "LuxEngine"
include "EngineLayer"
project "Premake"
	kind "Utility"	

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"**/premake5.lua",
		"premake5.lua"
	}


	postbuildmessage "Regenerating project files with Premake5!"
	postbuildcommands
	{
		"\"%{wks.location}scripts/vendorbin/premake5\" %{_ACTION} --file=\"%{wks.location}premake5.lua\""
	}

	

	



	