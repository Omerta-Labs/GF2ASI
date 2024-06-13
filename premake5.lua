-- premake5.lua
workspace "GF2ASI"
   configurations { "Debug", "Release" }
   platforms { "Win32" }
   flags("MultiProcessorCompile")

project "GF2ASI"
   kind "SharedLib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "on"
   characterset "MBCS"
   targetextension ".asi"
   targetdir "E:/Games/Godfather 2/The Godfather II/scripts"
   debugcommand "E:/Games/Godfather 2/The Godfather II/godfather2.exe"
   debugdir "E:/Games/Godfather 2/The Godfather II/"
   includedirs { 
   "$(ProjectDir)" ,
   "$(ProjectDir)Source",
   "$(ProjectDir)Source/Packages",
   "vendors/detours",
   "vendors/polyhook/include",
   }

   libdirs {
   "vendors/detours",
   }

   links {
   "detours.lib",
   }
   
   -- rc and aps is for icon
   files { "**.h", "**.c" , "**.cpp", "**.rc", "**.aps"}
   removefiles { "**vendors/**" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      architecture "x86"
      symbols "On"

      links {
	  "vendors/polyhook/libs/debug/asmjit.lib",
	  "vendors/polyhook/libs/debug/capstone.lib",
	  "vendors/polyhook/libs/debug/PolyHook_2.lib",
	  "vendors/polyhook/libs/debug/Zycore.lib",
	  "vendors/polyhook/libs/debug/Zydis.lib",
	  "vendors/discord/libs/discord_game_sdk.dll.lib"
      }

   filter "configurations:Release"
      defines { "NDEBUG" }
      architecture "x86"
      optimize "On"

      links {
	  "vendors/polyhook/libs/release/asmjit.lib",
	  "vendors/polyhook/libs/release/capstone.lib",
	  "vendors/polyhook/libs/release/PolyHook_2.lib",
	  "vendors/polyhook/libs/release/Zycore.lib",
	  "vendors/polyhook/libs/release/Zydis.lib",
	  "vendors/discord/libs/discord_game_sdk.dll.lib"
      }