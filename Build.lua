workspace "TooGoodEngine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "GoodEditor"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Core"
	include "TooGoodEngine/Build-Core.lua"
group ""

include "GoodEditor/Build-App.lua"