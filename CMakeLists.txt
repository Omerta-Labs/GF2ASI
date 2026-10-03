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

set(GF2ASI_GAME_DIR "J:/Games/The Godfather II" CACHE PATH
    "Game install. The built .asi is copied into <dir>/scripts after linking, "
    "and this is where the Visual Studio debugger launches from.")

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
# Output and runtime
#-----------------------------------------------------------------------------

set_target_properties(GF2ASI PROPERTIES
    OUTPUT_NAME "GF2ASI"
    SUFFIX ".asi"                       # loaded by an ASI loader, not as a DLL
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    VS_DEBUGGER_COMMAND "${GF2ASI_GAME_DIR}/godfather2.exe"
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

if(EXISTS "${GF2ASI_GAME_DIR}")
    add_custom_command(TARGET GF2ASI POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${GF2ASI_GAME_DIR}/scripts"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "$<TARGET_FILE:GF2ASI>" "${GF2ASI_GAME_DIR}/scripts/"
        COMMENT "Deploying $<TARGET_FILE_NAME:GF2ASI> to ${GF2ASI_GAME_DIR}/scripts"
        VERBATIM
    )
else()
    message(STATUS
        "GF2ASI_GAME_DIR does not exist (${GF2ASI_GAME_DIR}); skipping deploy. "
        "Set -DGF2ASI_GAME_DIR=<path> to enable it.")
endif()

set_property(DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT GF2ASI)

list(LENGTH GF2SDK_SOURCES _n_sdk)
list(LENGTH GF2MOD_SOURCES _n_mod)
list(LENGTH GF2ASI_SOURCES _n_asi)
math(EXPR _n_asi "${_n_asi} + 5")       # dllmain, GF2Hook, HookMods, ScripthookEvents
message(STATUS "GF2ASI: ${_n_sdk} SDK + ${_n_mod} Mod + ${_n_asi} ASI "
               "translation units (262 total -- see Reference/build_baseline.md)")
