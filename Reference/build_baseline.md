# Build baseline (pre-restructure)

Captured at Phase 0 so the Phase 1 CMake port can be checked for producing the
same thing. Recorded from the premake-generated `GF2ASI.vcxproj`; `premake5.lua`
is the only tracked build file, and the `.sln`/`.vcxproj` are gitignored output.

Translation units are listed in `build_baseline_files.txt` (259 of them).
Headers are not listed — they are not compiled, and premake globs them purely so
they show up in the IDE.

## Configuration

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

`Source/` as a single include root is what keeps Phase 3's target split free of
include rewrites. Phase 2e replaces it with per-package roots.

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
