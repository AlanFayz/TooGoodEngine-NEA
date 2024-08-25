project "ImGui"
	kind "StaticLib"
	language "C++"

	targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

	includedirs
	{
		"ImGui", 
		"GLFW/include"
	}

	files
	{
		"ImGui/imconfig.h",
		"ImGui/imgui.h",
		"ImGui/imgui.cpp",
		"ImGui/imgui_draw.cpp",
		"ImGui/imgui_internal.h",
		"ImGui/imgui_widgets.cpp",
		"ImGui/imstb_rectpack.h",
		"ImGui/imstb_textedit.h",
		"ImGui/imstb_truetype.h",
		"ImGui/imgui_demo.cpp", 
		"ImGui/imgui_tables.cpp",
		"ImGui/backends/imgui_impl_glfw.cpp",
		"ImGui/backends/imgui_impl_glfw.h",
		"ImGui/backends/imgui_impl_opengl3.cpp",
		"ImGui/backends/imgui_impl_opengl3.h"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++20"
		staticruntime "On"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++20"
		staticruntime "On"

	filter "configurations:Debug"
        	defines { "DEBUG" }
        	runtime "Debug"
        	symbols "on"

    filter "configurations:Release"
       	defines { "RELEASE" }
       	runtime "Release"
       	optimize "On"

    filter "configurations:Dist"
       	defines { "DIST" }
       	runtime "Release"
       	optimize "Full"
       	symbols "Off"
	
	
