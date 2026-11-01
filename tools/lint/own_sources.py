#!/usr/bin/env python3
"""hf-core's own translation units in the host compile database.

run-clang-tidy.sh and run-cppcheck.sh analyse exactly this set, so both tools
agree on what counts as hf-core code. Submodule sources (hf-core-drivers,
hf-core-utils) are separate repositories with their own CI and are left out,
as are build-tree files.

Usage:
  own_sources.py <compile_commands.json>             print one source path per line
  own_sources.py <compile_commands.json> --db <out>  write a filtered compile database
"""

from __future__ import annotations

import argparse
import json
import os
import sys

KEEP = ("/handlers/", "/tests/host/")
SKIP = ("/hf-core-drivers/", "/hf-core-utils/", "/build/")
SOURCE_SUFFIXES = (".c", ".cpp", ".cc")


def own_entries(db: list[dict], root: str) -> list[dict]:
    out = []
    for entry in db:
        path = os.path.realpath(os.path.join(entry.get("directory", ""), entry["file"])).replace("\\", "/")
        rel = "/" + os.path.relpath(path, root).replace("\\", "/")
        if rel.startswith("/.."):
            continue
        if any(s in rel for s in SKIP):
            continue
        if any(rel.startswith(k) for k in KEEP) and rel.endswith(SOURCE_SUFFIXES):
            out.append(entry)
    return out


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("compile_commands")
    parser.add_argument("--db", help="write the filtered compile database here instead of printing paths")
    args = parser.parse_args()

    root = os.path.realpath(os.path.join(os.path.dirname(__file__), "..", ".."))
    with open(args.compile_commands, encoding="utf-8") as fh:
        entries = own_entries(json.load(fh), root)
    if not entries:
        print(f"no hf-core translation units in {args.compile_commands}", file=sys.stderr)
        return 2
    if args.db:
        with open(args.db, "w", encoding="utf-8") as fh:
            json.dump(entries, fh, indent=1)
    else:
        for entry in entries:
            print(entry["file"])
    return 0


if __name__ == "__main__":
    sys.exit(main())
