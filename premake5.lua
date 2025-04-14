workspace "DeepWell"
	architecture "x64"
	
	configurations {
		"Debug",
		"Release"
	}

	startproject "DeepWell"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

newaction {
	trigger = "clean",
	description = "Remove all binaries, intermidiates and project files.",
	execute = function()
		print("Removing binaries...")
		os.rmdir("./bin")
		print("Removing intermidiates...")
		os.rmdir("./bin-int")
		print("Removing project files...")
		os.rmdir("./.vs")
		os.remove("**.sln")
		os.remove("**.vcxproj")
		os.remove("**.vcxproj.filters")
		os.remove("**.vcxproj.user")
		print("Done")
	end
}

project "DeepWell"
	location "DeepWell"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/libs/flecs/**.h",
		"%{prj.name}/libs/flecs/**.c"
	}

	--filter { "files:flecs.c" }
	--	compileas "C"
	--	buildoptions "-std=gnu99"

	includedirs{
		"Dependencies/PDCurses-3.9",
		"Dependencies/PDCurses-3.9/wincon",
		"%{prj.name}/libs/flecs"
	}

	libdirs{
		"Dependencies/PDCurses-3.9/wincon"
	}

	links{
		"pdcurses.lib"
	}

  --linkoptions{
  --  "--enable-sigwinch"
  --}

	defines{

	}

	filter { "configurations:Debug" }
		--buildoptions "/NODEFAULTLIB:MSVCRT"
		buildoptions "/MTd"
		runtime "Debug"
		symbols "on"

	filter { "configurations:Release" }
		--buildoptions "/NODEFAULTLIB:MSVCRT"
		buildoptions "/MT"
		runtime "Release"
		optimize "on"
