# GF2ASI

A recompilation of **The Godfather II** (PC, Steam build), built up from the
original EARS engine source recovered from the shipped debug symbols.

Three things live in this repository, as three layers that depend only
downwards:

- **GF2SDK** — the reconstructed game and engine code, in the original package
  and file layout.
- **GF2Mod** — the modding layer: logging, configuration, ImGui, the mod
  loader, the hashed-name registry, and the game fixes.
- **GF2ASI** — the script hook itself: start-up, the detours that get our code
  running inside the retail executable, and the debug menu.

## The goal, in two stages

**Stage 1 — parity as an ASI.** Today each reconstructed class is a shell over
one specific `godfather2.exe`: its methods either call the original code at a
fixed address or reimplement it outright, and both work because the retail
executable is the process we are loaded into. This stage is finished when every
function in the manifest is written and the engine runs on our code with the
original binary acting only as the host.

**Stage 2 — a standalone executable.** The same source, compiled to a program
that does not need `godfather2.exe` at all. Everything that still reaches into
the retail image has to go: today that is **624** `MemUtils::Call*` sites and
**908** hardcoded address literals in the SDK. Those two numbers reaching zero
is the real measure of stage 2, and they are the reason a function that
forwards to the original is progress for stage 1 but debt for stage 2.

Alongside both, and the reason the project is usable today: a layer of **fixes**
for bugs in the shipped game, and **mod support** so other people's changes do
not have to be patches to a binary.

## Where it stands

| | done | total | |
|---|---|---|---|
| Files, in mirrored packages | 464 | 2,968 | 15.6% |
| Functions | 1,097 | 39,775 | 2.8% |
| Classes with anything written | 222 | 3,479 | |
| Classes with nothing missing | 4 | 3,479 | |

The function number is the honest one — a file existing says very little about
how much of it is there. [ROADMAP.md](ROADMAP.md) breaks the gap down by
package, file and class, and explains how to regenerate all of it.

## Building

32-bit MSVC only. `godfather2.exe` is a 32-bit process and the SDK is full of
hardcoded addresses into its image, so CMake fails the configure if you point it
at a 64-bit generator.

```bash
cmake --preset vs2026        # or vs2022; see CMakePresets.json
cmake --build build --config Release
```

Point it at your own game install by copying `LocalConfig.cmake.example` to
`LocalConfig.cmake` and editing one line:

```cmake
set(GF2ASI_GAME_DIR "D:/Games/The Godfather II")
```

`LocalConfig.cmake` is gitignored, so your paths never reach a commit. CMake
reads it before it declares any of its own defaults, so what you set there wins
and takes effect on the next configure — no command-line arguments, and no need
to delete the build directory. Use forward slashes, or escape backslashes
(`"D:\\Games\\The Godfather II"`) — a bare backslash is a CMake escape
character.

The same file sets the rest: `GF2ASI_GAME_EXE` (the executable the debugger
launches), `GF2ASI_DEBUGGER_ARGS`, `GF2ASI_DEPLOY` (copy the `.asi` into
`<game>/scripts` after linking, on by default) and `GF2ASI_USE_PCH`.

All of them are cache variables when `LocalConfig.cmake` does not set them, so
a preset, the CMake GUI or `-D` work too if you prefer. Bear in mind that a
cache variable is written once: with the Visual Studio generator, changing a
default in `CMakeLists.txt` afterwards has no effect on an existing build
directory, which is the reason `LocalConfig.cmake` exists.

A successful build deploys `GF2ASI.asi` and `discord_game_sdk.dll` into
`<game>/scripts`. CMake warns at configure time if the game directory is
missing, if the executable is not where it expects, or if there is no ASI
loader present — a build that deploys into a game with no loader looks exactly
like a build that did not take.

## Running

You need an ASI loader in the game directory. This project is developed against
a `dinput8.dll` proxy loader.

Launch the game from its own executable. Launching through the Visual Studio
debugger has historically produced a perpetual black screen; the debugger
command and working directory are configured for you, but treat that path as
unproven.

## Repository layout

```
Source/SDK/         reconstructed engine and game code, original layout
Source/Platform/    the primitives the SDK is built on: calling into the game
                    image, and the sinks it reports diagnostics through
Source/Addons/      GF2Mod — config, logging, ImGui runtime, keybinds
Source/Scripthook/  the fixes, the mod loader, the menu, the detours
Reference/          the recovered original file manifest and build baselines
Tools/              the scripts that measure and enforce all of the above
Vendors/            prebuilt third-party libraries
```

## Documentation

- [CONTRIBUTING.md](CONTRIBUTING.md) — how changes get in, including the rules
  for AI-assisted work, what "matching the original" means, the naming standard
  for members, parameters and locals, and the annotations used to flag bugs and
  optimisations.
- [ROADMAP.md](ROADMAP.md) — what is left, by package, file and class.
- [RESTRUCTURE_PLAN.md](RESTRUCTURE_PLAN.md) — the repository restructure that
  got the tree into the original layout. Phases 0–5 are done; it is kept as the
  record of why things are where they are.
- [Tools/README.md](Tools/README.md) — every script, what it reads, and what its
  numbers do and do not mean.

## Why this is open source

- So other people can contribute to it.
- So other projects on the EARS engine (EA Redwood Shores, roughly 2005–2013)
  have something to work from — the framework is close enough across titles to
  be useful.
- So the mod can be shown not to be a virus if Nexus Mods blocks downloads.

## Licensing

There is no licence file. The reconstructed engine code in `Source/SDK` derives
from a commercial game, and `Vendors/` contains third-party libraries under
their own terms. If you are planning to redistribute anything from here, work
that out first.
