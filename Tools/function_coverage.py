#!/usr/bin/env python3
"""Per-function reconstruction coverage, cross-referenced against the linker map.

sdk_coverage.py counts files. That says nothing about how much of a file is
done: family.cpp exists, but the original has 384 symbols in family.obj and we
may have twenty of them. This answers the useful question instead -- which
functions the original had, and which of them we have written.

The expected set comes from the map: every .text symbol in an EARS or
RenderWare object, with the class and function name pulled out of the mangled
name. The done set comes from parsing our own sources -- the matching .cpp and
its paired header, so our own inline definitions count. Both sides key on
(class, function), which is robust against our parameter types or return type
differing from the original.

Output is CSV on stdout, or a summary with --summary:

    original_file,object,class,scope_kind,function,kind,x360_address,status,
    emitted_in,mangled

scope_kind separates a real class from a namespace holding free functions:
EARS::Alchemy has 579 "members", all of them free functions across many files.

emitted_in is the number of objects the symbol was found in. Anything above 1
was inline in the original and emitted into every object that used it, so it has
no owning file -- original_file names one of them and means little.

Two things the map cannot tell us, which bound what any number here can mean:

  - A function the original inlined at every call site emits no out-of-line code
    anywhere, so it is absent from the map entirely. It is in neither the
    numerator nor the denominator: writing it scores nothing, and not writing it
    is not counted as missing. --summary reports the size of that blind spot as
    "written, no symbol in the map".
  - Addresses are Xbox 360 (0x82xxxxxx). The PC build the project targets has
    different ones, so treat them as a cross-reference for locating the PC
    equivalent, not as something to paste into a thunk.

Compiler-generated symbols (vtables, RTTI descriptors, string literals, thunks)
are counted separately under --summary and excluded from the function tallies.

Usage:
    python function_coverage.py --summary
    python function_coverage.py > coverage.csv
    python function_coverage.py --package ears_godfather --summary
    python function_coverage.py --file modules/families/family.cpp
"""

from __future__ import annotations

import argparse
import collections
import csv
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
SDK = REPO / "Source" / "SDK"
DEFAULT_MANIFEST = REPO / "Reference" / "original_tree.txt"
DEFAULT_MAP = (Path(r"J:\Projects\RE Material\The Godfather II")
               / "ears_godfather_singleplayer" / "ears_godfather_d_mt.map")

MAP_LINE = re.compile(r"^ ([0-9a-f]{4}):([0-9a-f]{8})\s+(\S+)\s+([0-9a-f]{8})")
TEXT_SECTION = "0003"

# A free function in a namespace mangles as ?Name@Scope@@Y<cc>, where a member
# carries an access code instead. Without this, every namespace holding free
# functions reads as an enormous class.
FREE_FN = re.compile(r"@@Y[A-Z]")

# Library names in the map are <package>_d_mt, with these exceptions. The
# library is the only thing that distinguishes assert.obj in ears_framework from
# assert.obj in ears_common -- the manifest has four files called assert.cpp,
# and 33 object stems are ambiguous that way. Objects with no library were
# compiled straight into the image, which is the game project itself.
LIB_ALIAS = {"cct": "ears_rt_cct"}
MAIN_IMAGE_PACKAGE = "ears_godfather"

# Third-party libraries in the map; everything else is ours to reconstruct.
THIRD_PARTY = ("xgraphics", "plasmasdk", "d3dx9", "d3d9", "hkp", "hkb", "hks",
               "hkvisualize", "hkscenedata", "hkbase", "libcmt", "libcpmt",
               "xaudio", "rwaudio", "xapilib", "dirtysock", "lua_", "eathread",
               "vp6", "xnet", "xonline", "csisfw", "xceedzip", "realmemcard",
               "ppmalloc", "job_manager", "xhv", "xmp", "apt_d_mt", "tnl_d_mt",
               "exposure", "rwmovie", "rwstdc", "eastl")

# Mangled-name prefixes that are not functions we would ever write by hand.
GENERATED = {
    "??_7": "vftable", "??_8": "vbtable", "??_R": "rtti", "??_C": "string",
    "??_E": "vector-dtor", "??_G": "scalar-dtor", "??_F": "default-ctor-closure",
    "??_I": "vector-ctor-iter", "??_L": "eh-vector-ctor", "??_M": "eh-vector-dtor",
    "??_9": "vcall-thunk", "??_B": "local-static-guard", "??_P": "udt-returning",
}

# Our own definitions: "void Foo::Bar(", "Foo::Foo(", "Foo::~Foo(".
# Leading whitespace is required, not optional: every definition in this project
# sits inside `namespace EARS::Modules {` and so is indented. Anchoring at
# column 0 reports 0 done for every file.
# The namespace prefix is skipped lazily, so a fully qualified definition --
# `void EARS::Modules::Family::AddMedic(...)`, which is how family.cpp writes
# them -- yields Family, not EARS.
DEFN_RE = re.compile(r"^[ \t]*(?:[\w<>,*&\[\]]+[ \t]+)*?"
                     r"(?:[A-Za-z_]\w*::)*?([A-Za-z_]\w*)::(~?[A-Za-z_]\w*)[ \t]*\(",
                     re.MULTILINE)
# Telling a definition from a call. A definition's parameter list is followed by
# `{`, or by `:` for a constructor initialiser list, often on the next line. A
# call ends in `;` or sits inside an expression. This matters in both
# directions: `void NPC::Foo(uint32_t a)` ends in `)` exactly as a call does,
# and a body full of MemUtils::CallClassMethod would otherwise read as dozens of
# reconstructed functions.
STATEMENT_START = re.compile(r"^\s*(?:return|if|else|while|for|switch|case)\b")
# In-class declarations, paired with the enclosing class.
CLASS_RE = re.compile(r"^\s*(?:class|struct)\s+([A-Za-z_]\w*)")
MEMBER_RE = re.compile(r"^\s+(?:[\w:<>,*&\[\]]+\s+)*?(~?[A-Za-z_]\w*)\s*\([^;]*\)\s*"
                       r"(?:const\s*)?(?:override\s*)?(?:=\s*0\s*)?[;{]")


def parse_mangled(sym: str) -> tuple[str, str, str] | None:
    """-> (class path, function, kind). None if not a named member/free function."""
    for prefix, kind in GENERATED.items():
        if sym.startswith(prefix):
            return (None, None, kind)
    if not sym.startswith("?"):
        return ("", sym, "c-linkage")          # plain C symbol
    body = sym[1:]
    kind = "func"
    if body.startswith("?0"):
        body, kind = body[2:], "ctor"
    elif body.startswith("?1"):
        body, kind = body[2:], "dtor"
    elif body.startswith("?$") or body.startswith("?_"):
        return (None, None, "template-or-special")
    elif body.startswith("?"):
        return (None, None, "operator-or-special")
    if "@@" not in body:
        return None
    qual = body.split("@@", 1)[0]
    if "?$" in qual or "@?" in qual:
        return (None, None, "template")
    parts = [p for p in qual.split("@") if p]
    if not parts:
        return None
    if kind in ("ctor", "dtor"):
        cls = "::".join(reversed(parts))
        return (cls, parts[0], kind)
    if len(parts) == 1:
        return ("", parts[0], "free")
    return ("::".join(reversed(parts[1:])), parts[0], kind)


def package_for_lib(lib: str) -> str:
    """-> the manifest package an object's library corresponds to."""
    if not lib:
        return MAIN_IMAGE_PACKAGE
    name = lib[:-len("_d_mt")] if lib.endswith("_d_mt") else lib
    return LIB_ALIAS.get(name, name)


def load_map(path: Path):
    """-> {(library, object): [(address, mangled)]} for .text symbols of ours."""
    objects: dict[tuple[str, str], list[tuple[str, str]]] = collections.defaultdict(list)
    for line in path.open(encoding="utf-8", errors="replace"):
        m = MAP_LINE.match(line)
        if not m or m.group(1) != TEXT_SECTION:
            continue
        last = line.split()[-1]
        if not last.lower().endswith(".obj"):
            continue
        lib = last.rsplit(":", 1)[0].lower() if ":" in last else ""
        if lib and any(t in lib for t in THIRD_PARTY):
            continue
        obj = last.rsplit(":", 1)[-1].lower()
        if obj.startswith("bb_runtime."):
            continue                            # RenderWare unity builds
        objects[(lib, obj)].append((m.group(4), m.group(3)))
    return objects


def ours(paths: list[Path]) -> tuple[set[tuple[str, str]], dict[tuple[str, str], str]]:
    """-> (lowercased (class, function) pairs, lowered key -> our exact spelling)."""
    found: set[tuple[str, str]] = set()
    spelling: dict[tuple[str, str], str] = {}
    for p in paths:
        if not p.is_file():
            continue
        text = p.read_text(encoding="utf-8", errors="replace")
        lines = text.split("\n")
        for i, line in enumerate(lines):
            m = DEFN_RE.match(line)
            if not m or STATEMENT_START.match(line):
                continue
            stripped = line.rstrip()
            if stripped.endswith(";"):
                continue                        # a declaration or a call
            nxt = next((l.strip() for l in lines[i + 1:] if l.strip()), "")
            if not (stripped.endswith("{") or nxt[:1] in ("{", ":")):
                continue                        # no body follows: a call
            key = (m.group(1).lower(), m.group(2).lstrip("~").lower())
            found.add(key)
            spelling[key] = m.group(2).lstrip("~")
        # In-class declarations, tracked with a brace-depth stack rather than a
        # single "current class". Family declares nested struct OmertaEntry and
        # struct Decision partway through, so a single variable hands every one
        # of Family's 177 members to whichever nested type was seen last.
        stack: list[tuple[str, int]] = []
        pending: str | None = None
        depth = 0
        for line in lines:
            c = CLASS_RE.match(line)
            if c and not line.rstrip().endswith(";"):
                pending = c.group(1)              # not a forward declaration
            if "{" in line and pending:
                stack.append((pending, depth))
                pending = None
            depth += line.count("{") - line.count("}")
            while stack and depth <= stack[-1][1]:
                stack.pop()
            if stack:
                m = MEMBER_RE.match(line)
                if m:
                    key = (stack[-1][0].lower(), m.group(1).lstrip("~").lower())
                    found.add(key)
                    spelling.setdefault(key, m.group(1).lstrip("~"))
    return found, spelling


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--map", type=Path, default=DEFAULT_MAP)
    ap.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    ap.add_argument("--package", help="restrict to one package")
    ap.add_argument("--file", help="restrict to one original path within a package")
    ap.add_argument("--summary", action="store_true", help="per-package and worst-gap report")
    ap.add_argument("--limit", type=int, default=25, help="rows in the --summary tables")
    args = ap.parse_args()

    if not args.map.is_file():
        print(f"error: map not found: {args.map}", file=sys.stderr)
        return 1

    # object basename -> the manifest paths that could have produced it
    by_stem: dict[str, list[str]] = collections.defaultdict(list)
    for line in args.manifest.read_text(encoding="utf-8").splitlines():
        line = line.strip()
        if not line:
            continue
        stem = line.rsplit("/", 1)[-1].rsplit(".", 1)[0]
        by_stem[stem].append(line)

    objects = load_map(args.map)
    rows = []
    written: set[tuple[str, str]] = set()      # every (class, function) we define
    expected: set[tuple[str, str]] = set()     # every (class, function) in the map
    miscased: list[tuple[str, str, str, str]] = []
    generated = collections.Counter()
    unparsed = 0

    for (lib, obj), syms in sorted(objects.items()):
        stem = obj.rsplit(".", 1)[0]
        candidates = by_stem.get(stem, [])
        # Narrow to the package the library names before picking a file, or an
        # ambiguous stem lands on whichever manifest line happened to come
        # first. Falls back to the unscoped list for libraries with no matching
        # package, such as renderwarestubs.
        scoped = [c for c in candidates
                  if c.split("/", 1)[0] == package_for_lib(lib)]
        candidates = scoped or candidates
        src = next((c for c in candidates if c.endswith(".cpp")), None) \
            or next((c for c in candidates if c.endswith(".c")), None) \
            or (candidates[0] if candidates else None)
        if src is None:
            continue
        pkg = src.split("/", 1)[0]
        intra = src.split("/", 1)[1]
        if args.package and pkg != args.package:
            continue
        if args.file and args.file not in intra:
            continue

        header = next((c for c in candidates if c.endswith(".h")), None)
        have, spelling = ours([SDK / src] + ([SDK / header] if header else []))
        exists = (SDK / src).is_file()
        written.update(have)

        for addr, sym in syms:
            parsed = parse_mangled(sym)
            if parsed is None:
                unparsed += 1
                continue
            cls, fn, kind = parsed
            if cls is None:
                generated[kind] += 1
                continue
            simple = cls.rsplit("::", 1)[-1]
            # Compared case-insensitively on purpose: our reconstructions do not
            # always match the original's acronym casing -- the original has
            # NPC::ActivateHudIndicator where we wrote ActivateHUDIndicator --
            # and that is a naming difference, not a missing function.
            key = (simple.lower(), fn.lower())
            expected.add(key)
            done = key in have or ("", fn.lower()) in have
            if done:
                mine = spelling.get(key) or spelling.get(("", fn.lower()))
                if mine and mine != fn:
                    miscased.append((src, cls, fn, mine))
            rows.append({
                "original_file": src, "object": obj, "class": cls,
                "scope_kind": "global" if not cls else
                              ("namespace" if FREE_FN.search(sym) else "class"),
                "function": fn, "kind": kind, "x360_address": "0x" + addr,
                "status": "done" if done else ("missing" if exists else "no-file"),
                "emitted_in": 1, "mangled": sym,
            })

    # One row per symbol. A function the original defined inline is emitted into
    # every object that used it -- __lvx lands in 260 of them -- so without this
    # a single function reads as missing in a hundred files at once, and the
    # totals count it a hundred times. Keep the row with the best status, so
    # work already done is not hidden behind an arbitrary file choice.
    rank = {"done": 0, "missing": 1, "no-file": 2}
    best: dict[str, dict] = {}
    emitted = collections.Counter()
    for r in rows:
        emitted[r["mangled"]] += 1
        prev = best.get(r["mangled"])
        if prev is None or rank[r["status"]] < rank[prev["status"]]:
            best[r["mangled"]] = r
    for sym, r in best.items():
        r["emitted_in"] = emitted[sym]
    inlined_everywhere = sum(1 for r in best.values() if r["emitted_in"] > 1)
    collapsed = len(rows) - len(best)
    rows = sorted(best.values(),
                  key=lambda r: (r["original_file"], r["class"], r["function"]))

    if not args.summary:
        w = csv.DictWriter(sys.stdout, fieldnames=list(rows[0].keys()) if rows else
                           ["original_file","object","class","scope_kind","function",
                            "kind","x360_address","status","emitted_in","mangled"],
                           lineterminator="\n")
        w.writeheader()
        w.writerows(rows)
        print(f"{len(rows)} functions", file=sys.stderr)
        return 0

    per_pkg = collections.defaultdict(lambda: collections.Counter())
    per_file = collections.defaultdict(lambda: collections.Counter())
    for r in rows:
        pkg = r["original_file"].split("/", 1)[0]
        per_pkg[pkg][r["status"]] += 1
        per_file[r["original_file"]][r["status"]] += 1

    print(f"{'package':20} {'done':>7} {'missing':>8} {'no-file':>8} {'total':>8} {'%':>6}")
    td = tm = tn = 0
    for pkg in sorted(per_pkg):
        c = per_pkg[pkg]
        d, m, n = c["done"], c["missing"], c["no-file"]
        td += d; tm += m; tn += n
        tot = d + m + n
        print(f"{pkg:20} {d:7} {m:8} {n:8} {tot:8} {100.0*d/tot if tot else 0:5.1f}%")
    tot = td + tm + tn
    print(f"{'TOTAL':20} {td:7} {tm:8} {tn:8} {tot:8} {100.0*td/tot if tot else 0:5.1f}%")

    started = {f: c for f, c in per_file.items() if c["done"] or c["missing"]}
    print(f"\nFiles we have started, by functions still missing "
          f"(top {args.limit}):")
    print(f"  {'missing':>7} {'done':>5}  file")
    for f, c in sorted(started.items(), key=lambda kv: -kv[1]["missing"])[:args.limit]:
        print(f"  {c['missing']:7} {c['done']:5}  {f}")

    by_scope = collections.Counter(r["scope_kind"] for r in rows)
    print(f"\nscope: {by_scope['class']} class members, "
          f"{by_scope['namespace']} namespace-scope free functions, "
          f"{by_scope['global']} global")

    print(f"inline in the original, emitted into more than one object: "
          f"{inlined_everywhere} symbols ({collapsed} duplicate rows collapsed) "
          f"-- their original_file is nominal")

    # Functions we define that no symbol in the map accounts for. Two causes,
    # which the map cannot separate: the original inlined it at every call site
    # so no out-of-line copy exists anywhere, or it is ours and never existed.
    # Either way it is invisible to the percentages above.
    print(f"written, no symbol in the map: {len(written - expected)} "
          f"(inlined away in the original, or ours)")

    print(f"\ncompiler-generated symbols skipped: "
          f"{', '.join(f'{k}={v}' for k, v in generated.most_common(6))}")
    print(f"mangled names not parsed: {unparsed}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
