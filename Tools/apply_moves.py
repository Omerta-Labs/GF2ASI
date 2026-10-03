#!/usr/bin/env python3
"""Carry out a move list from Tools/plan_moves.py, and fix the includes.

Three things have to happen together or the tree does not build:

  1. Every "SDK/<old path>" include directive becomes "SDK/<new path>". Matched
     case-insensitively, so a directive that was already miscased is corrected
     rather than missed.
  2. Bare sibling includes that the move separates are rewritten to the full
     "SDK/<new path>" form. Splitting a package into include/ and src/ does
     exactly this: Guid.cpp says #include "Guid.h", and the header lands in
     include/ears_common/ while the source goes to src/.
  3. The files move, with git recording renames so history follows them.

Case-only renames need care. Git on Windows defaults to core.ignorecase=true,
and "EARS_Common" and "ears_common" are the same directory to the filesystem,
so files are staged through Source/SDK/__restructure/ and the emptied originals
are removed before the staged tree is moved up. Never rename a package
directory in place.

Uncommitted work is preserved. For a file with unstaged changes, the index entry
is rebuilt as HEAD's content plus this script's rewrites, so a commit carries the
mechanical change and leaves the rest in the working tree. Without this, staging
a moved file would sweep up whatever was in progress.

Usage:
    python apply_moves.py moves_2b.tsv --dry-run
    python apply_moves.py moves_2b.tsv
"""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
SDK_REL = "Source/SDK"
SDK = REPO / SDK_REL
STAGE = SDK / "__restructure"
SCAN_EXT = (".cpp", ".c", ".h", ".hpp", ".inl")
SKIP_TREES = ("Source/Addons/imgui", "Source/Addons/discord")

BARE_INCLUDE_RE = re.compile(r'(^[ \t]*#[ \t]*include[ \t]*")([^"/]+\.(?:h|hpp|inl))(")',
                             re.MULTILINE)


def git(*args: str, check: bool = True) -> str:
    done = subprocess.run(("git",) + args, cwd=REPO, capture_output=True, text=True)
    if check and done.returncode != 0:
        raise RuntimeError(f"git {' '.join(args)}\n{done.stderr.strip()}")
    return done.stdout


def load_moves(tsv: Path) -> list[tuple[str, str]]:
    moves = []
    for line in tsv.read_text(encoding="utf-8").splitlines():
        if line.strip() and "\t" in line:
            old, new = line.split("\t", 1)
            moves.append((old.strip(), new.strip()))
    return moves


def scan_files() -> list[Path]:
    files = [p for p in (REPO / "Source").rglob("*")
             if p.suffix.lower() in SCAN_EXT and "Vendors" not in p.parts
             and not any(t in p.as_posix() for t in SKIP_TREES)]
    files.append(REPO / "dllmain.cpp")
    return files


def rewrite(text: str, path_rel: str, mv: dict[str, str],
            by_dir: dict[tuple[str, str], str]) -> str:
    """Apply both include rewrites to one file's text."""
    # 1. SDK/<old> -> SDK/<new>, longest first so no prefix shadows another
    for old in sorted(mv, key=len, reverse=True):
        pattern = re.compile(r"(?<=[\"<])SDK/" + re.escape(old) + r"(?=[\">])",
                             re.IGNORECASE)
        text = pattern.sub(f"SDK/{mv[old]}", text)

    # 2. bare sibling includes the move separates
    if path_rel.startswith(SDK_REL + "/"):
        within = path_rel[len(SDK_REL) + 1:]
        my_old_dir = within.rsplit("/", 1)[0] if "/" in within else ""
        my_new_dir = mv.get(within, within)
        my_new_dir = my_new_dir.rsplit("/", 1)[0] if "/" in my_new_dir else ""

        def fix_bare(match: re.Match[str]) -> str:
            target = match.group(2)
            sibling_new = by_dir.get((my_old_dir, target.lower()))
            if sibling_new is None:
                return match.group(0)
            sib_dir = sibling_new.rsplit("/", 1)[0] if "/" in sibling_new else ""
            if sib_dir == my_new_dir:
                return match.group(0)        # still siblings, leave it
            return f"{match.group(1)}SDK/{sibling_new}{match.group(3)}"

        text = BARE_INCLUDE_RE.sub(fix_bare, text)
    return text


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("tsv", type=Path)
    parser.add_argument("--dry-run", action="store_true")
    args = parser.parse_args()

    moves = load_moves(args.tsv)
    mv = dict(moves)
    # (old dir within SDK, lowercased basename) -> new path, for bare includes
    by_dir: dict[tuple[str, str], str] = {}
    for old, new in moves:
        d = old.rsplit("/", 1)[0] if "/" in old else ""
        by_dir[(d, old.rsplit("/", 1)[-1].lower())] = new

    # Files whose working tree differs from the index: their work is not ours to
    # stage, so remember them before anything moves.
    dirty = {line[3:].strip().strip('"')
             for line in git("status", "--porcelain").splitlines()
             if line[:2] in (" M", "MM", "AM")}
    if dirty:
        print("uncommitted work, will be preserved:")
        for d in sorted(dirty):
            print(f"  {d}")
        print()

    # --- 1 + 2. includes -------------------------------------------------
    touched = 0
    plan_index: list[tuple[str, str]] = []      # (path at HEAD, path now) for dirty files
    for path in scan_files():
        rel = path.relative_to(REPO).as_posix()
        original = path.read_text(encoding="utf-8", errors="surrogateescape")
        updated = rewrite(original, rel, mv, by_dir)
        if updated == original:
            continue
        touched += 1
        if args.dry_run:
            continue
        path.write_text(updated, encoding="utf-8", errors="surrogateescape", newline="")
        if rel in dirty:
            plan_index.append((rel, rel))
    print(f"includes rewritten in {touched} files")

    if args.dry_run:
        print(f"dry run: {len(moves)} moves not performed")
        return 0

    # --- 3. moves --------------------------------------------------------
    STAGE.mkdir(parents=True, exist_ok=True)
    for old, new in moves:
        dest = STAGE / new
        dest.parent.mkdir(parents=True, exist_ok=True)
        git("mv", "-f", f"{SDK_REL}/{old}", f"{SDK_REL}/__restructure/{new}")
    print(f"{len(moves)} files staged under __restructure/")

    # Remove the emptied originals, deepest first, so the case-only package
    # rename that follows has nothing to collide with.
    for d in sorted((p for p in SDK.rglob("*") if p.is_dir() and STAGE not in p.parents
                     and p != STAGE),
                    key=lambda p: len(p.parts), reverse=True):
        try:
            d.rmdir()
        except OSError:
            pass

    # Move the staged packages up. If the original package directory survived --
    # which happens when some of its files were already correctly placed and so
    # were never moved -- then moving the directory would nest it inside, giving
    # SDK/rwfilesystem/rwfilesystem/. Move the contents in that case.
    for pkg in sorted(p.name for p in STAGE.iterdir() if p.is_dir()):
        src, dst = f"{SDK_REL}/__restructure/{pkg}", f"{SDK_REL}/{pkg}"
        if (SDK / pkg).exists():
            for entry in sorted(p.name for p in (STAGE / pkg).iterdir()):
                if (SDK / pkg / entry).exists():
                    raise RuntimeError(
                        f"{dst}/{entry} already exists; move list and tree disagree")
                git("mv", "-f", f"{src}/{entry}", f"{dst}/{entry}")
            (STAGE / pkg).rmdir()
        else:
            git("mv", "-f", src, dst)
    STAGE.rmdir()
    print(f"moved {len(list(SDK.iterdir()))} package directories into place")

    # --- index repair for uncommitted work -------------------------------
    for rel, _ in plan_index:
        new_rel = rel
        within = rel[len(SDK_REL) + 1:] if rel.startswith(SDK_REL + "/") else None
        if within and within in mv:
            new_rel = f"{SDK_REL}/{mv[within]}"
        head = git("show", f"HEAD:{rel}")
        fixed = rewrite(head, rel, mv, by_dir)
        tmp = REPO / ".git" / "gf2asi_index_blob"
        tmp.write_text(fixed, encoding="utf-8", errors="surrogateescape", newline="")
        blob = git("hash-object", "-w", str(tmp)).strip()
        tmp.unlink()
        git("update-index", "--cacheinfo", f"100644,{blob},{new_rel}")
        print(f"index: {new_rel} = HEAD + include rewrite only "
              f"(your changes stay in the working tree)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
