#!/usr/bin/env python3
"""Resolve a class or symbol name to the original source file it was compiled from.

The linker map names, for every public and static symbol, the object file it came
from. Object basenames match source basenames, so a class name resolves through
the map to an authoritative original file -- which fuzzy name matching cannot do
(it happily proposes Bitflags.cpp -> printf.cpp).

This is also how file consolidations surface: several reconstructed classes that
all report the same object were one file in the original.

    $ python map_symbol_to_obj.py AmbushSM
    AmbushSM
      12x  npcsearchsm.obj
           ears_godfather/src/modules/npc/statemachines/npcsearchsm.cpp
           ears_godfather/src/modules/npc/statemachines/npcsearchsm.h

A name with no symbols here is NOT proof the type is ours. A fully inlined
template emits no out-of-line code, so nothing reaches the map even though the
type is original -- BitArray and SMBuilder both come back empty here and are
both original. Confirm with Tools/pdb_find_type.py before concluding anything
from a miss.

Usage:
    python map_symbol_to_obj.py AmbushSM DrawGunSM HolsterGunSM
    python map_symbol_to_obj.py --map ears_godfather_r_mt.map CrimeManager
    python map_symbol_to_obj.py --file names.txt --quiet
"""

from __future__ import annotations

import argparse
import collections
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
DEFAULT_MAP = (Path(r"J:\Projects\RE Material\The Godfather II")
               / "ears_godfather_singleplayer" / "ears_godfather_d_mt.map")
DEFAULT_MANIFEST = REPO / "Reference" / "original_tree.txt"

# " 0001:000e236c       ?MAX_ACTIVE@BuildingManager@...@@0IB 820e276c   building_manager.obj"
#  field 0 = section:offset, 1 = symbol, 2 = rva, [f|i], -1 = lib:object
SYMBOL_LINE_RE = re.compile(r"^ [0-9a-f]{4}:[0-9a-f]{8}\s+(\S+)\s")


def load_map(path: Path) -> list[tuple[str, str]]:
    """Return [(symbol, object)] for every symbol line in the map."""
    entries: list[tuple[str, str]] = []
    with path.open("r", encoding="utf-8", errors="replace") as handle:
        for line in handle:
            if not SYMBOL_LINE_RE.match(line):
                continue
            fields = line.split()
            if len(fields) < 4:
                continue
            obj = fields[-1]
            if not obj.lower().endswith(".obj"):
                continue
            entries.append((fields[1], obj.rsplit(":", 1)[-1].lower()))
    return entries


def load_manifest(path: Path) -> dict[str, list[str]]:
    """Index manifest paths by source basename stem."""
    by_stem: dict[str, list[str]] = collections.defaultdict(list)
    if not path.is_file():
        return by_stem
    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.strip()
        if not line:
            continue
        stem = line.rsplit("/", 1)[-1].rsplit(".", 1)[0]
        by_stem[stem].append(line)
    return by_stem


def resolve(name: str, entries: list[tuple[str, str]],
            by_stem: dict[str, list[str]], quiet: bool) -> bool:
    """Print what name resolves to. Returns True if anything was found."""
    # Match the name as a complete mangled-name component, i.e. "@Name@" or a
    # leading "?Name@". Substring matching alone pulls in NPCSearchSMState etc.
    needle = name.lower()
    tally: collections.Counter[str] = collections.Counter()
    for symbol, obj in entries:
        lowered = symbol.lower()
        if f"@{needle}@" in lowered or lowered.startswith(f"?{needle}@"):
            tally[obj] += 1
    if not tally:
        if not quiet:
            print(f"{name}\n  (not in the map -- may still be an inlined template;"
                  f" check with pdb_find_type.py)\n")
        return False

    print(name)
    for obj, count in tally.most_common():
        print(f"  {count:4}x  {obj}")
        for src in sorted(by_stem.get(obj.rsplit('.', 1)[0], [])):
            print(f"         {src}")
    print()
    return True


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("names", nargs="*", help="class or symbol names to resolve")
    parser.add_argument("--map", type=Path, default=DEFAULT_MAP, help="linker map to read")
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST,
                        help="original_tree.txt, used to turn an object into a source path")
    parser.add_argument("--file", type=Path, help="read names from a file, one per line")
    parser.add_argument("--quiet", action="store_true",
                        help="only report names that resolved")
    args = parser.parse_args()

    names = list(args.names)
    if args.file:
        names += [l.strip() for l in args.file.read_text(encoding="utf-8").splitlines()
                  if l.strip() and not l.startswith("#")]
    if not names:
        parser.error("give at least one name, or --file")

    if not args.map.is_file():
        print(f"error: map not found: {args.map}", file=sys.stderr)
        return 1

    entries = load_map(args.map)
    by_stem = load_manifest(args.manifest)
    print(f"{args.map.name}: {len(entries)} symbols\n", file=sys.stderr)

    found = sum(resolve(n, entries, by_stem, args.quiet) for n in names)
    print(f"resolved {found}/{len(names)}", file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
