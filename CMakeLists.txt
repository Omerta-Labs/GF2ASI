#=============================================================================
# GF2ASI -- The Godfather II script hook
#
# Three targets, each depending only on the one below it:
#
#   GF2ASI   SharedLib   start-up, detours, the menu
#   GF2Mod   StaticLib   logging, settings, ImGui, mod loading, game fixes
#   GF2SDK   StaticLib   reconstructed game code, plus the platform primitives
#                        it is built on
#
# Generate (32-bit is mandatory -- the game is a 32-bit process):
#
#   cmake --preset vs2022
#   cmake --build build --config Release
#=============================================================================

cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0091 NEW)

project(GF2ASI LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

#-----------------------------------------------------------------------------
# Toolchain requirements
#-----------------------------------------------------------------------------

if(NOT MSVC)
    message(FATAL_ERROR
        "GF2ASI builds with MSVC only. Source/Platform/MemUtils.h uses __asm "
        "for the thunk that passes an argument in EAX, which clang-cl does not "
        "support in 32-bit mode.")
endif()

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR
        "GF2ASI must be built as 32-bit -- godfather2.exe is a 32-bit process "
        "and the SDK is full of hardcoded addresses into its image.\n"
        "Re-run CMake with:  -A Win32")
endif()

#-----------------------------------------------------------------------------
# Options
#-----------------------------------------------------------------------------

# Set any of these in LocalConfig.cmake, next to this file. It is read first and
# whatever it sets wins, it is not tracked, and editing it takes effect on the
# next configure -- unlike a default below, which an existing cache ignores.
# Copy LocalConfig.cmake.example to get started.
#
# They are cache variables otherwise, so a preset, the CMake GUI or -D all still
# work for anyone who prefers that.
#
# The NOT DEFINED guards are load-bearing. Creating a cache entry removes a
# normal variable of the same name, so an unguarded set(... CACHE ...) would
# discard the local override on the first configure and honour it on every one
# after -- which is worse than either behaviour on its own.
#
# Each docstring has to be a single argument. Split one across two strings and
# CMake stops recognising the CACHE signature, silently falls back to plain
# set(), and leaves behind a normal variable holding the literal text
# "value;CACHE;PATH;doc...". Nothing can then set it: there is no cache entry
# for a preset or the GUI to write, and a -D is shadowed by the normal variable.
# GF2ASI_GAME_DIR was in exactly that state.

include("${CMAKE_SOURCE_DIR}/LocalConfig.cmake" OPTIONAL)

if(NOT DEFINED GF2ASI_GAME_DIR)
    set(GF2ASI_GAME_DIR "J:/Games/The Godfather II" CACHE PATH
        "Game install; the .asi deploys to <dir>/scripts and the debugger starts here")
endif()

if(NOT DEFINED GF2ASI_GAME_EXE)
    set(GF2ASI_GAME_EXE "godfather2.exe" CACHE STRING
        "Executable inside GF2ASI_GAME_DIR that the debugger launches")
endif()

if(NOT DEFINED GF2ASI_DEBUGGER_ARGS)
    set(GF2ASI_DEBUGGER_ARGS "" CACHE STRING
        "Extra command line the debugger passes to the game")
endif()

if(NOT DEFINED GF2ASI_DEPLOY)
    option(GF2ASI_DEPLOY
        "Copy the built .asi into GF2ASI_GAME_DIR/scripts after linking" ON)
endif()

if(NOT DEFINED GF2ASI_USE_PCH)
    option(GF2ASI_USE_PCH "Give each target a precompiled header" ON)
endif()

set(GF2ASI_GAME_PATH "${GF2ASI_GAME_DIR}/${GF2ASI_GAME_EXE}")

# Both of these are configure-time decisions, so a game directory that appears
# later -- an unmounted drive, a fresh install -- needs a reconfigure.
set(_gf2_can_deploy FALSE)

if(NOT IS_DIRECTORY "${GF2ASI_GAME_DIR}")
    message(WARNING
        "GF2ASI_GAME_DIR is not a directory [${GF2ASI_GAME_DIR}]. The build will "
        "not deploy and the debugger has nothing to launch. Set it in "
        "LocalConfig.cmake next to CMakeLists.txt -- copy "
        "LocalConfig.cmake.example.")
else()
    message(STATUS "GF2ASI: game directory ${GF2ASI_GAME_DIR}")
    if(GF2ASI_DEPLOY)
        set(_gf2_can_deploy TRUE)
    endif()

    if(NOT EXISTS "${GF2ASI_GAME_PATH}")
        message(WARNING
            "${GF2ASI_GAME_EXE} is not in ${GF2ASI_GAME_DIR}; set GF2ASI_GAME_EXE "
            "to the executable to debug.")
    endif()

    # Nothing loads a .asi on its own. This project is developed against the
    # dinput8.dll proxy loader; without a loader the game starts clean and the
    # deployed file is simply ignored, which looks exactly like a build that
    # did not take.
    if(NOT EXISTS "${GF2ASI_GAME_DIR}/dinput8.dll")
        message(WARNING
            "No ASI loader (dinput8.dll) in ${GF2ASI_GAME_DIR}; the game will "
            "start without loading the .asi.")
    endif()
endif()

#-----------------------------------------------------------------------------
# Shared settings
#-----------------------------------------------------------------------------
# Applied to every target through an interface library, so the three cannot
# drift apart in ways that only show up at link time.

add_library(gf2_settings INTERFACE)

target_compile_options(gf2_settings INTERFACE
    /std:c++latest      # what premake's cppdialect "C++20" emitted
    /MP
)

target_compile_definitions(gf2_settings INTERFACE
    _MBCS

    # DEBUG, not _DEBUG. Source tests `#if DEBUG` -- see the IMPLEMENT_*
    # switches in HookMods.cpp -- so relying on MSVC's own _DEBUG would
    # silently compile out every debug-only path.
    $<$<CONFIG:Debug>:DEBUG=1>
    $<$<CONFIG:Release>:NDEBUG>
)

# Repository root, Source/, and each SDK package's own source root, so an SDK
# include reads as it did in the original: "modules/families/family.h" rather
# than "SDK/ears_godfather/src/modules/families/family.h".
#
# ears_framework needs both of its roots: src/framework/ and
# src/game_framework/framework/ hold disjoint files sharing the framework/...
# prefix. Three packages that otherwise use include/ keep a few headers in
# src/, so those are listed too.
set(GF2ASI_SDK_ROOTS
    allocator/include
    ears_common/include
    ears_common/src
    ears_framework/src
    ears_framework/src/game_framework
    ears_godfather/src
    ears_locale/include
    ears_physics/src
    ears_rt_cct/include
    ears_rt_cct/src
    ears_rt_llrender/include
    ears_rt_llrender/src
    ears_statemachine/include
    ears_trinity/include
    ears_trinity/src
    rwcontroller/include
    rwfilesystem/include
)

target_include_directories(gf2_settings INTERFACE
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/Source"
)
foreach(_root ${GF2ASI_SDK_ROOTS})
    if(NOT IS_DIRECTORY "${CMAKE_SOURCE_DIR}/Source/SDK/${_root}")
        message(FATAL_ERROR
            "SDK include root Source/SDK/${_root} does not exist. The package "
            "layout and GF2ASI_SDK_ROOTS have drifted apart.")
    endif()
    target_include_directories(gf2_settings INTERFACE
        "${CMAKE_SOURCE_DIR}/Source/SDK/${_root}")
endforeach()

# Third-party headers, marked SYSTEM so their warnings stay out of the way.
target_include_directories(gf2_settings SYSTEM INTERFACE
    "${CMAKE_SOURCE_DIR}/Vendors/d3d9/include"
    "${CMAKE_SOURCE_DIR}/Vendors/detours"
    "${CMAKE_SOURCE_DIR}/Vendors/polyhook/include"
    "${CMAKE_SOURCE_DIR}/Vendors/libcurl/include"
    "${CMAKE_SOURCE_DIR}/Vendors/lua/include"
    "${CMAKE_SOURCE_DIR}/Vendors/sol"
)

#-----------------------------------------------------------------------------
# Sources
#-----------------------------------------------------------------------------
# Globbed with CONFIGURE_DEPENDS, so the file list follows the tree without a
# manual regenerate. Headers are listed only to place them in the IDE.

function(gf2_collect out_sources out_headers)
    set(_sources "")
    set(_headers "")
    foreach(_dir ${ARGN})
        file(GLOB_RECURSE _s CONFIGURE_DEPENDS
            "${CMAKE_SOURCE_DIR}/${_dir}/*.cpp" "${CMAKE_SOURCE_DIR}/${_dir}/*.c")
        file(GLOB_RECURSE _h CONFIGURE_DEPENDS
            "${CMAKE_SOURCE_DIR}/${_dir}/*.h" "${CMAKE_SOURCE_DIR}/${_dir}/*.hpp"
            "${CMAKE_SOURCE_DIR}/${_dir}/*.inl")
        list(APPEND _sources ${_s})
        list(APPEND _headers ${_h})
    endforeach()
    set(${out_sources} ${_sources} PARENT_SCOPE)
    set(${out_headers} ${_headers} PARENT_SCOPE)
endfunction()

#-----------------------------------------------------------------------------
# GF2SDK -- reconstructed game code
#-----------------------------------------------------------------------------
# Source/SDK mirrors the original package tree exactly; Source/Platform holds
# the primitives it is built on (calling into the game image, and the sinks the
# host fills in). Checked by Tools/check_layers.py: nothing here may include
# from Addons/, Scripthook/ or Utils/, or reference polyhook or ImGui.

gf2_collect(GF2SDK_SOURCES GF2SDK_HEADERS "Source/SDK" "Source/Platform")

add_library(GF2SDK STATIC ${GF2SDK_SOURCES} ${GF2SDK_HEADERS})
target_link_libraries(GF2SDK PUBLIC gf2_settings)
source_group(TREE "${CMAKE_SOURCE_DIR}" FILES ${GF2SDK_SOURCES} ${GF2SDK_HEADERS})

#-----------------------------------------------------------------------------
# GF2Mod -- modding infrastructure and game fixes
#-----------------------------------------------------------------------------
# Logging, settings, vendored ImGui and the Discord SDK, the mod loader, the
# hashed-name registry, the object manager, the modded state machines, and the
# four game fixes. The fixes live here because they depend only on settings,
# logging and the SDK -- none of them touches ImGui.

gf2_collect(GF2MOD_SOURCES GF2MOD_HEADERS
    "Source/Addons"
    "Source/Utils"
    "Source/Scripthook/SH_ModManager"
    "Source/Scripthook/SH_Discord"
    "Source/Scripthook/SH_HashedNames"
    "Source/Scripthook/SH_ObjectManager"
    "Source/Scripthook/SH_PlayerMasterSM"
    "Source/Scripthook/SH_GammaFix"
    "Source/Scripthook/SH_EdgeAA"
    "Source/Scripthook/SH_VSyncFix"
    "Source/Scripthook/SH_MugShotFix"
)

add_library(GF2Mod STATIC
    ${GF2MOD_SOURCES} ${GF2MOD_HEADERS}
    "${CMAKE_SOURCE_DIR}/framework.h"
)
target_link_libraries(GF2Mod PUBLIC gf2_settings GF2SDK)
source_group(TREE "${CMAKE_SOURCE_DIR}" FILES ${GF2MOD_SOURCES} ${GF2MOD_HEADERS})

#-----------------------------------------------------------------------------
# GF2ASI -- start-up, detours and the menu
#-----------------------------------------------------------------------------
# dllmain and GF2Hook own the start-up order and install every detour; nothing
# self-registers, so no object here can be dropped for want of a reference.
# SH_ImGui is the menu. Phase 4 moves the ImGui runtime -- context, backends,
# device reset, draw snapshot -- down into GF2Mod and leaves the panels here.

gf2_collect(GF2ASI_SOURCES GF2ASI_HEADERS
    "Source/Scripthook/SH_ImGui"
    "Source/Scripthook/SH_SDKHooks"
)

file(GLOB GF2ASI_NATVIS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Natvis/*.natvis")

add_library(GF2ASI SHARED
    ${GF2ASI_SOURCES} ${GF2ASI_HEADERS}
    "${CMAKE_SOURCE_DIR}/dllmain.cpp"
    "${CMAKE_SOURCE_DIR}/Source/GF2Hook.cpp"
    "${CMAKE_SOURCE_DIR}/Source/GF2Hook.h"
    "${CMAKE_SOURCE_DIR}/Source/Scripthook/HookMods.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Scripthook/HookMods.h"
    "${CMAKE_SOURCE_DIR}/Source/Scripthook/ScripthookEvents.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Scripthook/ScripthookEvents.h"
    ${GF2ASI_NATVIS}
)
target_link_libraries(GF2ASI PRIVATE gf2_settings GF2Mod)
source_group(TREE "${CMAKE_SOURCE_DIR}" FILES
    ${GF2ASI_SOURCES} ${GF2ASI_HEADERS} ${GF2ASI_NATVIS})

#-----------------------------------------------------------------------------
# Precompiled headers
#-----------------------------------------------------------------------------
# One per target, force-included by the compiler, so no source file has to
# name it. Contents are the headers a target's own sources already pull in
# most, which is also where the parse time goes.
#
# Two ordering constraints are baked in here:
#
#  - MemUtils.h defines NOMINMAX before it includes Windows.h. Forcing it into
#    every SDK translation unit makes that uniform across the target, where
#    today it depends on which header a file happens to include first.
#
#  - GF2ASI's own sources do use the Windows min/max macros, so its header
#    includes <windows.h> directly and ahead of everything else, leaving them
#    defined exactly as they are today.
#
# Vendored sources are left out: imgui and the Discord SDK are third-party
# code with their own include order, and nothing is gained by forcing ours
# into them.

if(GF2ASI_USE_PCH)
    target_precompile_headers(GF2SDK PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_SOURCE_DIR}/Source/Platform/MemUtils.h>")

    target_precompile_headers(GF2Mod PRIVATE
        "${CMAKE_SOURCE_DIR}/Source/Addons/Hook.h"
        "${CMAKE_SOURCE_DIR}/Source/Addons/tConsole.h")

    foreach(_vendor_source ${GF2MOD_SOURCES})
        if(_vendor_source MATCHES "/Source/Addons/(imgui|discord)/")
            set_source_files_properties("${_vendor_source}" PROPERTIES
                SKIP_PRECOMPILE_HEADERS ON)
        endif()
    endforeach()

    target_precompile_headers(GF2ASI PRIVATE
        "<windows.h>"
        "<d3d9.h>"
        "<polyhook2/Detour/x86Detour.hpp>"
        "<polyhook2/ZydisDisassembler.hpp>"
        "${CMAKE_SOURCE_DIR}/Source/Addons/imgui/imgui.h")
endif()

#-----------------------------------------------------------------------------
# Output and runtime
#-----------------------------------------------------------------------------

set_target_properties(GF2ASI PROPERTIES
    OUTPUT_NAME "GF2ASI"
    SUFFIX ".asi"                       # loaded by an ASI loader, not as a DLL
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    VS_DEBUGGER_COMMAND "${GF2ASI_GAME_PATH}"
    VS_DEBUGGER_COMMAND_ARGUMENTS "${GF2ASI_DEBUGGER_ARGS}"

    # The game resolves relative paths against its own directory -- the ImGui
    # font is loaded as "scripts/Roboto-Medium.ttf", and settings and keybinds
    # likewise -- so the debugger has to start there, not in the build tree.
    VS_DEBUGGER_WORKING_DIRECTORY "${GF2ASI_GAME_DIR}"
)

# Static runtime on every target; a mismatch between a static library and the
# DLL that consumes it is a link error with an unhelpful message.
foreach(_t GF2SDK GF2Mod GF2ASI)
    set_property(TARGET ${_t} PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endforeach()

#-----------------------------------------------------------------------------
# Libraries
#-----------------------------------------------------------------------------
# Full paths rather than a name plus a search directory, so a missing vendor
# library fails at generate time instead of as a confusing link error.
# d3d9/d3dx9 come from Vendors/d3d9/libs, NOT a system DirectX SDK -- the legacy
# June 2010 SDK is not installed on build machines.
#
# Attached to GF2ASI: the static libraries do not link, so their undefined
# references are resolved once, here.

set(PH "${CMAKE_SOURCE_DIR}/Vendors/polyhook/libs")

target_link_libraries(GF2ASI PRIVATE
    "${CMAKE_SOURCE_DIR}/Vendors/detours/detours.lib"
    "${CMAKE_SOURCE_DIR}/Vendors/d3d9/libs/d3d9.lib"
    "${CMAKE_SOURCE_DIR}/Vendors/discord/libs/discord_game_sdk.dll.lib"

    "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/Vendors/d3d9/libs/d3dx9d.lib,${CMAKE_SOURCE_DIR}/Vendors/d3d9/libs/d3dx9.lib>"
    "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/Vendors/libcurl/debug/libcurl-d.lib,${CMAKE_SOURCE_DIR}/Vendors/libcurl/release/libcurl.lib>"
    "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/Vendors/lua/libs/lua_debug.lib,${CMAKE_SOURCE_DIR}/Vendors/lua/libs/lua_release.lib>"

    "${PH}/$<IF:$<CONFIG:Debug>,debug,release>/PolyHook_2.lib"
    "${PH}/$<IF:$<CONFIG:Debug>,debug,release>/asmjit.lib"
    "${PH}/$<IF:$<CONFIG:Debug>,debug,release>/capstone.lib"
    "${PH}/$<IF:$<CONFIG:Debug>,debug,release>/Zydis.lib"
    "${PH}/$<IF:$<CONFIG:Debug>,debug,release>/Zycore.lib"
)

#-----------------------------------------------------------------------------
# Deploy
#-----------------------------------------------------------------------------

if(_gf2_can_deploy)
    # discord_game_sdk.dll goes alongside the .asi because the Discord SDK is
    # linked through an import library, so the loader resolves it while loading
    # the .asi itself -- a missing copy fails the whole module, not just Discord.
    add_custom_command(TARGET GF2ASI POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${GF2ASI_GAME_DIR}/scripts"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "$<TARGET_FILE:GF2ASI>" "${GF2ASI_GAME_DIR}/scripts/"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${CMAKE_SOURCE_DIR}/Vendors/discord/libs/discord_game_sdk.dll"
                "${GF2ASI_GAME_DIR}/scripts/"
        COMMENT "Deploying $<TARGET_FILE_NAME:GF2ASI> to ${GF2ASI_GAME_DIR}/scripts"
        VERBATIM
    )
endif()

set_property(DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT GF2ASI)
