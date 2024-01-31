-- premake5.lua
workspace "Vizu"
   architecture "x64"
   configurations { "Debug", "Release" }
   startproject "VizuApp"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}/%{cfg.buildcfg}"

include "Core/Build-Core.lua"
include "App/Build-App.lua"