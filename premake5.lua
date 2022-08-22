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


	