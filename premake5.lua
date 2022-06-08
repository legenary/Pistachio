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
IncludeDir["Glad"] = "Pistachio/vendor/Glad/include"
IncludeDir["ImGui"] = "Pistachio/vendor/imgui"

include "Pistachio/vendor/GLFW"
include "Pistachio/vendor/Glad"
include "Pistachio/vendor/imgui"

project "Pistachio"
	location "Pistachio"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		--staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"PTC_PLATFORM_WINDOWS",
			"PTC_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "PTC_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "PTC_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "PTC_DIST"
		runtime "Release"
		optimize "On"



project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

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
		--staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"PTC_PLATFORM_WINDOWS",
		}


	filter "configurations:Debug"
		defines "PTC_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "PTC_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "PTC_DIST"
		runtime "Release"
		optimize "On"

