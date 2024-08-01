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

workspace "TooGoodEngine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "GoodEditor"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

   flags { "MultiProcessorCompile" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"
PythonDir = FindPython()

group "Core"
	include "TooGoodEngine/Build-Core.lua"
group ""

include "GoodEditor/Build-App.lua"