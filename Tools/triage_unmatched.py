#!/usr/bin/env python3
"""Work out the original name of each SDK file that sdk_coverage.py cannot match.

Five passes, strongest evidence first. There is deliberately no fuzzy string
matching: it proposes nonsense (Bitflags.cpp -> printf.cpp,
PlayerCamera.cpp -> playercrewleadercomponent.cpp). Anything these passes cannot
settle is reported as unresolved for a human, not guessed at.

  0. override   Reference/rename_overrides.txt -- answers already confirmed by
                hand. Always wins, so a verified path is never re-guessed.
  1. alias      Known systematic misnamings in this project, each confirmed
                against the manifest. Currently just ChrCntrl_ -> chrcntl_.
  2. name       Deterministic transforms of the file name, looked up in the same
                package: CamelCase -> snake_case (BuildingManager.cpp ->
                building_manager.cpp) and underscore-stripped.
  3. symbol     Types declared or defined *inside* the file, resolved through the
                linker map to the object they compiled into, then to that
                object's source path. Authoritative, and necessary because a
                misnamed file's stem is no guide -- CorleoneData.h declares
                CorleoneFamilyData. For a .cpp this reads "Type::Member"
                definitions, since implementation files rarely declare anything.
  4. sibling    If X.h resolved to foo.h, then X.cpp is foo.cpp when the manifest
                has it. Pure bookkeeping, applied last and iterated.

RenderWare packages were unity builds, so their objects are
bb_runtime.<pkg>.cpp.*.obj and pass 3 can never name a file for them; they
resolve by pass 2 or stay unresolved.

Usage:
    python triage_unmatched.py                  # report
    python triage_unmatched.py --emit-owned     # paths for project_owned.txt
    python triage_unmatched.py --emit-renames   # "current<TAB>original" rows
"""

from __future__ import annotations

import argparse
import collections
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from map_symbol_to_obj import DEFAULT_MAP, load_map  # noqa: E402
from sdk_coverage import (DEFAULT_MANIFEST, DEFAULT_SDK, PACKAGE_DIRS,  # noqa: E402
                          load_manifest)

DEFAULT_OVERRIDES = (Path(__file__).resolve().parent.parent
                     / "Reference" / "rename_overrides.txt")

# Types declared in a header.
DECL_RE = re.compile(r"^\s*(?:class|struct)\s+([A-Za-z_]\w*)\s*(?:final\s*)?[:{]",
                     re.MULTILINE)
# Out-of-line member definitions in a .cpp: "void Foo::Bar(", "Foo::Foo(".
DEFN_RE = re.compile(r"^[A-Za-z_][\w:<>,*&\s]*?\b([A-Za-z_]\w*)::~?\w+\s*\(",
                     re.MULTILINE)

# Systematic misnamings in this project, confirmed against the manifest.
# ChrCntrl_* : the original CCT prefix is chrcntl_ (no 'r' after "Cnt").
ALIASES: tuple[tuple[re.Pattern[str], str], ...] = (
    (re.compile(r"^chrcntrl_"), "chrcntl_"),
)


def snake(name: str) -> str:
    step = re.sub(r"(.)([A-Z][a-z]+)", r"\1_\2", name)
    return re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", step).lower()


def types_in(path: Path) -> list[str]:
    """Types this file declares or defines, in source order."""
    try:
        text = path.read_text(encoding="utf-8", errors="replace")
    except OSError:
        return []
    seen: dict[str, None] = {}
    for pattern in (DECL_RE, DEFN_RE):
        for name in pattern.findall(text):
            if name not in ("if", "for", "while", "switch", "return", "else"):
                seen.setdefault(name, None)
    return list(seen)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--sdk", type=Path, default=DEFAULT_SDK)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--map", type=Path, default=DEFAULT_MAP)
    parser.add_argument("--overrides", type=Path, default=DEFAULT_OVERRIDES)
    parser.add_argument("--emit-owned", action="store_true")
    parser.add_argument("--emit-renames", action="store_true")
    args = parser.parse_args()

    overrides: dict[str, str] = {}
    if args.overrides.is_file():
        for line in args.overrides.read_text(encoding="utf-8").splitlines():
            line = line.strip()
            if not line or line.startswith("#") or "\t" not in line:
                continue
            current, original = (part.strip() for part in line.split("\t", 1))
            overrides[current.lower().replace("\\", "/")] = original

    packages = load_manifest(args.manifest)
    by_name: dict[str, dict[str, list[str]]] = {}
    by_stem: dict[str, dict[str, list[str]]] = {}
    for pkg, paths in packages.items():
        names: dict[str, list[str]] = collections.defaultdict(list)
        stems: dict[str, list[str]] = collections.defaultdict(list)
        for intra in paths:
            base = intra.rsplit("/", 1)[-1]
            names[base].append(intra)
            stems[base.rsplit(".", 1)[0]].append(intra)
        by_name[pkg], by_stem[pkg] = names, stems

    sym_index: dict[str, collections.Counter] = collections.defaultdict(collections.Counter)
    if args.map.is_file():
        for symbol, obj in load_map(args.map):
            for comp in symbol.lower().lstrip("?").split("@"):
                if comp:
                    sym_index[comp][obj] += 1

    unmatched = subprocess.run(
        [sys.executable, str(Path(__file__).parent / "sdk_coverage.py"),
         "--only", "unmatched", "--sdk", str(args.sdk), "--manifest", str(args.manifest)],
        capture_output=True, text=True, check=True).stdout.split()

    resolved: dict[str, tuple[str, str]] = {}   # rel -> (pkg/intra, evidence)
    pending: list[str] = []

    def lookup_name(pkg: str, stem: str, ext: str) -> str | None:
        for cand in (f"{stem}.{ext}", f"{snake(stem)}.{ext}",
                     f"{stem.replace('_', '')}.{ext}"):
            if cand in by_name[pkg]:
                return by_name[pkg][cand][0]
        return None

    for rel in unmatched:
        pkg = PACKAGE_DIRS.get(rel.split("/")[0])
        if pkg is None:
            continue
        path = args.sdk / rel
        ext = path.suffix.lstrip(".").lower()
        stem = path.stem

        # pass 0 -- hand-confirmed overrides
        hit = why = None
        override = overrides.get(rel.lower())
        if override:
            hit, why = override.split("/", 1)[1], "override"

        # pass 1 -- aliases
        aliased = stem.lower()
        for pattern, repl in ALIASES:
            aliased = pattern.sub(repl, aliased)
        if hit is None and aliased != stem.lower():
            found = lookup_name(pkg, aliased, ext)
            if found:
                hit, why = found, "alias"

        # pass 2 -- name transforms
        if hit is None:
            found = lookup_name(pkg, stem, ext)
            if found:
                hit, why = found, "name"

        # pass 3 -- symbols
        if hit is None:
            for type_name in types_in(path):
                tally = sym_index.get(type_name.lower())
                if not tally:
                    continue
                for obj, _n in tally.most_common():
                    if obj.startswith("bb_runtime."):
                        continue
                    cands = by_stem[pkg].get(obj.rsplit(".", 1)[0], [])
                    pick = [c for c in cands if c.endswith("." + ext)]
                    if pick:
                        hit, why = pick[0], f"symbol {type_name}"
                        break
                if hit:
                    break

        if hit:
            owner = override.split("/", 1)[0] if why == "override" else pkg
            resolved[rel] = (f"{owner}/{hit}", why)
        else:
            pending.append(rel)

    # pass 4 -- sibling pairing, iterated until it stops finding anything
    changed = True
    while changed:
        changed = False
        for rel in list(pending):
            pkg = PACKAGE_DIRS.get(rel.split("/")[0])
            path = args.sdk / rel
            ext = path.suffix.lstrip(".").lower()
            for sib_ext in ("h", "cpp", "inl"):
                if sib_ext == ext:
                    continue
                sibling = f"{rel[: -len(ext)]}{sib_ext}"
                if sibling not in resolved:
                    continue
                sib_intra = resolved[sibling][0].split("/", 1)[1]
                base = sib_intra.rsplit("/", 1)[-1].rsplit(".", 1)[0]
                cands = by_stem[pkg].get(base, [])
                pick = [c for c in cands if c.endswith("." + ext)]
                if pick:
                    resolved[rel] = (f"{pkg}/{pick[0]}", f"sibling of {Path(sibling).name}")
                    pending.remove(rel)
                    changed = True
                    break

    if args.emit_owned:
        for rel in sorted(pending):
            print(rel)
        return 0
    if args.emit_renames:
        for rel in sorted(resolved):
            print(f"{rel}\t{resolved[rel][0]}")
        return 0

    width = max((len(r) for r in resolved), default=0)
    print(f"=== RESOLVED ({len(resolved)}) ===")
    for rel in sorted(resolved):
        orig, why = resolved[rel]
        print(f"  {rel:{width}}  ->  {orig}   [{why}]")
    print(f"\n=== UNRESOLVED ({len(pending)}) ===")
    print("  project-owned, or needs a hand look at the manifest\n")
    for rel in sorted(pending):
        types = types_in(args.sdk / rel)[:3]
        tag = f"   (types: {', '.join(types)})" if types else ""
        print(f"  {rel}{tag}")
    tally = collections.Counter(w.split()[0] for _, w in resolved.values())
    print("\nevidence: " + ", ".join(f"{k}={v}" for k, v in tally.most_common()))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
