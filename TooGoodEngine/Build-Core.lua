project "TooGoodEngine"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   files { "Source/**.h", "Source/**.cpp", "Source/**.hpp", "Source/**.c" }

   local libDir = "Libs"
   
   includedirs
   {
      -- Core includes --
      "Source",

      -- Vendor includes --
      PythonDir .. "include",
      libDir    .. "/AssimpWithPremake/include",
      libDir    .. "/GLFW/include",
      libDir    .. "/ImGui",
      libDir    .. "/glm"

   }

   libdirs 
   {
       PythonDir .. "/libs"
   }

   links 
   {
       "python3",
       "assimp",
       "glfw",
       "opengl32",
       "ImGui"
   }

   filter "system:windows"
       systemversion "latest"
       defines { }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Release"
       symbols "On"
       optimize "Off"

       libdirs
       {
           libDir .. "/Binaries/Debug"
       }

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

       libdirs
       {
           libDir .. "/Binaries/Release"
       }

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "Full"
       symbols "Off"
       vectorextensions "SSE2"

       libdirs
       {
           libDir .. "/Binaries/Dist"
       }

group "Dependancies"
    include "Libs/Build-Assimp.lua"
    include "Libs/Build-GLFW.lua"
    include "Libs/Build-ImGui.lua"
group ""