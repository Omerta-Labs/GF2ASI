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
   targetdir "C:/Users/conno/Documents/My Games/Godfather 2/scripts"
   debugcommand "C:/Users/conno/Documents/My Games/Godfather 2/godfather2.exe"
   debugdir "C:/Users/conno/Documents/My Games/Godfather 2/"
   includedirs { 
   "$(ProjectDir)" ,
   "$(ProjectDir)Source",
   "$(ProjectDir)Source/Packages",
   "vendors/detours",
   "vendors/polyhook/include",
   "vendors/libcurl/include",
   "vendors/lua/include",
   "vendors/sol/"
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
	  "vendors/discord/libs/discord_game_sdk.dll.lib",
	  "vendors/libcurl/debug/libcurl-d.lib",
          "vendors/lua/libs/lua_debug.lib"
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
	  "vendors/discord/libs/discord_game_sdk.dll.lib",
	  "vendors/libcurl/release/libcurl.lib",
          "vendors/lua/libs/lua_release.lib"
      }