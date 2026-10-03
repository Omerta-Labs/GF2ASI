#=============================================================================
# GF2ASI -- The Godfather II script hook
#
# Replaces premake5.lua. Phase 1 of the restructure: same single target, same
# compiler and linker settings, no source changes. Reference/build_baseline.md
# records what this has to reproduce.
#
# Generate (32-bit is mandatory -- the game is a 32-bit process):
#
#   cmake -S . -B build -G "Visual Studio 17 2022" -A Win32
#
# Then open build/GF2ASI.sln, or build from the command line:
#
#   cmake --build build --config Release
#
# Phase 3 splits this into GF2SDK / GF2Mod / GF2ASI. Until then the single
# Source/ include root is what keeps that split free of include rewrites.
#=============================================================================

cmake_minimum_required(VERSION 3.21)

# MSVC_RUNTIME_LIBRARY instead of hand-patching /MT into the flags.
cmake_policy(SET CMP0091 NEW)

project(GF2ASI LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

#-----------------------------------------------------------------------------
# Toolchain requirements
#-----------------------------------------------------------------------------

if(NOT MSVC)
    message(FATAL_ERROR
        "GF2ASI builds with MSVC only. Source/SDK/Platform uses __asm for the "
        "thunks that pass arguments in registers no calling convention can "
        "express, which clang-cl does not support in 32-bit mode.")
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
# Sources
#-----------------------------------------------------------------------------
# Globbed, as premake did. CONFIGURE_DEPENDS re-runs the glob when the set of
# files changes, which matters a great deal for Phase 2: the SDK restructure
# renames roughly 500 files and nothing here should need touching.
#
# Headers are listed only so they appear in the IDE; they are not compiled.

file(GLOB_RECURSE GF2ASI_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/Source/*.cpp"
    "${CMAKE_SOURCE_DIR}/Source/*.c"
)
file(GLOB_RECURSE GF2ASI_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/Source/*.h"
    "${CMAKE_SOURCE_DIR}/Source/*.hpp"
    "${CMAKE_SOURCE_DIR}/Source/*.inl"
)

# Vendored imgui and the Discord SDK live under Source/Addons and are part of
# the build; everything under Vendors/ is prebuilt and must not be compiled.
list(FILTER GF2ASI_SOURCES EXCLUDE REGEX "/Vendors/")
list(FILTER GF2ASI_HEADERS EXCLUDE REGEX "/Vendors/")

# Natvis visualisers for the SDK container types. premake never picked these up,
# so they have been sitting unused; MSVC loads them from the project.
file(GLOB GF2ASI_NATVIS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Natvis/*.natvis")

add_library(GF2ASI SHARED
    "${CMAKE_SOURCE_DIR}/dllmain.cpp"
    "${CMAKE_SOURCE_DIR}/framework.h"
    ${GF2ASI_SOURCES}
    ${GF2ASI_HEADERS}
    ${GF2ASI_NATVIS}
)

# Mirror the tree in the IDE rather than dumping 500 files into one folder.
source_group(TREE "${CMAKE_SOURCE_DIR}" FILES
    ${GF2ASI_SOURCES} ${GF2ASI_HEADERS} ${GF2ASI_NATVIS})

#-----------------------------------------------------------------------------
# Output
#-----------------------------------------------------------------------------

set_target_properties(GF2ASI PROPERTIES
    OUTPUT_NAME "GF2ASI"
    SUFFIX ".asi"                       # loaded by an ASI loader, not as a DLL
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    VS_DEBUGGER_COMMAND "${GF2ASI_GAME_DIR}/godfather2.exe"
    VS_DEBUGGER_WORKING_DIRECTORY "${GF2ASI_GAME_DIR}"
)

#-----------------------------------------------------------------------------
# Compiler
#-----------------------------------------------------------------------------

target_compile_options(GF2ASI PRIVATE
    /std:c++latest      # what premake's cppdialect "C++20" actually emitted
    /MP                 # premake flags("MultiProcessorCompile")
)

# Deliberately NOT adding /permissive-, /W4 or /WX here. Phase 1 reproduces the
# old build; tightening conformance would surface new errors in SDK code and
# muddle "the port broke it" with "the port exposed it". Worth doing per target
# once Phase 3 has split them.

target_compile_definitions(GF2ASI PRIVATE
    # MBCS, not Unicode: the SDK's string types and the Win32 calls around them
    # are all narrow. CMake does not define UNICODE itself, so this only has to
    # assert the positive.
    _MBCS

    # DEBUG, not _DEBUG. Source tests `#if DEBUG` -- see HookMods.cpp's
    # IMPLEMENT_* switches -- so relying on MSVC's own _DEBUG would silently
    # compile out every debug-only path.
    $<$<CONFIG:Debug>:DEBUG=1>
    $<$<CONFIG:Release>:NDEBUG>
)

#-----------------------------------------------------------------------------
# Includes
#-----------------------------------------------------------------------------
# $(ProjectDir)Source/Packages from the premake file is dropped: it does not
# exist and never did.

target_include_directories(GF2ASI PRIVATE
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/Source"
)

target_include_directories(GF2ASI SYSTEM PRIVATE
    "${CMAKE_SOURCE_DIR}/Vendors/d3d9/include"
    "${CMAKE_SOURCE_DIR}/Vendors/detours"
    "${CMAKE_SOURCE_DIR}/Vendors/polyhook/include"
    "${CMAKE_SOURCE_DIR}/Vendors/libcurl/include"
    "${CMAKE_SOURCE_DIR}/Vendors/lua/include"
    "${CMAKE_SOURCE_DIR}/Vendors/sol"
)

#-----------------------------------------------------------------------------
# Libraries
#-----------------------------------------------------------------------------
# Full paths rather than a name plus a search directory, so a missing vendor
# library fails at generate time instead of producing a confusing link error.
# d3d9/d3dx9 come from Vendors/d3d9/libs, NOT a system DirectX SDK -- the legacy
# June 2010 SDK is not installed on build machines.

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
# premake wrote the .asi straight into the game's scripts folder. Building to
# bin/ and copying afterwards keeps the build working on a machine that has no
# game install, which the old setup did not.

if(EXISTS "${GF2ASI_GAME_DIR}")
    add_custom_command(TARGET GF2ASI POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory
                "${GF2ASI_GAME_DIR}/scripts"
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

list(LENGTH GF2ASI_SOURCES GF2ASI_TU_COUNT)
math(EXPR GF2ASI_TU_COUNT "${GF2ASI_TU_COUNT} + 1")   # dllmain.cpp
message(STATUS "GF2ASI: ${GF2ASI_TU_COUNT} translation units "
               "(baseline is 269 -- see Reference/build_baseline.md)")
