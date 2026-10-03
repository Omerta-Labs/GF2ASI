#!/usr/bin/env python3
"""Resolve every #include in the project and report the ones that do not exist.

Stands in for a compile when checking a large rename. Phase 2 moves roughly 500
files and rewrites ~550 include directives, and a stale directive is the whole
risk; this catches one in a second rather than at the end of a build.

Resolution order matches MSVC for quoted includes: first the directory of the
including file, then each -I root in order. Angled includes skip the including
file's directory. Roots mirror CMakeLists.txt.

Case matters even though Windows does not care, because the original tree is
all lowercase and Phase 2c relies on getting the casing right. A directive that
only resolves case-insensitively is reported as a warning -- it will build here
and nowhere else.

Exit code is non-zero if anything is unresolved, so this works in a hook.

Usage:
    python check_includes.py
    python check_includes.py --quiet          # just the summary
    python check_includes.py --ignore-case    # drop the casing warnings
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent

# Mirrors target_include_directories() in CMakeLists.txt. Keep the two in step;
# CMake errors out if one of the SDK roots below stops existing.
SDK_ROOTS = [
    "allocator/include",
    "ears_common/include",
    "ears_common/src",
    "ears_framework/src",
    "ears_framework/src/game_framework",
    "ears_godfather/src",
    "ears_locale/include",
    "ears_physics/src",
    "ears_rt_cct/include",
    "ears_rt_cct/src",
    "ears_rt_llrender/include",
    "ears_rt_llrender/src",
    "ears_statemachine/include",
    "ears_trinity/include",
    "ears_trinity/src",
    "rwcontroller/include",
    "rwfilesystem/include",
]

INCLUDE_ROOTS = [
    REPO,
    REPO / "Source",
    *[REPO / "Source" / "SDK" / r for r in SDK_ROOTS],
    REPO / "Vendors/d3d9/include",
    REPO / "Vendors/detours",
    REPO / "Vendors/polyhook/include",
    REPO / "Vendors/libcurl/include",
    REPO / "Vendors/lua/include",
    REPO / "Vendors/sol",
]

SCAN_EXT = (".cpp", ".c", ".h", ".hpp", ".inl")

# Vendored third party that lives under Source/ and is compiled, but is not ours
# to fix. Both carry platform-conditional includes (Carbon, emscripten,
# sys/wait.h, optional stb and freetype backends) that are #ifdef'd out on
# Windows, so scanning them only produces noise.
SKIP_TREES = ("Source/Addons/imgui", "Source/Addons/discord")
INCLUDE_RE = re.compile(r'^\s*#\s*include\s*(["<])([^">]+)[">]', re.MULTILINE)

# Resolved by the toolchain, not by us.
C_HEADERS = {
    "assert.h", "complex.h", "ctype.h", "errno.h", "fenv.h", "float.h",
    "inttypes.h", "iso646.h", "limits.h", "locale.h", "math.h", "setjmp.h",
    "signal.h", "stdalign.h", "stdarg.h", "stdatomic.h", "stdbool.h",
    "stddef.h", "stdint.h", "stdio.h", "stdlib.h", "stdnoreturn.h", "string.h",
    "tgmath.h", "threads.h", "time.h", "uchar.h", "wchar.h", "wctype.h",
}

PLATFORM_PREFIXES = (
    # Windows SDK and DirectX
    "windows", "psapi", "d3d", "dxgi", "winsock", "ws2", "xinput", "dinput",
    "shlobj", "shellapi", "objbase", "unknwn", "wtypes", "process.h", "io.h",
    "direct.h", "tchar.h", "mmsystem", "timeapi", "dbghelp", "comdef", "wrl",
    "winternl", "winnt", "imm.h", "dwmapi", "versionhelpers", "intsafe",
    "fcntl.h", "sys/", "conio.h", "malloc.h", "crtdbg.h", "share.h",
    # compiler intrinsics
    "intrin.h", "immintrin.h", "emmintrin.h", "nmmintrin.h", "smmintrin.h",
    "xmmintrin.h", "pmmintrin.h", "tmmintrin.h", "mmintrin.h",
    # non-Windows platforms, reached only through #ifdef
    "unistd.h", "carbon/", "targetconditionals.h", "emscripten/", "pthread.h",
)


def is_system(target: str) -> bool:
    low = target.lower()
    if "/" not in low and "." not in low:
        return True                       # <vector>, <string>, <cstdint>, ...
    if low in C_HEADERS:
        return True
    if low.startswith(("std", "experimental/")):
        return True
    return any(low.startswith(p) for p in PLATFORM_PREFIXES)


_LISTING: dict[Path, dict[str, str]] = {}


def real_name(parent: Path, name: str) -> str | None:
    """The on-disk spelling of name inside parent, or None if absent.

    Path.resolve() does not reliably correct case on Windows, so compare against
    the actual directory listing instead -- and do it per component, since a
    miscased *directory* matters just as much as a miscased file.
    """
    listing = _LISTING.get(parent)
    if listing is None:
        try:
            listing = {e.name.lower(): e.name for e in parent.iterdir()}
        except OSError:
            listing = {}
        _LISTING[parent] = listing
    return listing.get(name.lower())


def resolve(target: str, quoted: bool, origin: Path) -> tuple[Path | None, str | None]:
    """Return (path, on-disk spelling of target). Mirrors MSVC search order.

    The second value is None when the path does not exist, and equals target
    when the casing is already correct.
    """
    roots = ([origin.parent] if quoted else []) + INCLUDE_ROOTS
    parts = target.split("/")
    for root in roots:
        here, actual = root, []
        for part in parts:
            if part in ("..", "."):
                here, actual = (here.parent if part == ".." else here), actual + [part]
                continue
            got = real_name(here, part)
            if got is None:
                break
            actual.append(got)
            here = here / got
        else:
            return here, "/".join(actual)
    return None, None


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--quiet", action="store_true")
    parser.add_argument("--ignore-case", action="store_true",
                        help="do not warn about includes that only resolve case-insensitively")
    args = parser.parse_args()

    files = [p for p in (REPO / "Source").rglob("*")
             if p.suffix.lower() in SCAN_EXT and "Vendors" not in p.parts
             and not any(p.as_posix().replace("\\", "/").find(t) >= 0 for t in SKIP_TREES)]
    files.append(REPO / "dllmain.cpp")

    missing: list[tuple[Path, int, str]] = []
    miscased: list[tuple[Path, int, str, str]] = []
    total = 0

    for path in files:
        text = path.read_text(encoding="utf-8", errors="replace")
        for match in INCLUDE_RE.finditer(text):
            quoted = match.group(1) == '"'
            target = match.group(2).strip().replace("\\", "/")
            if is_system(target):
                continue
            total += 1
            found, actual = resolve(target, quoted, path)
            line = text.count("\n", 0, match.start()) + 1
            if found is None:
                missing.append((path, line, target))
            elif actual != target and not args.ignore_case:
                miscased.append((path, line, target, actual))

    if missing and not args.quiet:
        print(f"=== UNRESOLVED ({len(missing)}) ===")
        for path, line, target in missing:
            print(f"  {path.relative_to(REPO).as_posix()}:{line}\n      {target}")
        print()
    if miscased and not args.quiet:
        print(f"=== WRONG CASE ({len(miscased)}) ===")
        print("  builds on Windows, breaks anywhere case-sensitive\n")
        for path, line, target, actual in miscased:
            print(f"  {path.relative_to(REPO).as_posix()}:{line}\n"
                  f"      wrote:   {target}\n      on disk: {actual}")
        print()

    print(f"{total} project includes across {len(files)} files: "
          f"{len(missing)} unresolved, {len(miscased)} wrong case")
    return 1 if missing else 0


if __name__ == "__main__":
    raise SystemExit(main())
