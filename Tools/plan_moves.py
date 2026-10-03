#!/usr/bin/env python3
"""Generate the move list for a phase of the SDK restructure.

Writes "old<TAB>new" rows, both relative to Source/SDK, for Tools/apply_moves.py
to carry out. Separating planning from applying means the plan can be read and
argued with before anything moves.

Phases:

  2b  Package roots. Lowercases the package directory and every directory
      beneath it, and inserts the package's own source root -- src/,
      src/framework/, include/ears_common/ and so on. File basenames are left
      alone; that is 2c. The version level (dev, release, hk550) is dropped: it
      is a build-system artefact.

      Where a file matches the manifest, its directory comes straight from the
      manifest, which is the only way to get the cases a rule cannot predict:
      the ears_common include/src split, and the 11 ears_framework files that
      belong to src/game_framework/framework/ rather than src/framework/.
      Otherwise the per-package rule below applies, and 2d corrects any that
      land in the wrong directory.

  2d  Names for the files the manifest cannot match, from
      Tools/triage_unmatched.py -- which uses the linker map, so it can place a
      file whose name bears no resemblance to the original. Any original claimed
      by more than one project file is a merge, not a rename, and is reported
      separately rather than planned: several of our classes shared one file in
      the original, and combining them is hand work.

  2c  File basenames to exact original lowercase, for every file that matches
      the manifest. Directories are already lowercase after 2b, so these are
      same-directory, case-only renames. Files with no manifest match are left
      for 2d, which has the linker map to go on.

Usage:
    python plan_moves.py --phase 2b
    python plan_moves.py --phase 2b --out moves_2b.tsv
"""

from __future__ import annotations

import argparse
import collections
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from sdk_coverage import (DEFAULT_MANIFEST, DEFAULT_SDK, PACKAGE_DIRS,  # noqa: E402
                          SOURCE_EXT, load_manifest, matches)

# Where a package puts headers and sources, for files the manifest cannot place.
# ("<header root>", "<source root>"); an empty header root means the package
# keeps headers beside sources, with no include/ directory at all.
PACKAGE_ROOTS = {
    "allocator":         ("include/allocator/",        "src/"),
    "ears_common":       ("include/ears_common/",      "src/"),
    "ears_framework":    ("",                          "src/framework/"),
    "ears_godfather":    ("",                          "src/"),
    "ears_locale":       ("include/ears_locale/",      "src/"),
    "ears_physics":      ("",                          "src/ears_physics/"),
    "ears_rt_cct":       ("include/ears_rt_cct/",      "src/"),
    "ears_rt_llrender":  ("include/ears_rt_llrender/", "src/"),
    "ears_statemachine": ("include/ears_statemachine/", "src/"),
    "ears_trinity":      ("include/",                  "src/"),
    "rwcontroller":      ("include/rw/core/controller/", "source/"),
    "rwfilesystem":      ("include/rw/core/filesys/",  "source/"),
}

HEADER_EXT = (".h", ".hpp", ".inl")
STRIPPABLE = ("include/", "src/", "source/")


def strip_root(rel_dir: str) -> str:
    """Drop a leading include/, src/ or source/ the project added itself."""
    for root in STRIPPABLE:
        if rel_dir == root.rstrip("/"):
            return ""
        if rel_dir.startswith(root):
            return rel_dir[len(root):]
    return rel_dir


def plan_2b(sdk: Path, manifest: Path) -> list[tuple[str, str]]:
    packages = load_manifest(manifest)
    by_name: dict[str, dict[str, list[str]]] = {}
    for pkg, paths in packages.items():
        idx: dict[str, list[str]] = collections.defaultdict(list)
        for intra in paths:
            idx[intra.rsplit("/", 1)[-1]].append(intra)
        by_name[pkg] = idx

    moves: list[tuple[str, str]] = []
    for path in sorted(sdk.rglob("*")):
        if not path.is_file() or path.suffix.lower() not in SOURCE_EXT:
            continue
        rel = path.relative_to(sdk).as_posix()
        top = rel.split("/")[0]
        pkg = PACKAGE_DIRS.get(top)
        if pkg is None or pkg not in PACKAGE_ROOTS:
            continue

        name = path.name
        within = rel.split("/", 1)[1] if "/" in rel else name       # below the package dir
        rel_dir = within.rsplit("/", 1)[0] if "/" in within else ""

        # Manifest first: it knows the splits no rule can predict.
        target_dir = None
        for intra in by_name[pkg].get(name.lower(), []):
            if matches(within.lower(), intra):
                target_dir = intra.rsplit("/", 1)[0] if "/" in intra else ""
                break

        if target_dir is None:
            header_root, source_root = PACKAGE_ROOTS[pkg]
            root = header_root if path.suffix.lower() in HEADER_EXT else source_root
            if not root:                      # package keeps headers with sources
                root = source_root
            target_dir = (root + strip_root(rel_dir).lower()).rstrip("/")

        new = f"{pkg}/{target_dir}/{name}" if target_dir else f"{pkg}/{name}"
        if new != rel:
            moves.append((rel, new))
    return moves


def plan_2c(sdk: Path, manifest: Path) -> list[tuple[str, str]]:
    """Basenames to the manifest's exact spelling, which is always lowercase."""
    packages = load_manifest(manifest)
    by_name: dict[str, dict[str, list[str]]] = {}
    for pkg, paths in packages.items():
        idx: dict[str, list[str]] = collections.defaultdict(list)
        for intra in paths:
            idx[intra.rsplit("/", 1)[-1]].append(intra)
        by_name[pkg] = idx

    moves: list[tuple[str, str]] = []
    for path in sorted(sdk.rglob("*")):
        if not path.is_file() or path.suffix.lower() not in SOURCE_EXT:
            continue
        rel = path.relative_to(sdk).as_posix()
        pkg = PACKAGE_DIRS.get(rel.split("/")[0])
        if pkg is None:
            continue
        within = rel.split("/", 1)[1] if "/" in rel else path.name
        hit = next((intra for intra in by_name[pkg].get(path.name.lower(), [])
                    if matches(within.lower(), intra)), None)
        if hit is None:
            continue                      # no manifest match; 2d deals with it
        original_name = hit.rsplit("/", 1)[-1]
        if original_name == path.name:
            continue                      # already correct
        new = f"{rel.rsplit('/', 1)[0]}/{original_name}" if "/" in rel else original_name
        moves.append((rel, new))
    return moves


def plan_2d(sdk: Path, manifest: Path) -> list[tuple[str, str]]:
    """Triage's proposals, minus the groups that are really merges."""
    import subprocess
    rows = subprocess.run(
        [sys.executable, str(Path(__file__).parent / "triage_unmatched.py"),
         "--emit-renames"],
        capture_output=True, encoding="utf-8", errors="surrogateescape",
        check=True).stdout.splitlines()

    proposals = [tuple(r.split("	", 1)) for r in rows if "	" in r]
    claimants: dict[str, list[str]] = collections.defaultdict(list)
    for cur, orig in proposals:
        claimants[orig].append(cur)

    moves, merges = [], {}
    for cur, orig in proposals:
        # Two ways a proposal is really a merge. Several unmatched files may
        # claim one original -- the three gun state machines all report
        # npcguncombatsm.obj. Or one unmatched file may claim an original that
        # an already-placed file occupies, which no collision check between
        # proposals can see.
        occupied = (sdk / orig).is_file() and orig != cur
        if len(claimants[orig]) > 1 or occupied:
            group = claimants[orig][:]
            if occupied and orig not in group:
                group.append(orig + "   (already placed)")
            merges.setdefault(orig, group)
            continue
        if cur != orig:
            moves.append((cur, orig))

    if merges:
        print(f"{sum(len(v) for v in merges.values())} files in "
              f"{len(merges)} merge groups, left for 2d-ii:", file=sys.stderr)
        for orig, cur in sorted(merges.items()):
            print(f"  {orig}", file=sys.stderr)
            for c in sorted(cur):
                print(f"      {c}", file=sys.stderr)
    return moves


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--phase", required=True, choices=("2b", "2c", "2d"))
    parser.add_argument("--sdk", type=Path, default=DEFAULT_SDK)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--out", type=Path, help="write TSV here instead of stdout")
    args = parser.parse_args()

    planner = {"2b": plan_2b, "2c": plan_2c, "2d": plan_2d}[args.phase]
    moves = planner(args.sdk, args.manifest)
    rows = "".join(f"{old}\t{new}\n" for old, new in moves)
    if args.out:
        args.out.write_text(rows, encoding="utf-8", newline="\n")
        print(f"{len(moves)} moves -> {args.out}", file=sys.stderr)
    else:
        sys.stdout.write(rows)
        print(f"\n{len(moves)} moves", file=sys.stderr)

    # A collision means two files would land on one path: always a planning bug.
    seen: dict[str, str] = {}
    for old, new in moves:
        if new.lower() in seen:
            print(f"COLLISION: {old} and {seen[new.lower()]} both -> {new}",
                  file=sys.stderr)
        seen[new.lower()] = old
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
