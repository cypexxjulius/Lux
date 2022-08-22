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


project "Premake"
	kind "Utility"	

	files
	{
		"**/premake5.lua",
		"premake5.lua"
	}

	

	

-- Include directories

include "LuxEngine"
include "EngineLayer"


	