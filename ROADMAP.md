# Roadmap

What is left, and a defensible order to do it in. Every number here is
generated — see [Regenerating this](#regenerating-this) — and all of it moves,
so treat the figures as a snapshot and the *shape* as the point.

## The two stages

### Stage 1 — parity as an ASI

Each reconstructed class is a shell over one specific `godfather2.exe`. A method
either forwards to the original code at a fixed address or reimplements it, and
both work because the retail executable is the host process. That means
reconstruction is additive and never breaks what already runs.

**Done when** every function in the manifest is written and the engine runs on
our code, with the original binary acting only as the host.

### Stage 2 — a standalone executable

The same source, compiled to a program that does not need `godfather2.exe`.
Everything still reaching into the retail image has to go. Two numbers measure
it, and both must reach zero:

| | count |
|---|---|
| `MemUtils::Call*` sites in the SDK | **624** |
| Hardcoded address literals in the SDK | **908** |

This is why a forwarding thunk is progress for stage 1 but debt for stage 2.
Both are worth having — a thunk gets the surrounding code running and testable
today — but a function that forwards is not finished, and the count above is the
list of unfinished ones.

Stage 2 also needs things that are not reconstruction at all: an entry point,
real resource loading rather than riding the host's, and a decision on the
licensed middleware below.

## The size of the job

The manifest holds **5,890** original source files, but not all of them are a
target:

| | files |
|---|---|
| EA-authored engine and game code | 3,519 |
| Licensed middleware | 2,371 |

The middleware is Havok (`ears_rt_havok`, 1,461 files), RenderWare
(`rwaudiocore` 633, `rwmovie` 87, `rwcore` 23, `rwstdc` 19, `rwcodec` 10,
`rwfilesystem` 43, `rwcontroller` 11, `rwaudioprofile`, `rwtimer` and
`rwbigfile`) and EA's base
libraries (`eastl` 36, `eathread` 33, `eabase` 5, `coreallocator` 2).
Reconstructing those is almost certainly not the goal — for stage 2 you would
link a real implementation or a replacement. Two of them are partly mirrored
already (`rwfilesystem`, `rwcontroller`) because the game ships a customised
RenderWare build, so this is a judgement per package rather than a blanket rule.
**It has not been made yet, and it changes the denominator by 40%.**

## Where it stands

### Functions, by package

`missing` means the file exists and the function does not. `no-file` means the
file has not been created yet — so the two columns are "fill in" versus "start".

| package | done | missing | no-file | total | % |
|---|---|---|---|---|---|
| ears_statemachine | 37 | 40 | 2 | 79 | 46.8% |
| ears_locale | 6 | 74 | 100 | 180 | 3.3% |
| ears_godfather | 815 | 6,247 | 18,036 | 25,098 | 3.2% |
| ears_common | 14 | 122 | 304 | 440 | 3.2% |
| ears_framework | 214 | 1,455 | 6,640 | 8,309 | 2.6% |
| ears_trinity | 3 | 0 | 302 | 305 | 1.0% |
| ears_rt_cct | 6 | 149 | 599 | 754 | 0.8% |
| ears_rt_llrender | 1 | 51 | 830 | 882 | 0.1% |
| ears_physics | 1 | 149 | 807 | 957 | 0.1% |
| ears_alchemy | 0 | 0 | 2,867 | 2,867 | 0.0% |
| ears_audio | 0 | 0 | 705 | 705 | 0.0% |
| ears_online | 0 | 0 | 365 | 365 | 0.0% |
| ears_graph | 0 | 0 | 228 | 228 | 0.0% |
| ears_xmlrpc | 0 | 0 | 135 | 135 | 0.0% |
| ears_rt_math | 0 | 0 | 92 | 92 | 0.0% |
| ears_rt_callstack | 0 | 0 | 21 | 21 | 0.0% |
| rwfilesystem | 0 | 0 | 19 | 19 | 0.0% |
| **total** | **1,097** | **8,287** | **32,052** | **41,436** | **2.6%** |

### Files, in the twelve mirrored packages

| package | have | total | % |
|---|---|---|---|
| allocator | 1 | 1 | 100.0% |
| ears_statemachine | 4 | 9 | 44.4% |
| ears_common | 25 | 78 | 32.1% |
| ears_godfather | 329 | 1,791 | 18.4% |
| rwcontroller | 2 | 11 | 18.2% |
| ears_trinity | 5 | 36 | 13.9% |
| ears_rt_cct | 12 | 108 | 11.1% |
| ears_framework | 74 | 718 | 10.3% |
| ears_locale | 2 | 22 | 9.1% |
| ears_physics | 5 | 75 | 6.7% |
| rwfilesystem | 2 | 43 | 4.7% |
| ears_rt_llrender | 3 | 76 | 3.9% |
| **total** | **464** | **2,968** | **15.6%** |

### Classes

**3,479** classes appear in the linker maps. **222** have at least one function
written. **4** have nothing missing.

Separately there are **106** namespaces holding free functions, of which the
global namespace alone has **3,499**. These show up in the tooling with the
namespace in the class column — `EARS::Alchemy` with 579 "members" is 579 free
functions across many files, not a type.

### Where the thunks are

The stage-2 debt, by package:

| package | `MemUtils::Call*` sites | files |
|---|---|---|
| ears_godfather | 530 | 121 |
| ears_framework | 81 | 17 |
| ears_locale | 6 | 1 |
| ears_common | 3 | 1 |
| ears_statemachine | 2 | 2 |
| ears_physics | 1 | 1 |
| rwfilesystem | 1 | 1 |

## Suggested order

Nothing here is binding — but some of it is dependency, not preference.

### 1. Finish the foundations

Small, high-leverage, and almost thunk-free, so the work is real reimplementation
rather than debt:

- **`ears_statemachine`** — 9 files, 4 written, 46.8% of functions, 2 thunks.
  The only package in reach of complete. State machines drive NPC and player
  behaviour, so everything above it benefits.
- **`ears_common`** — 78 files, 32.1%, 3 thunks. The base library every other
  package includes: containers, hashing, linked lists, GUIDs. Much of it is
  templates in headers, which is why its function percentage (3.2%) understates
  what is there.

### 2. The entity hierarchy, in dependency order

The largest game classes are a single inheritance chain, which fixes the order.
Reconstructing a derived class against an incomplete base means guessing at what
the base already does:

```
Framework::Entity  (155 left,  15 done)
  └─ Framework::Animated  (142 left,  12 done)
       └─ PartedAnimated
            └─ Modules::Sentient  (550 left,   6 done)
                 ├─ Modules::NPC     (299 left,   2 done)
                 └─ Modules::Player  (337 left,   5 done)
```

`Sentient` is the single biggest class in the game at 556 functions, and both
`NPC` and `Player` derive from it. It is also the point where the project stops
being plumbing and starts being the game.

### 3. Small untouched packages, for onboarding

Self-contained, few dependencies, and a contributor can finish one:

| package | files | functions |
|---|---|---|
| ears_rt_callstack | 5 | 21 |
| ears_rt_math | 13 | 92 |
| ears_graph | 17 | 228 |
| ears_xmlrpc | 10 | 135 |
| ears_audio | 37 | 705 |

### 4. The long tail

Largest classes with nothing written yet:

| functions | class |
|---|---|
| 214 | EARS::Vehicles::WhiteboxCar |
| 156 | EARS::Vehicles::AICarBehavior |
| 154 | EARS::Apt::UIOnlineMenu |
| 142 | EARS::Modules::MultiplayerScenario |
| 138 | EARS::Modules::CommonCombatDatabase |
| 129 | EARS::Apt::UIPauseMap |
| 116 | EARS::SceneManagerInternal |
| 104 | EARS::Modules::NPCDebugDisplay |
| 97 | EARS::Apt::UIPauseMapSelectionManager |
| 96 | EARS::Alchemy::TFXSequencer |

Largest classes already started:

| left | done | class |
|---|---|---|
| 550 | 6 | EARS::Modules::Sentient |
| 337 | 5 | EARS::Modules::Player |
| 299 | 2 | EARS::Modules::NPC |
| 228 | 19 | EARS::Modules::Family |
| 203 | 2 | EARS::Modules::BuildingStore |
| 155 | 15 | EARS::Framework::Entity |
| 142 | 12 | EARS::Framework::Animated |
| 121 | 5 | EARS::Apt::UIHud |
| 119 | 4 | EARS::Modules::Item |
| 114 | 4 | EARS::Apt::UIFrontend |

Files with the most functions outstanding:

| left | done | file |
|---|---|---|
| 588 | 6 | ears_godfather/src/modules/sentient/sentient.cpp |
| 495 | 5 | ears_godfather/src/modules/player/player.cpp |
| 343 | 2 | ears_godfather/src/modules/npc/npc.cpp |
| 240 | 0 | ears_godfather/src/main/gameservices.cpp |
| 228 | 0 | ears_godfather/src/modules/vehicles/behaviors/whitebox_car/whitebox_car.cpp |
| 220 | 19 | ears_godfather/src/modules/families/family.cpp |
| 208 | 12 | ears_framework/src/framework/core/animated/animated.cpp |
| 205 | 15 | ears_framework/src/framework/core/entity/entity.cpp |
| 193 | 3 | ears_godfather/src/modules/buildings/building_store.cpp |
| 141 | 12 | ears_framework/src/framework/core/streammanager/streammanager.cpp |

## Regenerating this

The exhaustive list — every one of the 41,436 functions with its class, file,
object, mangled name and Xbox address — is generated, not committed. It is
~41k rows, and committing it would bury the signal in diff noise.

```bash
python Tools/sdk_coverage.py                       # files, per package
python Tools/sdk_coverage.py --package ears_godfather
python Tools/function_coverage.py --summary        # functions, per package
python Tools/function_coverage.py > coverage.csv   # every row
python Tools/function_coverage.py --file modules/families/family.cpp --summary
```

Both read ground truth from the Xbox 360 build output at
`J:\Projects\RE Material\The Godfather II` — 9 PDBs and 4 linker maps. Pass
`--re-material` / `--map` if yours is elsewhere.

## Reading the numbers honestly

Five things will mislead you otherwise. All five are properties of the measuring,
not of the code.

**Header-inline functions are attributed to the wrong file.** The original
emitted them into whichever object used them, not into their own file's object.
`npc.cpp` reads 2 of 345 largely because most of what we have is inline in
`npc.h`. Per-file totals understate header-heavy types, and a function can read
as missing while sitting in the header — `StateTable::IsReferenced`,
`ClearRefCount` and `DecRefCount` all do.

**Addresses are Xbox 360** (`0x82xxxxxx`). The PC build this project targets has
different ones. Use them to locate the PC equivalent, never to paste into a
thunk.

**A class can be counted against another package.** It is attributed to the
object it was compiled into, so `EARS::Framework::FastPoolGroupIAllocator`
appears under `ears_statemachine` because it landed in `statemachinemanager.obj`.
Seven of that package's forty "missing" functions are this.

**Namespace-scope free functions appear as classes**, with the namespace in the
class column. See above.

**Matching is case-insensitive and ignores types.** It keys on
`(class, function)`, so our parameter or return types differing from the
original does not register as missing — and the original's
`NPC::ActivateHudIndicator` matches our `ActivateHUDIndicator`.
`function_coverage.py --summary` lists every casing divergence so they can be
aligned.

Templates, vtables, RTTI, string literals and compiler-generated destructors —
around 31,000 symbols — are counted separately and excluded from all of the
above.
