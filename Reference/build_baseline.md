# Build baseline (pre-restructure)

Captured at Phase 0 so the Phase 1 CMake port can be checked for producing the
same thing. Recorded from the premake-generated `GF2ASI.vcxproj`; `premake5.lua`
is the only tracked build file, and the `.sln`/`.vcxproj` are gitignored output.

Translation units are listed in `build_baseline_files.txt` (269 of them).
Headers are not listed — they are not compiled, and premake globs them purely so
they show up in the IDE.

The committed `.vcxproj` lists only 259, because premake had not been re-run
since the shadow and lighting work was added (`Core/Graphics/*`,
`Modules/Lights/*`, `Modules/TimeOfDay/EARSLightTODMessages.cpp`). Since premake
globs `**.cpp`, the real baseline is whatever is on disk, so this file is
generated from the glob rather than read out of the stale project file. CMake's
`CONFIGURE_DEPENDS` picks new files up without a manual regenerate, which is
what went wrong here.

## Configuration

Since Phase 3c the build is three targets rather than one: `GF2ASI` (shared)
depends on `GF2Mod` (static), which depends on `GF2SDK` (static). Settings are
shared through a `gf2_settings` interface library so the three cannot drift
apart. Everything below describes settings that still apply to all of them.

| | |
|---|---|
| Output | `GF2ASI.asi` (DynamicLibrary, `TargetExt .asi`) |
| Platform | Win32 / x86 only |
| Toolset | v143 |
| Language | `stdcpplatest` (premake asks for C++20) |
| Character set | MultiByte (MBCS) |
| Runtime | `/MT` Release, `/MTd` Debug (`staticruntime on`) |
| PCH | none (`PrecompiledHeader NotUsing`) |
| Defines | `NDEBUG` Release, `DEBUG` Debug |
| Deploy target | `J:\Games\The Godfather II\scripts` |

Note `DEBUG`/`NDEBUG` rather than `_DEBUG`: source tests `#if DEBUG`, so the
CMake port has to define `DEBUG=1` and not rely on MSVC's own `_DEBUG`.

## Include directories

```
$(ProjectDir)
$(ProjectDir)Source
$(ProjectDir)Source/Packages      (does not exist; empty in premake too)
vendors/d3d9/include
vendors/detours
vendors/polyhook/include
vendors/libcurl/include
vendors/lua/include
vendors/sol
```

Phase 2e added the 17 per-package SDK roots on top of these, so an SDK include
directive reads as it did in the original. The repo root and `Source/` stay, for
`Addons/`, `Scripthook/`, `Utils/` and `framework.h`; no SDK include depends on
them any more.

## Translation unit count over the restructure

| | TUs |
|---|---|
| Phase 0 baseline | 269 |
| after 2d-ii (12 merged away, 3 new) | 260 |
| after 2d-iii (3 inlined into headers, 1 merged) | 257 |
| after 3a/3b (Platform, SH_SDKHooks; scorekeeper.cpp removed) | 262 |
| after 4a (ImGuiPanels.cpp) | 263 |
| after 4b (Addons/ImGuiRuntime.cpp) | 264 |

Split across targets: 214 GF2SDK, 35 GF2Mod, 13 GF2ASI.

A drop is expected at each merge and is the point of it; an *increase* would
mean a file was duplicated rather than moved.

## Link libraries

Config-independent: `detours.lib`, `d3d9.lib`, `d3dx9.lib` (`d3dx9d.lib` in
Debug — note these come from `vendors/d3d9/libs`, not a system DirectX SDK).

Per config, from `vendors/polyhook/libs/{debug,release}`: `asmjit`, `capstone`,
`PolyHook_2`, `Zycore`, `Zydis`; plus
`vendors/discord/libs/discord_game_sdk.dll.lib`,
`vendors/libcurl/{debug,release}/libcurl{-d,}.lib`,
`vendors/lua/libs/lua_{debug,release}.lib`.

Library search paths: `vendors/detours`, `vendors/d3d9/libs`.

## Verification step for Phase 1

The repo rule is that builds are done in Visual Studio, not from here, so the
binary half of this baseline needs one manual pass:

1. Build Release from the current premake-generated solution and record the
   `.asi` size and the `/VERBOSE:LIB` link output.
2. Build Release from the CMake-generated solution.
3. Compare the translation-unit list against `build_baseline_files.txt`, the
   resolved library set, and the `.asi` size. Sizes will not match to the byte —
   link order and `__FILE__` paths both shift — but a difference of more than a
   few percent means something was dropped or added.
4. Load the game and confirm the menu opens, a level loads, and the fixes apply.

The last-known-good artefact for reference is the Release `.asi` deployed to
`J:\Games\The Godfather II\scripts` (6,797,824 bytes, 25 Sep 2025). It predates
the current working tree, so it is a sanity check on magnitude, not a
byte-for-byte target.
