# Contributing

The aim is a faithful recompilation (see [README.md](README.md)). That changes
what a good contribution looks like: code here is judged against the original
game first and against taste second. A tidier implementation that behaves
differently is a regression, not an improvement.

## Review

**Every change arrives as a pull request, and a programmer other than the author
reviews it.** No direct pushes to `main`.

### AI-assisted work

AI assistance is welcome and used in this project. It comes with one condition:

**A human programmer must own the output.** Specifically:

- A programmer oversees the work as it is produced, reads all of it, and opens
  the pull request under their own name.
- That programmer is accountable for every line in it and must be able to
  explain why each function behaves the way it does, independently of whatever
  the model said about it.
- A second programmer reviews it, as for any other PR.

An agent does not open pull requests here, and unattended automation is not
accepted no matter how good the diff looks. The reason is specific to this
project rather than general suspicion: a model asked to reconstruct
`StateMachineTree::Update` from a name and a signature will produce something
plausible, compiling and wrong, and nothing downstream will catch it. Fidelity
to a binary is not checkable by review of the C++ alone — it needs someone who
has looked at the disassembly.

Mark AI-assisted commits with a `Co-Authored-By:` trailer naming the model. It
is useful history, not a confession.

### What a reviewer is checking

- Does it match the original? Name, signature, const-ness, access level, the
  file it lives in.
- Is anything here invented? If a function's behaviour was inferred rather than
  recovered, it should say so (see `UNVERIFIED` below).
- Are divergences from the original deliberate, marked, and justified?
- Do the tooling checks pass?

## Matching the original

**File paths and names.** `Reference/original_tree.txt` holds all 5,890 original
source paths, recovered from the shipped debug symbols. A new file's location
and name are a lookup, not a decision — exact original lowercase, so
`ui_frontend.cpp`, `building_manager.cpp`, `chrcntl_character.h`. Where the
linker map shows several classes compiled into one object, they belong in one
file, as they were.

Do not create empty files to stake out the tree. The manifest already is that
index, and a stub makes `sdk_coverage.py` report a file as present when nothing
is in it.

**Function names and signatures.** The linker maps carry the mangled name of
every symbol, which pins the class, the function name, the parameter types, the
return type, the const-ness and the access level. Recover the signature from
there rather than inventing a plausible one. Where our casing has drifted from
the original — `ActivateHUDIndicator` against the original's
`ActivateHudIndicator` — the original wins; `function_coverage.py --summary`
lists every such divergence.

**Identifier names we do not take from the original.** Everything above is about
the names the linker map pins: files, classes, functions, their signatures. Three
kinds of identifier are deliberately *not* recovered from the original, because
they are invisible to every consumer of the code and consistency across the tree
is worth more than fidelity to one 2008 author's habits:

| Kind | Form | Example |
|---|---|---|
| Class and struct members | `m_` + CapitalisedName, `m_b` for a `bool` | `m_Arr`, `m_ClassName`, `m_bApplyOnLoad` |
| Parameters, in | `In` + CapitalisedName, `bIn` for a `bool` | `InVenueID`, `InCapacity`, `bInResetCamera` |
| Parameters, out | `Out` + CapitalisedName | `OutSearchKey`, `OutPosition` |
| Local variables | CapitalisedName, `b` for a `bool` | `ObjIdx`, `Element`, `bRemoved` |

Apply this even when the disassembly or the PDB hands you the original spelling.
`RegArr`'s debug asserts name its member `mArr` and the PDB gives its nested
`RegData` fields as `mKey`/`mData`; in this tree they are `m_Arr`, `m_Key` and
`m_Data`. Keep the original *word* — `mArr` becomes `m_Arr`, not `m_Items` — and
restyle it. Where the original name is the only record of what a field means,
that belongs in a comment, not in the spelling.

None of this is layout-affecting, so it cannot cause the silent wrong read that
a misplaced field can.

**Class layout.** Members go in the class they belong to, in the original order,
with a `static_assert` on `sizeof` wherever the size is known. These types are
reinterpret_cast over live game memory, so a field in the wrong place is a
silent wrong read rather than a compile error.

**No inline assembly in reconstructed code.** `Source/Platform/MemUtils.h` has
the one exception, for a thunk that has to pass an argument in EAX, and it is
contained there deliberately. Do not add more.

**No disassembly listings in comments.** Record the address and what you
established from it, not the instructions.

## Annotations

Findings about the original code are recorded in the source, uppercase and
greppable, so they survive and can be audited:

| Tag | Means |
|---|---|
| `MAYBE_BUG` | The original looks wrong. Say what input produces the wrong result. |
| `MAYBE_OPTIMISE` | The original is correct but wasteful. Say what it costs. |
| `DEVIATION` | We intentionally differ from the original. Say what and why. |
| `UNVERIFIED` | Written, but not confirmed against the disassembly. |
| `TODO` | Known incomplete work, with enough detail to act on. |

One line, tag, colon, the claim — then as much detail as it needs:

```cpp
// MAYBE_BUG: clamps after the divide, so a zero radius gives inf rather than
// the intended 0. Reachable from any ScreenZoomBlur::Params built by level data.
```

Two of these carry weight beyond documentation:

- **`DEVIATION` is the stage-2 audit list.** A standalone executable built from
  code that quietly "fixed" things will not behave like the game. Every
  deliberate difference needs to be findable.
- **`UNVERIFIED` is what tells a reviewer where to look.** Anything inferred
  from a name, a signature or a sibling implementation rather than read out of
  the disassembly belongs behind this tag. It is not an admission of sloppiness;
  an unmarked guess is the problem.

An annotation with no content (`// TODO: not sure if this works`) is worse than
nothing. Say what is unverified and how to verify it.

## Comments

Comments explain the **system**: non-obvious engine behaviour, which thread
something runs on, where a hardcoded address came from, a bug in the original,
why a reconstruction had to deviate. They do not narrate the work that produced
the change — no refactor history, no "moved out of X", no references to a plan
or a conversation. `git log` holds that already.

The test: read it as someone opening the file in two years with no knowledge of
how it got there.

## Before opening a PR

Builds are done in Visual Studio, not from a script. Beyond compiling, run the
checks — they are fast and catch things a compiler does not:

```bash
python Tools/check_sources.py    # brace balance, duplicate includes
python Tools/check_includes.py   # every project include resolves, correct case
python Tools/check_layers.py     # the SDK never reaches up into the mod layer
python Tools/sdk_coverage.py     # nothing misplaced against the manifest
```

`check_layers.py` is the one worth understanding. The SDK must not include from
`Addons/`, `Scripthook/` or `Utils/`, or touch ImGui or polyhook. Where the SDK
needs something from the layer above — logging, debug rendering, a mod
injection point — it goes through a sink table in `Source/Platform/`
(`EARS::Diag`, `EARS::Host`, `EARS::ModPoints`) that the host fills in at
start-up. A data dependency counts: a coupling through a shared global is as
much a violation as an `#include`, and has broken the build here before.

If you touched reconstruction, say what the coverage numbers were before and
after:

```bash
python Tools/function_coverage.py --summary
python Tools/function_coverage.py --file modules/families/family.cpp --summary
```

## What not to commit

- Game paths. `GF2ASI_GAME_DIR` and friends are CMake cache variables; put
  yours in `CMakeUserPresets.json`, which is gitignored.
- The function coverage CSV. It is ~41k rows and regenerating it would bury the
  signal in diff noise. The summary is what gets quoted.
- Anything from `J:\Projects\RE Material` — the PDBs, maps and IDA databases
  stay out of the repository.
