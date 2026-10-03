# GF2ASI Restructure Plan

Target: three build targets, SDK folder structure matching the original EARS source tree,
and a modding layer the SDK can call into without depending on it.

Ground truth for all structural decisions is the Xbox 360 build output at
`J:\Projects\RE Material\The Godfather II\ears_godfather_singleplayer` — 5 PDBs (full original
source paths) and 4 linker `.map` files (class -> original object file). We rebuild against
`ears_godfather_d_mt`.

---

## 1. Target architecture

```
GF2ASI.asi  (SharedLib)   dllmain, GF2Hook, HookMods, menu panels
     |
GF2Mod      (StaticLib)   logging, settings, ImGui runtime, keybinds,
     |                    mod manager, discord, hashed names, game fixes
GF2SDK      (StaticLib)   original game code + Platform (MemUtils, diagnostics sinks)
```

### Why MemUtils stays in the SDK, not GF2Mod

`GF2Core` merges up into `GF2Mod` as planned — **except `Addons/Hook.h`**. 158 of 491 SDK files
call `MemUtils::CallClassMethod` / `hook::Type` to reach the ~900 hardcoded addresses the
reconstructed classes are built on. Calling fixed addresses *is* what the SDK does, so MemUtils is
an SDK primitive, not mod infrastructure. It becomes `SDK/Platform/MemUtils.h`.

Everything else in `Addons/` (tLog, tConsole, Settings) moves into `GF2Mod`.

### How the SDK reaches logging and 3D debug

Dependency inversion, as you described. The SDK declares the API and a sink table; `GF2Mod` fills
the table in at init. Default is a no-op, so `GF2SDK` links and runs standalone.

`SDK/Platform/Diagnostics.h`:

```cpp
namespace EARS::Diag
{
    // Installed by GF2Mod at startup; all no-ops until then.
    struct Sinks
    {
        void (*Printf)(const char* Format, ...);
        void (*DrawLine)(const RwV3d& A, const RwV3d& B, uint32_t Colour);
        void (*DrawSphere)(const RwV3d& Centre, float Radius, uint32_t Colour);
        void (*DrawText3D)(const RwV3d& Position, const char* Text, uint32_t Colour);
    };

    void InstallSinks(const Sinks& InSinks);

    void Printf(const char* Format, ...);
    void DrawLine(const RwV3d& A, const RwV3d& B, uint32_t Colour);
    void DrawSphere(const RwV3d& Centre, float Radius, uint32_t Colour);
    void DrawText3D(const RwV3d& Position, const char* Text, uint32_t Colour);
}
```

Three SDK files call `tConsole::fPrintf` today and switch to `EARS::Diag::Printf`:
`SimManager.cpp`, `StreamManager.cpp`, `ShaderManager.cpp`.

### Game fixes live in GF2Mod

Confirmed by dependency analysis: `SH_GammaFix`, `SH_EdgeAA`, `SH_VSyncFix` and `SH_MugShotFix`
include only `Hook.h`, `Settings.h`, `tConsole.h` and SDK headers. **None of them touch ImGui**, so
they create no cycle with the menu. ~800 lines across 4 modules — not worth its own target.

### Include roots: directives match the original source

Because file names are exact-lowercase (D1), each package root goes on the include path, so a
directive transcribed out of original code compiles as written:

| Include root | Directive form |
|---|---|
| `SDK/ears_godfather/src/` | `#include "modules/families/family.h"` |
| `SDK/ears_framework/src/` + `SDK/ears_framework/src/game_framework/` | `#include "framework/core/eventhandler/ceventhandler.h"` |
| `SDK/ears_common/include/` | `#include "ears_common/string.h"` |
| `SDK/ears_physics/src/` | `#include "ears_physics/characters/characterproxy.h"` |
| `SDK/ears_rt_cct/include/` | `#include "ears_rt_cct/chrcntl_character.h"` |
| `SDK/ears_rt_llrender/include/` | `#include "ears_rt_llrender/shadermanager.h"` |
| `SDK/rwfilesystem/include/` | `#include "rw/core/filesys/manager.h"` |
| `SDK/rwcontroller/include/` | `#include "rw/core/controller/manager.h"` |

That last pair is corroborated by the code already using `rw::core::filesys::Manager` — the
namespace mirrors the include path, which is how RenderWare packages were laid out.

Cost of this versus a single `Source/` root: ~12 `-I` flags instead of 1, and generic top-level
prefixes (`modules/`, `core/`, `main/`, `system/`) become visible to every target, so a future name
clash is possible. Worth it for a reconstruction project — it removes a translation step on every
file transcribed from the original.

### ImGui splits across the GF2Mod / GF2ASI line

`GF2Mod` owns the ImGui **runtime**: context creation, DX9/Win32 backends, device lost/restored,
the draw-data snapshot, WndProc routing, `KeybindManager`, and a panel-registration API.
`GF2ASI` owns the **content**: the 15 `DrawTab_*` panels, NPC inspector, photo mode, checkpoint
debug. This is what lets "ImGui in the modding layer" and "menu merged into the ASI" both hold.

---

## 2. Phases

### Phase 0 — Manifest and safety net
*Prereq for everything. ~2 h.*

1. Commit the recovered original tree as reference material: `Reference/original_tree.txt`
   (6,488 paths across the EARS/RW packages).
2. Commit the three extraction scripts under `Tools/`:
   - `extract_pdb_paths.sh` — PDB -> source path list
   - `map_symbol_to_obj.sh` — class name -> original `.obj` (via the `.map`)
   - `sdk_coverage.py` — project tree vs manifest, prints coverage + misplacements
3. Record the current build: file list, link order, and `.asi` size, so Phase 1 can be verified
   as output-identical.

**Done when:** `sdk_coverage.py` runs and reproduces the audit numbers (370 matched / 121 to triage).

### Phase 1 — CMake port, single target
*~3 h. No source changes.*

`premake5.lua` is the only tracked build file; `.sln`/`.vcxproj` are gitignored artifacts, so there
is no hand-maintained project file to migrate and no filter tree to rebuild.

- One `CMakeLists.txt` producing `GF2ASI.asi`, same settings: `-A Win32`, v143, `/MT`,
  C++20/latest, MBCS, `.asi` suffix, no PCH yet.
- Vendor libs via `$<CONFIG>` generator expressions instead of duplicated Debug/Release blocks.
- Keep `Source/` as the single include root — this is what makes later phases cheap.
- Delete `premake5.lua` and `premake5.exe`; keep `.vcxproj` gitignored (CMake generates it).

**Done when:** the generated solution builds and the `.asi` loads and plays identically.
**Risk:** low. Rollback is `git revert`.

### Phase 2 — SDK restructure to original layout
*The big one. ~2–3 days, mostly scripted.*

Every include is already rooted at `Source/`, all 266 SDK headers use `#pragma once` (no guard
renaming), and premake/CMake globs the file list — so this is a rename + include-rewrite, nothing more.

**2a. Move the 15 verified-wrong paths** (13 wrong directory, 2 wrong package):

| Current | Original |
|---|---|
| `EARS_Framework/Toolkits/GroupManager/` | `toolkits/group/` |
| `EARS_Godfather/Modules/Components/` | `modules/component/` (singular) |
| `EARS_Godfather/Modules/Vehicles/Behaviours/` | `modules/vehicles/behaviors/` (US spelling) |
| `.../Behaviours/WhiteboxCar/` | `.../behaviors/whitebox_car/` |
| `EARS_Physics/Behaviours/` | `src/ears_physics/behaviors/` |
| `EARS_Common/IAllocator.h` | package `allocator`, `include/allocator/iallocator.h` |
| `EARS_Godfather/System/Memory/GlobalHeapAllocator.h` | `ears_framework/.../core/memory/` |

You currently have **both** `Modules/Vehicles/Behaviors/` and `.../Behaviours/`. Original is `behaviors`.

**2b. Adopt the original package roots.** Drop the version level (`dev`, `release`, `2.0`) — it is a
build-system artifact. Per-package `include/` vs `src/` is not guessable, so it comes straight from
the manifest:

| Package | Original layout |
|---|---|
| `ears_godfather` | `src/{modules,core,main,system,render}` — **no include dir** |
| `ears_framework` | `src/framework/{core,modules,toolkits,mainloop}` — no include dir |
| `ears_physics` | `src/ears_physics/` — no include dir |
| `ears_common` | `include/ears_common/` + `src/` |
| `ears_rt_cct` | `include/ears_rt_cct/` + `src/` (your `.cpp`s are wrongly in `include/`) |
| `ears_rt_llrender` | `include/ears_rt_llrender/` + `src/` |
| `ears_statemachine`, `ears_locale`, `ears_trinity`, `ears_apt` | `include/<pkg>/` + `src/` |
| `rwfilesystem` | `include/rw/core/filesys/` + `source/` |
| `rwcontroller` | `include/rw/core/controller/` + `source/` |

`ears_godfather` had no static lib at all — its 840 objects link straight into the exe. Keeping it
as an SDK subfolder rather than its own target matches that.

Package directories go lowercase too, so `Source/SDK/EARS_Godfather/` becomes
`Source/SDK/ears_godfather/`.

**2b-ii. `ears_framework` has two source roots,** `src/framework/` and
`src/game_framework/framework/`. They hold *disjoint* file sets (zero overlap) but share the
`framework/...` logical prefix, and both compile into the same `ears_framework_d_mt` lib. The
manifest resolves every file unambiguously; of the 65 that match, **11 belong to
`src/game_framework/`**:

```
Core/AttributeHandler/CAttributeHandler.{cpp,h}   -> src/game_framework/framework/core/attributehandler/
Core/AttributeHandler/CClassFactory.h             -> src/game_framework/framework/core/attributehandler/
Core/EventHandler/CEventHandler.{cpp,h}           -> src/game_framework/framework/core/eventhandler/
Core/ResourceManager/CResourceHandler.{cpp,h}     -> src/game_framework/framework/core/resourcemanager/
Core/ResourceManager/CResourceManager.{cpp,h}     -> src/game_framework/framework/core/resourcemanager/
MainLoop/Logic.{cpp,h}                            -> src/game_framework/framework/mainloop/
```

The other 54 go to `src/framework/`; 9 fall through to the `.map` triage in 2d.

**2c. Rename the 370 clean matches to exact original lowercase** (decision D1). All 370 already
differ from their original only by case, so this is purely mechanical. Case-only renames on Windows
need `git mv -f` with `core.ignorecase=false`, done as one commit so git records them as renames.

**2d. Triage the 121 leftovers via the `.map`, not fuzzy matching.** Fuzzy name matching is
unusable here (it proposed `Bitflags.cpp -> printf.cpp`). Instead, grep the class's mangled name in
`ears_godfather_d_mt.map`; the object file it lands in *is* the original source file:

```
AmbushSM         -> npcsearchsm.obj      -> modules/npc/statemachines/npcsearchsm.cpp
BuildingManager  -> building_manager.obj -> modules/buildings/building_manager.cpp
RwMaths          -> (no symbols)         -> project-owned, keep as-is
```

**2d-ii. Merge files that shared one original object** (decision D2). `DrawGunSM`, `HolsterGunSM`
and `ReloadGunSM` all resolve to `npcguncombatsm.obj` — one original file, not three. Expect ~15–25
such merges. Procedure per merge, so review stays tractable:

1. Resolve every class in the group to its object via the `.map`, and confirm they agree.
2. Concatenate into the original file name, headers first, preserving each class's existing comments.
3. Keep declaration order as the vtable/symbol order in the `.map` suggests, rather than
   alphabetical — it is free evidence about the original file.
4. Delete the old files in the *same* commit as the merge, one commit per merged group.

This is the one part of Phase 2 that is a real semantic change rather than a rename, so it is also
the one part worth reviewing by hand.

Known from the audit: CCT is `chrcntl_*` (no `r` after `Cnt`); godfather UI is `ui_*`
(`ui_frontend.cpp`, `ui_hud.cpp`, `ui_popup.cpp`); managers use underscores
(`building_manager.cpp`, `family_manager.cpp`, `corleone_data.cpp`); NPC state machines are
abbreviated (`apprchtargetsm.cpp`, `tracktoposanddirsm.cpp`, `npcmeleesupporter.cpp`);
graphics uses `godfather_*.cpp`.

Genuinely project-owned, no original counterpart — leave alone, but mark them so coverage reports
don't flag them: `RwMaths`, `Bitflags`, `Bitmask`, `SMBuilder`, `PersistenceRegistry`,
plus the new `Core/Graphics` and `Modules/Lights` work.

**2e. Rewrite the 549 include directives** across 280 files from the rename map, generated — never
hand-edited. Add the per-package include roots from §1 at the same time, so directives drop the
`SDK/<pkg>/src/` prefix and take their original form. Then one sweep to confirm nothing resolves by
luck: build once with the old `Source/` root *removed* from the include path, so any directive that
was not rewritten fails loudly rather than silently resolving.

**Done when:** build is green, `.asi` behaviour unchanged, and `sdk_coverage.py` reports 0 misplaced.
**Risk:** medium — it is a large diff, but every step is generated from the manifest and verified by
a compile. Do 2a–2e as five separate commits so any one can be reverted.

### Phase 3 — Split into three targets
*~1 day.*

1. Create `GF2SDK`, `GF2Mod`, `GF2ASI` targets; assign files by directory. No file moves — every
   target gets `Source/` on its include path, so includes are untouched.
2. Add `SDK/Platform/MemUtils.h` (moved from `Addons/Hook.h`) and `SDK/Platform/Diagnostics.h`.
3. Switch the 3 SDK `tConsole` callers to `EARS::Diag::Printf`; install the sinks from
   `GF2Hook::Init_Logging`.
4. Move `SimManager::InitialiseScripthookModLoader` out of the SDK into `GF2Mod`.
5. Move `ScoreKeeper::StaticApplyHooks` and `DemographicRegion::StaticApplyHooks` out of the SDK —
   they are the only 2 SDK files pulling in polyhook.
6. Move `MarketingDebug.cpp` into `GF2ASI` (it reads `ImGuiManager::HasCursorControl`).

All hooks are installed by explicit calls from `GF2Hook::Init_AttachHooks()` — there is no
self-registration anywhere — so static libs will not silently drop registration objects.

**Done when:** `GF2SDK` compiles with no polyhook, no ImGui, no Settings on its include path.

### Phase 4 — Split ImGuiManager
*~1 day.*

`ImGuiManager.cpp` is 1,796 lines: lines 565–1582 are 15 `DrawTab_*` members, the rest is the D3D9
runtime. Split along the GF2Mod / GF2ASI line:

- `GF2Mod`: `ImGuiRuntime` — context, backends, `OnDeviceLost/Restored`, draw snapshot, WndProc,
  cursor suppression, `KeybindManager`, plus a panel API (`RegisterPanel(name, callback)`).
- `GF2ASI`: one file per panel group, registered at startup.

The tabs are members touching private state, so this needs a panel context struct rather than a
straight cut.

### Phase 5 — Decompose Settings
*~0.5 day.*

`Settings.h` defines `EdgeAATuning` and `MugShotTuning`, so the mod core currently knows every fix.
Replace with a generic INI reader in `GF2Mod` plus each fix owning and registering its own tuning
struct. Also adds a PCH per target here — that will cut build time more than the target split does.

### Phase 6 — Build out the remaining code files
*Ongoing.*

With the structure matching, this becomes a set operation. `ears_godfather` alone is 1,790 original
files against 352 reconstructed (~20%). `sdk_coverage.py` prints the gap, and each new file has a
known exact path and name before a line is written — the 80 canonical module directories are
`actionablesurfaces/` through `zone/`.

Packages not mirrored at all yet, with original file counts: `ears_alchemy` (175),
`ears_apt` (67), `ears_audio` (37), `ears_graph` (17), `ears_online` (34), `ears_rt_math` (13),
`tnl` (88), `job_manager` (78).

---

## 3. Decisions taken

**D1 — Exact original lowercase file and directory names.** `ui_frontend.cpp`,
`building_manager.cpp`, `chrcntl_character.h`, `Source/SDK/ears_godfather/`. The PDB manifest
becomes a drop-in index and include directives match the original source character for character.

**D2 — Merge files to match the original object layout.** Where the `.map` puts several classes in
one object, they become one file. Truest to the original and keeps coverage a clean set operation.

Note these two decisions interact with Phase 6 in a useful way: a new file's path, name, *and*
include directives are all fully determined before a line is written.

---

## 4. Estimate

| Phase | Work | Est. |
|---|---|---|
| 0 | Manifest + scripts + baseline | 2 h |
| 1 | CMake, single target | 3 h |
| 2 | SDK restructure (2a–2e, incl. ~15–25 hand-reviewed merges) | 3–4 d |
| 3 | Three-target split + diagnostics inversion | 1 d |
| 4 | ImGuiManager split | 1 d |
| 5 | Settings decomposition + PCH | 0.5 d |
| **Total (0–5)** | | **~7 days** |
| 6 | Reconstruction build-out | ongoing |

Phases 0, 1 and 3 are low risk. Phase 2 carries the whole risk of the plan: it is a large diff, but
2a–2c and 2e are generated from the manifest and verified by a compile, leaving only 2d-ii (the
merges) as genuine hand work. Commit boundaries are per sub-step, so any one is revertible.

## 5. Commit sequence

```
P0  reference: original source tree manifest + extraction tools
P1  build: port to CMake, single target
P2a sdk: fix 15 verified-wrong paths
P2b sdk: adopt original package roots (include/ vs src/)
P2c sdk: rename to exact original lowercase
P2d sdk: resolve 121 unmatched names via linker map
P2d2 sdk: merge files that shared one original object   <- hand-reviewed
P2e sdk: rewrite includes, add per-package include roots
P3a sdk: add Platform/MemUtils.h + Platform/Diagnostics.h
P3b sdk: move mod-layer code out of SDK (modloader, 2 hook installers, MarketingDebug)
P3c build: split into GF2SDK / GF2Mod / GF2ASI
P4  mod: split ImGui runtime from menu panels
P5  mod: decompose Settings; add per-target PCH
```
