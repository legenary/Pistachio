workspace "Pistachio"
	architecture "x64"
	startproject "Pistachio-Editor"

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
IncludeDir["glm"] = "Pistachio/vendor/glm"
IncludeDir["stb_image"] = "Pistachio/vendor/stb_image"
IncludeDir["box2d"] = "Pistachio/vendor/box2d"

group "Dependencies"
	include "Pistachio/vendor/GLFW"
	include "Pistachio/vendor/Glad"
	include "Pistachio/vendor/imgui"
	include "Pistachio/vendor/box2d"
group ""

project "Pistachio"
	location "Pistachio"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ptcpch.h"
	pchsource "Pistachio/src/ptcpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines{
		"_CRT_SECRURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.box2d}/include"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"Box2D",
		"opengl32.lib"
	}

	filter "system:windows"
		--staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"PTC_PLATFORM_WINDOWS",
			"PTC_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}


	filter "configurations:Debug"
		defines "PTC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PTC_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "PTC_DIST"
		runtime "Release"
		optimize "on"



project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
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
		"Pistachio/src",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.box2d}/include",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Pistachio"
	}

	filter "system:windows"
		systemversion "latest"
		
		defines
		{
			"PTC_PLATFORM_WINDOWS",
		}


	filter "configurations:Debug"
		defines "PTC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PTC_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "PTC_DIST"
		runtime "Release"
		optimize "on"

		
project "Pistachio-Editor"
	location "Pistachio-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
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
		"Pistachio/src",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.box2d}/include",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Pistachio"
	}

	filter "system:windows"
		systemversion "latest"
		
		defines
		{
			"PTC_PLATFORM_WINDOWS",
		}


	filter "configurations:Debug"
		defines "PTC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PTC_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "PTC_DIST"
		runtime "Release"
		optimize "on"