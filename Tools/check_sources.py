#!/usr/bin/env python3
"""Structural sanity checks on our sources: brace balance and duplicate includes.

These exist because the Phase 2d-ii merges shipped broken code and the
verification at the time could not see it. That check compared every meaningful
line of each source against the merged result and reported "all source lines
accounted for" -- while skipping `{` and `}` lines as noise. The merge had
stripped the closing brace off the last function in five of six files, which is
precisely a `}` line, so the check was blind to the one thing that went wrong.

What it looks for:

  braces      Unbalanced {} per file, counted outside comments and string and
              character literals. Catches a dropped closer, which the compiler
              reports hundreds of lines later or in the next file entirely.
  duplicates  The same #include twice in one file. Harmless with #pragma once,
              but a reliable sign a merge or a rewrite ran twice.
  namespace   A closing brace carrying a trailing comment (`} // CCT`) is still
              a closing brace. Reported only as a note, since a merge tool that
              matches on `}` alone will silently append outside the namespace.

Exit code is non-zero if any file is unbalanced, so this works in a hook.

Usage:
    python check_sources.py
    python check_sources.py --path Source/SDK/ears_rt_cct
"""

from __future__ import annotations

import argparse
import collections
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
SCAN_EXT = (".cpp", ".c", ".h", ".hpp", ".inl")
SKIP_TREES = ("Source/Addons/imgui", "Source/Addons/discord")

INCLUDE_RE = re.compile(r'^\s*#\s*include\s*([<"][^">]+[">])', re.MULTILINE)
CLOSER_WITH_COMMENT = re.compile(r"^\s*\}\s*(?://|/\*)")


def brace_delta(text: str) -> int:
    """{ minus }, ignoring comments and literals."""
    depth = 0
    i, n = 0, len(text)
    in_line_comment = in_block_comment = False
    in_string = in_char = False
    while i < n:
        c = text[i]
        nxt = text[i + 1] if i + 1 < n else ""
        if in_line_comment:
            if c == "\n":
                in_line_comment = False
        elif in_block_comment:
            if c == "*" and nxt == "/":
                in_block_comment = False
                i += 1
        elif in_string:
            if c == "\\":
                i += 1
            elif c == '"':
                in_string = False
        elif in_char:
            if c == "\\":
                i += 1
            elif c == "'":
                in_char = False
        else:
            if c == "/" and nxt == "/":
                in_line_comment = True
                i += 1
            elif c == "/" and nxt == "*":
                in_block_comment = True
                i += 1
            elif c == '"':
                in_string = True
            elif c == "'":
                in_char = True
            elif c == "{":
                depth += 1
            elif c == "}":
                depth -= 1
        i += 1
    return depth


def show(p: Path, base: Path) -> str:
    """Display path, tolerating a --path outside the repo (archived commits)."""
    for root in (REPO, base):
        try:
            return p.relative_to(root).as_posix()
        except ValueError:
            continue
    return p.as_posix()


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--path", type=Path, default=REPO / "Source")
    ap.add_argument("--quiet", action="store_true")
    args = ap.parse_args()

    files = [p for p in args.path.rglob("*")
             if p.suffix.lower() in SCAN_EXT
             and "Vendors" not in p.parts
             and not any(t in p.as_posix() for t in SKIP_TREES)]

    unbalanced, duplicated, commented_closers = [], [], 0
    for p in files:
        text = p.read_text(encoding="utf-8", errors="replace")
        d = brace_delta(text)
        if d != 0:
            unbalanced.append((p, d))
        seen = collections.Counter(m.group(1) for m in INCLUDE_RE.finditer(text))
        dupes = [inc for inc, c in seen.items() if c > 1]
        if dupes:
            duplicated.append((p, dupes))
        commented_closers += len(CLOSER_WITH_COMMENT.findall(text))

    if unbalanced and not args.quiet:
        print(f"=== UNBALANCED BRACES ({len(unbalanced)}) ===")
        for p, d in unbalanced:
            what = f"{d} unclosed {{" if d > 0 else f"{-d} extra }}"
            print(f"  {show(p, args.path)}: {what}")
        print()
    if duplicated and not args.quiet:
        print(f"=== DUPLICATE INCLUDES ({len(duplicated)}) ===")
        for p, dupes in duplicated:
            print(f"  {show(p, args.path)}: {', '.join(dupes)}")
        print()

    print(f"{len(files)} files: {len(unbalanced)} unbalanced, "
          f"{len(duplicated)} with duplicate includes")
    print(f"note: {commented_closers} closing braces carry a trailing comment "
          f"(`}} // Namespace`); a merge tool matching bare `}}` will miss them")
    return 1 if unbalanced else 0


if __name__ == "__main__":
    raise SystemExit(main())
