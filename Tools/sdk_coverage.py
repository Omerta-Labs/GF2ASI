#!/usr/bin/env python3
"""Compare Source/SDK against the original source tree recovered from the PDBs.

Reports three things:

  placed      the file sits where the original did
  misplaced   the original has this file under a different path
  unmatched   no original by that name -- either ours, or renamed (run
              map_symbol_to_obj.py on the class to find the original)

plus per-package reconstruction coverage.

Path comparison is suffix-based, so this works against both the pre-Phase-2
layout (Source/SDK/EARS_Godfather/Modules/Families/Family.cpp) and the
post-Phase-2 one (Source/SDK/ears_godfather/src/modules/families/family.cpp).
After Phase 2c the PACKAGE_DIRS mapping below becomes an identity.

Usage:
    python sdk_coverage.py
    python sdk_coverage.py --verbose          # list every placed file too
    python sdk_coverage.py --package ears_godfather
"""

from __future__ import annotations

import argparse
import collections
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
DEFAULT_SDK = REPO / "Source" / "SDK"
DEFAULT_MANIFEST = REPO / "Reference" / "original_tree.txt"
DEFAULT_OWNED = REPO / "Reference" / "project_owned.txt"
DEFAULT_CONSOLIDATED = REPO / "Reference" / "consolidated.txt"

SOURCE_EXT = (".cpp", ".hpp", ".inl", ".h", ".c")

# Project directory under Source/SDK -> original package name.
# Phase 2c renames these directories to the package name, after which every
# entry is an identity mapping and this table can be deleted.
PACKAGE_DIRS = {
    "ears_common": "ears_common",
    "ears_framework": "ears_framework",
    "ears_godfather": "ears_godfather",
    "ears_locale": "ears_locale",
    "ears_physics": "ears_physics",
    "ears_rt_cct": "ears_rt_cct",
    "ears_rt_llrender": "ears_rt_llrender",
    "ears_statemachine": "ears_statemachine",
    "ears_trinity": "ears_trinity",
    "rwcontroller": "rwcontroller",
    "rwfilesystem": "rwfilesystem",
    # pre-Phase-2 spellings
    "EARS_Common": "ears_common",
    "EARS_Framework": "ears_framework",
    "EARS_Godfather": "ears_godfather",
    "EARS_Locale": "ears_locale",
    "EARS_Physics": "ears_physics",
    "EARS_RT_CCT": "ears_rt_cct",
    "EARS_RT_LLRender": "ears_rt_llrender",
    "EARS_StateMachine": "ears_statemachine",
    "EARS_Trinity": "ears_trinity",
}

STRIPPABLE_ROOTS = ("include/", "src/", "source/")


def load_manifest(path: Path) -> dict[str, list[str]]:
    """package -> [intra paths]"""
    packages: dict[str, list[str]] = collections.defaultdict(list)
    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.strip()
        if not line or "/" not in line:
            continue
        package, intra = line.split("/", 1)
        packages[package].append(intra)
    return packages


def load_owned(path: Path) -> set[str]:
    """Project-relative paths to exclude from the unmatched report.

    Reads both project_owned.txt (one path per line) and consolidated.txt
    (path TAB evidence), so either format works here.
    """
    if not path.is_file():
        return set()
    paths = set()
    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        paths.add(line.split("	")[0].strip().lower().replace("\\", "/"))
    return paths


def candidates(intra_paths: list[str], basename: str) -> list[str]:
    return [p for p in intra_paths if p.rsplit("/", 1)[-1] == basename]


def matches(project_rel: str, intra: str) -> bool:
    """True if intra is the same logical path as project_rel.

    Compared as a suffix so a project tree that omits the package's own source
    root (src/, include/<pkg>/, src/game_framework/framework/, ...) still lines
    up with the original.
    """
    stripped = project_rel
    for root in STRIPPABLE_ROOTS:
        if stripped.startswith(root):
            stripped = stripped[len(root):]
            break
    for probe in (project_rel, stripped):
        if intra == probe or intra.endswith("/" + probe):
            return True
    return False


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--sdk", type=Path, default=DEFAULT_SDK)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--owned", type=Path, default=DEFAULT_OWNED)
    parser.add_argument("--consolidated", type=Path, default=DEFAULT_CONSOLIDATED,
                        help="files whose types are original but whose original "
                             "file carries another name; resolved in Phase 2d")
    parser.add_argument("--package", help="restrict the report to one package")
    parser.add_argument("--verbose", action="store_true", help="also list placed files")
    parser.add_argument("--only", choices=("placed", "misplaced", "unmatched"),
                        help="print just those paths, one per line, for scripting")
    args = parser.parse_args()

    if not args.manifest.is_file():
        print(f"error: manifest not found: {args.manifest}\n"
              f"       run Tools/extract_pdb_paths.py first")
        return 1

    packages = load_manifest(args.manifest)
    owned = load_owned(args.owned)
    consolidated = load_owned(args.consolidated)

    placed: list[tuple[str, str]] = []
    misplaced: list[tuple[str, list[str]]] = []
    unmatched: list[str] = []
    skipped = 0
    pending_merge = 0
    covered: dict[str, set[str]] = collections.defaultdict(set)
    unknown_dirs: set[str] = set()

    for path in sorted(args.sdk.rglob("*")):
        if not path.is_file() or path.suffix.lower() not in SOURCE_EXT:
            continue
        rel = path.relative_to(args.sdk).as_posix()
        top = rel.split("/")[0]
        package = PACKAGE_DIRS.get(top)
        if package is None:
            unknown_dirs.add(top)
            continue
        if args.package and package != args.package:
            continue
        if rel.lower() in owned:
            skipped += 1
            continue
        if rel.lower() in consolidated:
            pending_merge += 1
            continue

        project_rel = rel.split("/", 1)[1].lower() if "/" in rel else rel.lower()
        basename = project_rel.rsplit("/", 1)[-1]
        cands = candidates(packages.get(package, []), basename)

        if not cands:
            unmatched.append(rel)
            continue
        hit = next((c for c in cands if matches(project_rel, c)), None)
        if hit is not None:
            placed.append((rel, f"{package}/{hit}"))
            covered[package].add(hit)
        else:
            misplaced.append((rel, [f"{package}/{c}" for c in cands]))
            covered[package].update(cands)

    if args.only:
        rows = {"placed": [r for r, _ in placed],
                "misplaced": [r for r, _ in misplaced],
                "unmatched": unmatched}[args.only]
        for rel in rows:
            print(rel)
        return 0

    if misplaced:
        print(f"=== MISPLACED ({len(misplaced)}) ===")
        for rel, cands in misplaced:
            print(f"  {rel}")
            for c in cands:
                print(f"      original: {c}")
        print()

    if unmatched:
        print(f"=== UNMATCHED ({len(unmatched)}) ===")
        print("  no original of this name; run map_symbol_to_obj.py on the class,")
        print("  or add to Reference/project_owned.txt if it is ours\n")
        for rel in unmatched:
            print(f"  {rel}")
        print()

    if args.verbose and placed:
        print(f"=== PLACED ({len(placed)}) ===")
        for rel, orig in placed:
            print(f"  {rel}\n      -> {orig}")
        print()

    print("=== COVERAGE ===")
    print(f"{'package':20} {'have':>6} {'total':>6} {'%':>6}")
    mirrored = sorted({p for p in PACKAGE_DIRS.values()
                       if not args.package or p == args.package})
    have_total = total_total = 0
    for package in mirrored:
        total = len(packages.get(package, []))
        have = len(covered.get(package, ()))
        have_total += have
        total_total += total
        pct = (100.0 * have / total) if total else 0.0
        print(f"{package:20} {have:6} {total:6} {pct:5.1f}%")
    pct = (100.0 * have_total / total_total) if total_total else 0.0
    print(f"{'TOTAL':20} {have_total:6} {total_total:6} {pct:5.1f}%")

    print(f"\nplaced {len(placed)}  misplaced {len(misplaced)}  "
          f"unmatched {len(unmatched)}  to-merge {pending_merge}  "
          f"project-owned {skipped}")
    if unknown_dirs:
        print(f"unmapped directories under {args.sdk.name}: "
              f"{', '.join(sorted(unknown_dirs))}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
