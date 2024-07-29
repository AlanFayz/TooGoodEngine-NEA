project "GoodEditor"
   kind "ConsoleApp"
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

      libDir .. "/ImGui"
   }

   links
   {
      "TooGoodEngine",
      "ImGui"
   }


   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

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
       optimize "On"
       symbols "Off"

       libdirs
       {
           libDir .. "/Binaries/Dist"
       }