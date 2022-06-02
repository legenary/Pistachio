workspace "Pistachio"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--include directories relative to root folder (solution directories)
IncludeDir = {}
IncludeDir["GLFW"] = "Pistachio/vendor/GLFW/include"

include "Pistachio/vendor/GLFW"

project "Pistachio"
	location "Pistachio"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ptcpch.h"
	pchsource "Pistachio/src/ptcpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib" 
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"PTC_PLATFORM_WINDOWS",
			"PTC_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "PTC_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PTC_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PTC_DIST"
		optimize "On"



project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Pistachio/vendor/spdlog/include",
		"Pistachio/src"
	}

	links
	{
		"Pistachio"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"PTC_PLATFORM_WINDOWS",
		}


	filter "configurations:Debug"
		defines "PTC_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PTC_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PTC_DIST"
		optimize "On"

