project "Runtime"
   kind "WindowedApp"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   files { "Source/**.h", "Source/**.cpp" }

   local libDir = "../TooGoodEngine/Libs"

   includedirs
   {
      "Source",
      "../TooGoodEngine/Source",

      libDir    .. "/GLFW/include",
      libDir    .. "/glm",
      PythonDir .. "include"
   }

   libdirs 
   {
       PythonDir .. "/libs"
   }

   links
   {
      "TooGoodEngine"
   }


   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }
    
   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "Full"
       symbols "Off"

       libdirs
       {
           libDir .. "/Binaries/Dist"
       }
