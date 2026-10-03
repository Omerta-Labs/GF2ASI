#!/usr/bin/env python3
"""Recover the original EARS source tree from the Xbox 360 build PDBs.

The retail PDBs still carry every translation unit's source path as a plain
string, rooted at the build machine's package directory:

    c:\\packages\\ears_godfather\\dev\\src\\modules\\families\\family.cpp

The version level ("dev", "release", "2.0") is a build-system artifact with no
bearing on source layout, so the normalised output drops it. What is left is
exactly the path each file should occupy under Source/SDK:

    ears_godfather/src/modules/families/family.cpp

Usage:
    python extract_pdb_paths.py                      # default RE material dir
    python extract_pdb_paths.py --re-material DIR
    python extract_pdb_paths.py --all-packages       # keep Havok, PlasmaSDK, ...
    python extract_pdb_paths.py --raw                # full untouched paths
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

DEFAULT_RE_MATERIAL = Path(r"J:\Projects\RE Material\The Godfather II")

DEFAULT_OUT = Path(__file__).resolve().parent.parent / "Reference" / "original_tree.txt"

SOURCE_EXT = (".cpp", ".hpp", ".inl", ".h", ".c")

# c:\packages\<pkg>\<ver>\<intra>  /  e:\dl\packages\...  /  x:\packages\...
PACKAGE_RE = re.compile(
    r"^[a-z]:/(?:dl/)?packages/([a-z0-9_]+)/([^/]+)/(.+)$"
)

# Packages in scope for reconstruction: the EARS engine, the RenderWare
# middleware it sits on, and the EA foundation packages their headers pull in
# (IAllocator and friends are referenced directly by SDK code). Everything else
# -- Havok, PlasmaSDK, the Xenon SDK, zlib, libcurl -- is third party and only
# adds noise, so --all-packages exists for when you need to look it up.
OWN_PACKAGE_PREFIXES = (
    "ears_",
    "rw",
    "cct",
    "apt",
    "tnl",
    "exposure",
    "job_",
    "allocator",
    "coreallocator",
    "eabase",
    "eastl",
    "eathread",
)

# Build output that happens to live under a package directory; not real source.
BUILD_ARTEFACT_RE = re.compile(r"(?:^|/)(?:build|bb_runtime)[./]")


def iter_strings(blob: bytes, min_len: int = 8):
    """Yield printable ASCII runs of at least min_len characters."""
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


def harvest(pdb: Path) -> set[str]:
    """Pull every source-file path out of one PDB, lowercased, slash-normalised."""
    found: set[str] = set()
    blob = pdb.read_bytes()
    for text in iter_strings(blob):
        if "\\" not in text and "/" not in text:
            continue
        lowered = text.lower()
        if not lowered.endswith(SOURCE_EXT):
            continue
        found.add(lowered.replace("\\", "/"))
    return found


def normalise(path: str, keep_all: bool) -> str | None:
    """c:/packages/<pkg>/<ver>/<intra>  ->  <pkg>/<intra>, or None to drop."""
    match = PACKAGE_RE.match(path)
    if match is None:
        return None
    package, _version, intra = match.groups()
    if not keep_all and not package.startswith(OWN_PACKAGE_PREFIXES):
        return None
    if BUILD_ARTEFACT_RE.search(intra):
        return None
    return f"{package}/{intra}"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--re-material", type=Path, default=DEFAULT_RE_MATERIAL,
                        help="directory holding the Xbox build output (searched recursively)")
    parser.add_argument("--out", type=Path, default=DEFAULT_OUT,
                        help="manifest to write")
    parser.add_argument("--all-packages", action="store_true",
                        help="keep third-party packages (Havok, PlasmaSDK, Xenon SDK, ...)")
    parser.add_argument("--raw", action="store_true",
                        help="emit the full original paths instead of the normalised form")
    args = parser.parse_args()

    if not args.re_material.is_dir():
        print(f"error: {args.re_material} is not a directory", file=sys.stderr)
        return 1

    pdbs = sorted(args.re_material.rglob("*.pdb"))
    if not pdbs:
        print(f"error: no .pdb files under {args.re_material}", file=sys.stderr)
        return 1

    raw: set[str] = set()
    for pdb in pdbs:
        before = len(raw)
        raw |= harvest(pdb)
        size_mb = pdb.stat().st_size / (1024 * 1024)
        print(f"  {pdb.name:34} {size_mb:6.0f} MB  +{len(raw) - before} new")

    if args.raw:
        lines = sorted(raw)
    else:
        lines = sorted({n for n in (normalise(p, args.all_packages) for p in raw) if n})

    args.out.parent.mkdir(parents=True, exist_ok=True)
    # Explicit newline so the manifest is LF on every platform; it is compared
    # against byte-for-byte by sdk_coverage.py.
    with args.out.open("w", encoding="utf-8", newline="\n") as handle:
        for line in lines:
            handle.write(line + "\n")

    print(f"\n{len(raw)} source paths across {len(pdbs)} PDBs")
    print(f"{len(lines)} written to {args.out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
