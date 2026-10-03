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

## plan_moves.py / apply_moves.py

The restructure move machinery. Planning is separate from applying so a plan can
be read and argued with before anything moves.

```bash
python Tools/plan_moves.py --phase 2b --out moves_2b.tsv
python Tools/apply_moves.py moves_2b.tsv --dry-run
python Tools/apply_moves.py moves_2b.tsv
```

`plan_moves.py` emits `old<TAB>new` rows relative to `Source/SDK`. Where a file
matches the manifest its target comes straight from there, which is the only way
to get the cases a rule cannot predict — the `ears_common` include/src split,
the 11 `ears_framework` files under `src/game_framework/framework/`, and
`ears_trinity`, whose headers sit directly in `include/` with no package
subdirectory. A per-package rule covers the rest. It reports collisions, which
are always a planning bug.

`apply_moves.py` does three things that have to happen together or the tree
stops building: rewrites `SDK/<old>` include directives to `SDK/<new>`
(case-insensitively, so an already-miscased directive gets corrected rather than
missed), rewrites bare sibling includes that the move separates, and performs
the moves with git recording renames.

Two traps it handles, both found the hard way:

- **Case-only renames.** Git on Windows defaults to `core.ignorecase=true`, and
  `EARS_Common` and `ears_common` are the same directory to the filesystem.
  Files go through `Source/SDK/__restructure/` and the emptied originals are
  removed before the staged tree moves up. Never rename a package directory in
  place.
- **A surviving original directory.** If some of a package's files were already
  correctly placed they are not in the move list, so the old directory is not
  empty, and moving the staged directory would nest it inside — giving
  `SDK/rwfilesystem/rwfilesystem/`. The contents are moved individually in that
  case.

A third rewrite matters for 2c, where files keep their directory: a bare
`#include "NPC.h"` stays bare, but the basename itself changes case, and
`"NPC.h"` pointing at `npc.h` compiles here and nowhere else.

It also preserves uncommitted work: for a file with unstaged changes the index
entry is rebuilt as HEAD plus the mechanical rewrite, so a commit carries the
rename and the include fix and leaves everything else in the working tree.

Two Windows details worth knowing if this ever misbehaves. Writes retry briefly,
because an editor or antivirus holding a handle for a moment makes `open` fail
with EINVAL and that is likely enough across hundreds of files to lose a run
halfway. And `git()` decodes with UTF-8 and surrogateescape rather than
`text=True`: the locale codec here is cp1252, and one stray byte in a source
file (`ImGuiManager.cpp` carries a `0x9d`) kills subprocess's reader thread and
returns None instead of the content.

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

## function_coverage.py

Per-function coverage, cross-referenced against the linker map. `sdk_coverage.py`
counts files; this counts what is inside them.

```bash
python Tools/function_coverage.py --summary
python Tools/function_coverage.py > coverage.csv
python Tools/function_coverage.py --file modules/families/family.cpp --summary
```

The expected set is every `.text` symbol in an EARS or RenderWare object, with
class and function pulled out of the mangled name. The done set comes from
parsing our own sources for the matching file. Both sides key on
`(class, function)`, so a difference in our parameter or return types does not
register as missing.

Matching is **case-insensitive**, which is not laziness: the original has
`NPC::ActivateHudIndicator` where we wrote `ActivateHUDIndicator`. Our acronym
casing often differs, and `--summary` lists every such divergence so it can be
aligned if you want.

Three things to know before trusting a figure:

- Functions the original defined inline in a header are emitted into whichever
  objects used them, not into their own file's object. They are attributed to
  the wrong file or missed, so per-file totals understate header-heavy types.
  `npc.cpp` reads 2 of 345 because most of what we have is inline in `npc.h`.
- Addresses are Xbox 360 (`0x82xxxxxx`). The PC build the project targets has
  different ones, so treat them as a cross-reference for locating the PC
  equivalent, never as something to paste into a thunk.
- Templates, vtables, RTTI, string literals and compiler-generated destructors
  are counted separately and excluded from the tallies — around 31k symbols.

The CSV is not committed. It is ~41k rows and regenerating it would produce a
large diff every time, which would bury the signal rather than track it.

## check_sources.py

Brace balance and duplicate includes, per file.

```bash
python Tools/check_sources.py
python Tools/check_sources.py --path Source/SDK/ears_rt_cct
```

This exists because the 2d-ii merges shipped code that would not compile, and
the verification at the time could not see it. That check compared every
meaningful line of each source against the merged result and reported "all
source lines accounted for" — while skipping `{` and `}` lines as noise. The
merge had stripped the closing brace off the last function in five of six files,
which is exactly a `}` line.

Run against the commit before the fixes it names all seven broken files, so it
is checked against a real failure rather than only against a clean tree:

```
npccoversm.cpp: 2 unclosed {        npcguncombatsm.cpp: 3 unclosed {
npcsearchsm.cpp: 3 unclosed {       npcshootsm.cpp: 3 unclosed {
playerdebug.cpp: 4 unclosed {       playerswitchitemsm.cpp: 2 unclosed {
chrcntl_character.cpp: 2 extra }
```

Braces are counted outside comments and string and character literals, so a `{`
in a comment or a `"}"` in a string does not register. Exits non-zero when any
file is unbalanced.

It also notes how many closing braces carry a trailing comment (`} // CCT`),
because a merge tool that matches on a bare `}` will miss those and append
outside the namespace — which is how CCTNameNode ended up outside `EA::CCT`.

## Reference data these read

| File | Purpose |
|---|---|
| `../Reference/original_tree.txt` | the manifest, generated |
| `../Reference/project_owned.txt` | files with no original at all |
| `../Reference/consolidated.txt` | original types, but the original file is named something else; resolved in Phase 2d |
| `../Reference/rename_overrides.txt` | hand-confirmed paths no rule derives |
| `../Reference/build_baseline.md` | pre-restructure build config |
