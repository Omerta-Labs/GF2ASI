#!/usr/bin/env python3
"""Ask the PDBs whether a type existed in the original build.

This is the check map_symbol_to_obj.py cannot make. A fully inlined template
emits no out-of-line code, so no symbol for it ever reaches the linker map --
yet the type is unquestionably original. BitArray<0x10000, unsigned int> and
EARS::Common::SingleInternalLinkedListLightweight<RWS::CLinkedMsg> are both in
this category. "Absent from the map" therefore does not mean "ours"; only
"absent from the PDBs as well" does.

Reports every distinct mangled name containing the type, which also gives its
namespace and, for a template, the arguments it was instantiated with.

    $ python pdb_find_type.py BitArray
    BitArray -- 8 mangled names, e.g.
      ??0?$BitArray@$0BAAA@I@@QAA@XZ          -> BitArray<0x10000,unsigned int>::ctor
      ?Clear@?$BitArray@$0BAAA@I@@QAAXI@Z

Usage:
    python pdb_find_type.py BitArray StaticBitArray
    python pdb_find_type.py --all RwMatrixTag     # every name, not a sample
    python pdb_find_type.py --exclude d3dx,xgraphics Manager
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

DEFAULT_RE_MATERIAL = Path(r"J:\Projects\RE Material\The Godfather II")
# One debug PDB carries essentially the whole type stream; scanning all nine
# costs ~1 GB of reads for a handful of extra names.
DEFAULT_PDB_GLOB = "ears_godfather_singleplayer/ears_godfather_d_mt.pdb"

MANGLED_CHARS = r"A-Za-z0-9_@?$"


def iter_strings(blob: bytes, min_len: int = 4):
    current = bytearray()
    for byte in blob:
        if 0x20 <= byte <= 0x7E:
            current.append(byte)
        else:
            if len(current) >= min_len:
                yield current.decode("ascii")
            current.clear()
    if len(current) >= min_len:
        yield current.decode("ascii")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("types", nargs="+", help="type names to look for")
    parser.add_argument("--re-material", type=Path, default=DEFAULT_RE_MATERIAL)
    parser.add_argument("--pdb", type=Path,
                        help="a specific PDB (default: the singleplayer debug build)")
    parser.add_argument("--all", action="store_true", help="print every name, not a sample")
    parser.add_argument("--exclude", default="d3dx,xgraphics,plasma",
                        help="comma-separated substrings to drop (third-party noise)")
    args = parser.parse_args()

    pdb = args.pdb or (args.re_material / DEFAULT_PDB_GLOB)
    if not pdb.is_file():
        print(f"error: PDB not found: {pdb}", file=sys.stderr)
        return 1
    drop = [s.strip().lower() for s in args.exclude.split(",") if s.strip()]

    patterns = {t: re.compile(f"[{MANGLED_CHARS}]*{re.escape(t)}[{MANGLED_CHARS}]*")
                for t in args.types}
    hits: dict[str, set[str]] = {t: set() for t in args.types}
    bare: dict[str, int] = {t: 0 for t in args.types}

    print(f"scanning {pdb.name} ({pdb.stat().st_size / 1024 / 1024:.0f} MB)",
          file=sys.stderr)
    for text in iter_strings(pdb.read_bytes()):
        for name, pattern in patterns.items():
            if name not in text:
                continue
            for match in pattern.findall(text):
                if any(d in match.lower() for d in drop):
                    continue
                if match == name:
                    bare[name] += 1
                elif "?" in match or "@" in match:
                    hits[name].add(match)

    exit_code = 0
    for name in args.types:
        mangled = sorted(hits[name])
        if mangled:
            print(f"\n{name} -- {len(mangled)} mangled name(s)"
                  f"{'' if args.all else ', sample'}:")
            for item in (mangled if args.all else mangled[:6]):
                print(f"  {item}")
        elif bare[name]:
            print(f"\n{name} -- present as a bare type name ({bare[name]}x) but no "
                  f"mangled name.\n  Original, and never instantiated out of line.")
        else:
            print(f"\n{name} -- absent from this PDB. Ours, not a renamed original.")
            exit_code = 1
    return exit_code


if __name__ == "__main__":
    raise SystemExit(main())
