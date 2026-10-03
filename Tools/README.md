# Tools

Reconstruction support scripts. Python 3.11+, no third-party packages.

They all read ground truth from the Xbox 360 build output at
`J:\Projects\RE Material\The Godfather II` — 9 PDBs and 4 linker maps across the
singleplayer and multiplayer folders. Pass `--re-material` / `--map` if it lives
somewhere else.

Two independent sources of truth, used for different questions:

- **PDBs** still carry every translation unit's source path, so they give the
  original directory layout and file names.
- **Linker maps** name the object file each symbol was compiled into, so they
  answer "which original file did this class live in" — including when our file
  is named something else entirely.

## extract_pdb_paths.py

Rebuilds `../Reference/original_tree.txt` from the PDBs.

```bash
python Tools/extract_pdb_paths.py
python Tools/extract_pdb_paths.py --all-packages   # include Havok, PlasmaSDK, ...
python Tools/extract_pdb_paths.py --raw            # untouched c:\packages\... paths
```

Output drops the version level (`dev`, `release`, `hk550`), which is a build
artefact, leaving `<package>/<path within package>` — exactly the path each file
should occupy under `Source/SDK/`. Re-run only if the manifest is lost or the
package filter changes; it is committed, so day to day you just read it.

## sdk_coverage.py

Compares `Source/SDK` against the manifest. The one to run after any structural
change.

```bash
python Tools/sdk_coverage.py
python Tools/sdk_coverage.py --package ears_godfather
python Tools/sdk_coverage.py --only misplaced       # bare paths, for scripting
```

Reports **placed** / **misplaced** / **unmatched**, plus per-package coverage.
Path comparison is suffix-based, so it works against both the current layout and
the post-Phase-2 one. Files listed in `../Reference/project_owned.txt` are
excluded.

## map_symbol_to_obj.py

Resolves a class name to the original file it was compiled from.

```bash
python Tools/map_symbol_to_obj.py AmbushSM
python Tools/map_symbol_to_obj.py --file names.txt --quiet
```

The highest symbol count is the defining file; smaller counts are references
from other translation units. A name with no symbols at all is ours, not a
renamed original.

This is also how file consolidations surface — `DrawGunSM`, `HolsterGunSM` and
`ReloadGunSM` all report `npcguncombatsm.obj`, meaning they were one file in the
original.

RenderWare packages were unity builds, so their symbols all land in
`bb_runtime.<pkg>.cpp.*.obj` and cannot be attributed to a file this way.

## triage_unmatched.py

Proposes an original path for everything `sdk_coverage.py` reports as unmatched.
Drives Phase 2d.

```bash
python Tools/triage_unmatched.py
python Tools/triage_unmatched.py --emit-renames   # "current<TAB>original" rows
python Tools/triage_unmatched.py --emit-owned
```

Five passes, strongest evidence first, and the `[evidence]` column says which
one fired:

| Pass | Evidence |
|---|---|
| `override` | `../Reference/rename_overrides.txt`, confirmed by hand |
| `alias` | known systematic misnaming (`ChrCntrl_` → `chrcntl_`) |
| `name` | CamelCase → snake_case, or underscore-stripped |
| `symbol` | types in the file → linker map → object → source path |
| `sibling` | `X.cpp` follows wherever `X.h` resolved |

There is no fuzzy string matching, on purpose — it proposes nonsense like
`Bitflags.cpp → printf.cpp`. Anything the passes cannot settle is reported as
unresolved rather than guessed at.

Treat the output as a proposal. `symbol` hits resting on one generic type name
(an interface, say) are the ones worth eyeballing; the others are deterministic.

## pdb_find_type.py

Asks the PDBs whether a type existed in the original build. This is the check
`map_symbol_to_obj.py` cannot make.

```bash
python Tools/pdb_find_type.py BitArray SMBuilder
python Tools/pdb_find_type.py --all RwMatrixTag
```

A fully inlined template emits no out-of-line code, so no symbol for it ever
reaches the linker map — yet the type is unquestionably original.
`BitArray<0x10000,unsigned int>` and
`EARS::Common::SingleInternalLinkedListLightweight<RWS::CLinkedMsg>` are both
like this, and so is `EARS::Framework::SMBuilder`.

**"Absent from the map" does not mean "ours". Only "absent from the PDBs too"
does.** Run this before concluding a file has no original, and check the types
the file declares — not its stem, which is not a type name.

Output gives each distinct mangled name, so you also get the namespace and, for
a template, the arguments it was instantiated with.

## check_includes.py

Resolves every `#include` in the project and reports the ones that do not exist.

```bash
python Tools/check_includes.py
python Tools/check_includes.py --quiet         # summary only
python Tools/check_includes.py --ignore-case   # skip the casing warnings
```

Stands in for a compile when checking a large rename, which is what Phase 2 is:
~500 files moved and ~550 include directives rewritten, where a single stale
directive is the whole risk. Exits non-zero if anything is unresolved, so it
works in a hook.

Search order matches MSVC, and the `-I` roots mirror `CMakeLists.txt` — keep the
two in step.

It also reports includes whose **casing** differs from the file on disk. Those
build here and nowhere else, and they matter more than usual now: the original
tree is entirely lowercase, so Phase 2c depends on getting case right. Casing is
checked per path component against the real directory listing, because
`Path.resolve()` does not reliably correct case on Windows.

Vendored imgui and the Discord SDK are skipped. They are compiled, but their
platform-conditional includes (Carbon, emscripten, optional stb and freetype
backends) are `#ifdef`'d out on Windows and only produce noise.

## Reference data these read

| File | Purpose |
|---|---|
| `../Reference/original_tree.txt` | the manifest, generated |
| `../Reference/project_owned.txt` | files with no original at all |
| `../Reference/consolidated.txt` | original types, but the original file is named something else; resolved in Phase 2d |
| `../Reference/rename_overrides.txt` | hand-confirmed paths no rule derives |
| `../Reference/build_baseline.md` | pre-restructure build config |
