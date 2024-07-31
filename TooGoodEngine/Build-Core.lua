function Split(input, seperator)
  local t = {}
  for match in (input .. seperator):gmatch("(.-)" .. seperator) do
    table.insert(t, match)
  end
  return t
end

function FindPython()
    local env = Split(os.getenv("Path"), ";")

   for _, path in ipairs(env) do
        if string.find(path, "Python") and not string.find(path, "Launcher") and not string.find(path, "Scripts") then
            return path
        end
    end
   
    print("Error no python found")
    return "error"
end

project "TooGoodEngine"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   files { "Source/**.h", "Source/**.cpp", "Source/**.hpp" }

   local pythonDir = FindPython()
   local libDir = "Libs"
   
   includedirs
   {
      -- Core includes --
      "Source",

      -- Vendor includes --
      pythonDir .. "include",
      libDir    .. "/AssimpWithPremake/include",
      libDir    .. "/GLFW/include",
      libDir    .. "/ImGui"
   }

   libdirs 
   {
       pythonDir .. "/libs",
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

group "Dependancies"
    include "Libs/Build-Assimp.lua"
    include "Libs/Build-GLFW.lua"
    include "Libs/Build-ImGui.lua"
group ""