project "glad"
    kind "StaticLib"
    language "C"
	staticruntime "On"
    
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "glad/include/glad/glad.h",
        "glad/include/KHR/khrplatform.h",
        "glad/src/glad.c"
    }

    includedirs
    {
        "glad/include"
    }
    
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

project "glfw"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"glfw/include/GLFW/glfw3.h",
		"glfw/include/GLFW/glfw3native.h",
		"glfw/src/glfw_config.h",
		"glfw/src/context.c",
		"glfw/src/init.c",
		"glfw/src/input.c",
		"glfw/src/monitor.c",
		"glfw/src/vulkan.c",
		"glfw/src/window.c",
		"glfw/src/platform.c",
		"glfw/src/null_init.c",
		"glfw/src/null_window.c",
		"glfw/src/null_monitor.c",
		"glfw/src/null_joystick.c"
	}
	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

		files
		{
			"glfw/src/x11_init.c",
			"glfw/src/x11_monitor.c",
			"glfw/src/x11_window.c",
			"glfw/src/xkb_unicode.c",
			"glfw/src/posix_time.c",
			"glfw/src/posix_thread.c",
			"glfw/src/glx_context.c",
			"glfw/src/egl_context.c",
			"glfw/src/osmesa_context.c",
			"glfw/src/linux_joystick.c",
			"glfw/src/platform.c"
		}

		defines
		{
			"_GLFW_X11"
		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		files
		{
			"glfw/src/win32_init.c",
			"glfw/src/win32_joystick.c",
			"glfw/src/win32_monitor.c",
			"glfw/src/win32_module.c",
			"glfw/src/win32_time.c",
			"glfw/src/win32_thread.c",
			"glfw/src/win32_window.c",
			"glfw/src/wgl_context.c",
			"glfw/src/egl_context.c",
			"glfw/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

project "msdfgen"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
    

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"msdf/msdfgen/core/**.h",
		"msdf/msdfgen/core/**.cpp",
		"msdf/msdfgen/core/**.hpp",

		"msdf/msdfgen/ext/**.h",
		"msdf/msdfgen/ext/**.cpp",
		"msdf/msdfgen/ext/**.hpp",	
	}

	includedirs
	{
		"msdf/msdfgen/ext",
		"msdf/msdfgen/core",
		"freetype/include",
		"freetype/include/freetype",
		"msdf/msdfgen/include"
	}

	filter "system:windows"
            systemversion "latest"

   	filter "configurations:Debug"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            runtime "Release"
            optimize "on"

project "msdf_atlas_gen"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
    

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"msdf/msdf-atlas-gen/**.**"
	}

	removefiles 
	{
		"**/main.cpp"
	}
	
	defines { "_CRT_SECURE_NO_WARNINGS" }

	includedirs
	{
		"msdf/msdfgen/include",
		"msdf/artery-font-format",
		"msdf/msdfgen",
		"msdf",

	}

	filter "system:windows"
            systemversion "latest"

   	filter "configurations:Debug"
            runtime "Debug"
            symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

project "freetype"
	kind "StaticLib"	
	language "C"
	staticruntime "on"
	
	defines
	{
		"_LIB",
		"FT2_BUILD_LIBRARY",
		"_CRT_SECURE_NO_WARNINGS"
	}
	includedirs
	{
		"freetype/include",
	}
	files
	{
		"freetype/src/autofit/autofit.c",
		"freetype/src/base/ftbase.c",
		"freetype/src/base/ftbbox.c",
		"freetype/src/base/ftbdf.c",
		"freetype/src/base/ftbitmap.c",
		"freetype/src/base/ftcid.c",
		"freetype/src/base/ftfstype.c",
		"freetype/src/base/ftgasp.c",
		"freetype/src/base/ftglyph.c",
		"freetype/src/base/ftgxval.c",
		"freetype/src/base/ftinit.c",
		"freetype/src/base/ftmm.c",
		"freetype/src/base/ftotval.c",
		"freetype/src/base/ftpatent.c",
		"freetype/src/base/ftpfr.c",
		"freetype/src/base/ftstroke.c",
		"freetype/src/base/ftsynth.c",
		"freetype/src/base/fttype1.c",
		"freetype/src/base/ftwinfnt.c",
		"freetype/src/bdf/bdf.c",
		"freetype/src/bzip2/ftbzip2.c",
		"freetype/src/cache/ftcache.c",
		"freetype/src/cff/cff.c",
		"freetype/src/cid/type1cid.c",
		"freetype/src/gzip/ftgzip.c",
		"freetype/src/lzw/ftlzw.c",
		"freetype/src/pcf/pcf.c",
		"freetype/src/pfr/pfr.c",
		"freetype/src/psaux/psaux.c",
		"freetype/src/pshinter/pshinter.c",
		"freetype/src/psnames/psnames.c",
		"freetype/src/raster/raster.c",
		"freetype/src/sdf/sdf.c",
		"freetype/src/sfnt/sfnt.c",
		"freetype/src/smooth/smooth.c",
		"freetype/src/svg/svg.c",
		"freetype/src/truetype/truetype.c",
		"freetype/src/type1/type1.c",
		"freetype/src/type42/type42.c",
		"freetype/src/winfonts/winfnt.c",
		"freetype/include/ft2build.h",
		"freetype/include/freetype/config/ftconfig.h",
		"freetype/include/freetype/config/ftheader.h",
		"freetype/include/freetype/config/ftmodule.h",
		"freetype/include/freetype/config/ftoption.h",
		"freetype/include/freetype/config/ftstdlib.h",
	}


	filter "system:windows"
        systemversion "latest"
		files
		{
			"freetype/builds/windows/ftsystem.c",			
			"freetype/builds/windows/ftdebug.c"
		}

	filter "system:unix"
		files 
		{
			"freetype/builds/unix/ftsystem.c",
			"freetype/builds/unix/ftdebug.c"
		}


   	filter "configurations:Debug"
            runtime "Debug"
            symbols "on"

    	filter "configurations:Release"
        	runtime "Release"
        	optimize "on"

	filter {}

