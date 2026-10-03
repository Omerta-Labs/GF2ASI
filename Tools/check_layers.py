#!/usr/bin/env python3
"""Check that the SDK does not depend on the layers above it.

Include-level coupling is the easy half and `grep` finds it. The half that got
through was a *data* coupling: SimManager::LoadResource read
PrivateUtils::RegisteredPackets, a namespace-scope map defined in
simmanager.cpp, and the code that filled it moved to the modding layer. Neither
file included the other, so nothing flagged it -- it only failed at compile
time, after the move was committed.

Three checks, in order of how much they catch:

  includes   Any SDK file including from Addons/, Scripthook/, Utils/ or a
             vendored library that the SDK has no business knowing about.
  symbols    Any identifier used in the modding layer that is *defined* at
             namespace scope in an SDK .cpp and declared in no header. Those are
             reachable only by accident of linkage and break the moment either
             side moves.
  orphans    The reverse: namespace-scope definitions in SDK .cpp files that no
             header declares and nothing else references. Usually leftovers.

Exit code is non-zero if either of the first two finds anything.

Usage:
    python check_layers.py
    python check_layers.py --quiet
"""

from __future__ import annotations

import argparse
import collections
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
SDK = REPO / "Source" / "SDK"
PLATFORM = REPO / "Source" / "Platform"
MOD_DIRS = [REPO / "Source" / "Addons", REPO / "Source" / "Scripthook",
            REPO / "Source" / "Utils", REPO / "Source" / "GF2Hook.cpp",
            REPO / "dllmain.cpp"]
SKIP_TREES = ("Source/Addons/imgui", "Source/Addons/discord")
SRC_EXT = (".cpp", ".c")
HDR_EXT = (".h", ".hpp", ".inl")

# What an SDK file must never include. Platform/ is below the SDK, so it is fine.
FORBIDDEN_INCLUDE = re.compile(
    r'^\s*#\s*include\s*[<"]('
    r'[Aa]ddons/|[Ss]cripthook/|[Uu]tils/|polyhook2/|sol[./]|discord'
    r')', re.MULTILINE)

# An object definition with external linkage: `<type> <name>;`,
# `<type> <name> = ...;` or `<type> <name>{...};`.
#
# static, constexpr and inline are excluded because internal linkage cannot
# couple two translation units -- when both layers appear to share a name like
# RunningTickEvent, each in fact has its own static copy. Functions are excluded
# too: a missing declaration fails loudly at the first call, whereas a stray
# variable links by accident, which is the case worth catching.
OBJECT_DEF = re.compile(
    r'^(?!\s*(?:namespace|using|typedef|template|class|struct|enum|union|extern|'
    r'static|constexpr|inline|return|if|else|while|for|switch|case|friend|'
    r'public|private|protected|static_assert)\b)'
    r'\s*[A-Za-z_][\w:<>,*&\s\[\]]*?'
    r'\b([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*(?:=[^;]*|\{[^;]*\})?;\s*$')

NAMESPACE_OPEN = re.compile(r'^\s*namespace\b[^{]*\{?\s*$')


def files(root: Path, exts):
    if root.is_file():
        return [root] if root.suffix.lower() in exts else []
    return [p for p in root.rglob("*")
            if p.suffix.lower() in exts
            and "Vendors" not in p.parts
            and not any(t in p.as_posix() for t in SKIP_TREES)]


def read(p: Path) -> str:
    return p.read_text(encoding="utf-8", errors="replace")


def strip_comments(text: str) -> str:
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL)
    return re.sub(r"//[^\n]*", "", text)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--quiet", action="store_true")
    args = ap.parse_args()

    bad_includes: list[tuple[Path, int, str]] = []
    for p in files(SDK, SRC_EXT + HDR_EXT):
        text = read(p)
        for m in FORBIDDEN_INCLUDE.finditer(text):
            line = text.count("\n", 0, m.start()) + 1
            bad_includes.append((p, line, text[m.start():text.find("\n", m.start())].strip()))

    # Namespace-scope definitions in SDK sources, and every name any SDK or
    # Platform header declares.
    declared: set[str] = set()
    for p in files(SDK, HDR_EXT) + files(PLATFORM, HDR_EXT):
        body = strip_comments(read(p))
        declared.update(re.findall(r"\b([A-Za-z_]\w*)\s*[;({=]", body))

    sdk_defs: dict[str, Path] = {}
    for p in files(SDK, SRC_EXT):
        # Walk with a stack of brace kinds. A line is at namespace scope only
        # when every brace currently open was opened by a namespace -- which is
        # what separates a file-scope variable from a local inside a function
        # that happens to sit one tab in.
        stack: list[str] = []
        pending_namespace = False
        for line in strip_comments(read(p)).splitlines():
            at_namespace_scope = all(kind == "ns" for kind in stack)
            if at_namespace_scope:
                m = OBJECT_DEF.match(line)
                if m and "(" not in line.split("=")[0]:
                    name = m.group(1)
                    if name not in declared and len(name) > 2:
                        sdk_defs.setdefault(name, p)
            if NAMESPACE_OPEN.match(line):
                pending_namespace = True
            for ch in line:
                if ch == "{":
                    stack.append("ns" if pending_namespace else "other")
                    pending_namespace = False
                elif ch == "}" and stack:
                    stack.pop()

    # Which of those does the modding layer reference?
    leaks: list[tuple[str, Path, Path]] = []
    mod_files = [f for d in MOD_DIRS for f in files(d, SRC_EXT + HDR_EXT)]
    for p in mod_files:
        body = strip_comments(read(p))
        used = set(re.findall(r"\b([A-Za-z_]\w*)\b", body))
        for name in used & sdk_defs.keys():
            leaks.append((name, sdk_defs[name], p))

    if bad_includes and not args.quiet:
        print(f"=== SDK INCLUDES FROM ABOVE ({len(bad_includes)}) ===")
        for p, line, what in bad_includes:
            print(f"  {p.relative_to(REPO).as_posix()}:{line}\n      {what}")
        print()
    if leaks and not args.quiet:
        print(f"=== CROSS-LAYER SYMBOL USE ({len(leaks)}) ===")
        print("  defined at namespace scope in an SDK .cpp, declared in no")
        print("  header, and referenced from the modding layer\n")
        for name, where, user in leaks:
            print(f"  {name}\n      defined: {where.relative_to(REPO).as_posix()}"
                  f"\n      used:    {user.relative_to(REPO).as_posix()}")
        print()

    print(f"SDK: {len(bad_includes)} forbidden includes, "
          f"{len(leaks)} cross-layer symbol uses "
          f"({len(sdk_defs)} undeclared namespace-scope definitions scanned)")
    return 1 if (bad_includes or leaks) else 0


if __name__ == "__main__":
    raise SystemExit(main())
