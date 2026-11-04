#!/usr/bin/env python3
"""Scaffold a standard hf-core handler from templates/handler/.

Writes, for chip ``<Chip>`` on bus ``<Bus>``:

  handlers/<chip>/<Chip>Handler.h, <Chip>Handler.cpp   the handler (hf::HandlerBase)
  handlers/<chip>/Hal<Bus><Chip>Comm.h                 the adapter (hf::Hal<Bus>CommBase), the error mapping
  tests/host/test_<chip>.cpp                           host test over hf::host::Mock<Bus>

and registers them: the ``HF_CORE_ENABLE_<CHIP>`` switch, sources, include directory, compile definition
and build summary in cmake/hf_core_build_settings.cmake (plus the UART or ADC auto-enable), ON in the host
build, the test in tests/host/CMakeLists.txt, the folder in the docs generator's FAMILIES and a section in
docs/handlers/<family>.md. Run ``hardfoc-docs gen`` afterwards to fill the section.

The driver's error enum is read from its headers and every enumerator gets a ``case`` mapped by
tools/status_rules.py; review the mapping. The defaults assume a driver that follows the HardFOC driver
contract (``<chip>::<CHIP><Bus>Interface`` port, ``<chip>::Error``, ``<chip>::Result<T>``, ``Initialize()``);
override them for a driver that does not yet.

Example (the reference handler under templates/handler/example/, never registered):

  python3 tools/new_handler.py --chip Bmp581 --bus i2c --driver-repo hf-bmp581-driver \\
      --driver-header bmp581.hpp --driver-class Driver --port-base BusInterface \\
      --error-enum DriverError --result-type DriverResult --init-method Init \\
      --example-dir templates/handler/example

``--check`` compares instead of writing (exit 1 on a difference); CI runs it on the example.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import status_rules  # noqa: E402

REPO = Path(__file__).resolve().parent.parent

BUSES = {
    "spi": ("Spi", "BaseSpi", "MockSpi"),
    "i2c": ("I2c", "BaseI2c", "MockI2c"),
    "uart": ("Uart", "BaseUart", "MockUart"),
    "adc": ("Adc", "BaseAdc", "MockAdc"),
}
PLACEHOLDER = re.compile(r"@[A-Za-z_]+@")


class ScaffoldError(SystemExit):
    pass


def fail(msg: str) -> None:
    raise ScaffoldError(f"[new_handler] {msg}")


# ---------------------------------------------------------------------------------------------------------------
# Inputs
# ---------------------------------------------------------------------------------------------------------------


def driver_errors(repo: Path, enum: str) -> list[str]:
    """Enumerators of ``enum class <enum>`` in the driver's headers, in declaration order."""
    for header in sorted(repo.rglob("*.h*")):
        if header.suffix not in (".h", ".hpp", ".ipp") or "/build" in header.as_posix():
            continue
        text = header.read_text(encoding="utf-8", errors="replace")
        m = re.search(rf"\benum\s+class\s+{re.escape(enum)}\b\s*(?::\s*[\w:]+\s*)?\{{(.*?)\}}", text, re.S)
        if not m:
            continue
        body = re.sub(r"/\*.*?\*/", "", m.group(1), flags=re.S)
        body = re.sub(r"//[^\n]*", "", body)
        names = [re.match(r"\s*(\w+)", item).group(1) for item in body.split(",") if item.strip()]
        if names:
            return names
    fail(f"no `enum class {enum}` under {repo}; pass --error-enum")
    return []


def driver_settings(repo: Path) -> tuple[str, str, bool]:
    """(build-settings file name, HF_<VAR> prefix, whether it exports HF_<VAR>_SOURCE_FILES)."""
    files = sorted((repo / "cmake").glob("hf_*_build_settings.cmake"))
    if not files:
        fail(f"{repo.name} has no cmake/hf_*_build_settings.cmake")
    text = files[0].read_text(encoding="utf-8")
    var = re.search(r"\bHF_(\w+?)_PUBLIC_INCLUDE_DIRS\b", text)
    if not var:
        fail(f"{files[0].name} does not set HF_<NAME>_PUBLIC_INCLUDE_DIRS")
    return files[0].name, var.group(1), f"HF_{var.group(1)}_SOURCE_FILES" in text


def values(args: argparse.Namespace, root: Path) -> dict[str, str]:
    if not re.fullmatch(r"[A-Z][A-Za-z0-9]*", args.chip):
        fail(f"--chip must be PascalCase (Bmp581, Tmc9660): {args.chip!r}")
    bus, base, mock = BUSES[args.bus]
    folder = args.folder or args.chip.lower()
    repo = root / "hf-core-drivers" / "external" / args.driver_repo
    if not repo.is_dir():
        fail(f"no driver checkout at {repo.relative_to(root)}")
    if not any(p.name == Path(args.driver_header).name for p in repo.rglob(Path(args.driver_header).name)):
        fail(f"{args.driver_header} is not in {args.driver_repo}")
    ns = args.driver_ns or folder
    errors = driver_errors(repo, args.error_enum)
    mapped = [(e, status_rules.classify(e)) for e in errors]
    oks = [e for e, s in mapped if s == "Ok"]
    if len(oks) != 1:
        fail(f"{ns}::{args.error_enum} needs exactly one success enumerator (Ok/None/Success), found {oks}")
    cases = "\n".join(f"  case {ns}::{args.error_enum}::{e}:\n    return Status::{s};" for e, s in mapped)
    settings, var, has_sources = driver_settings(repo)
    return {
        "Chip": args.chip,
        "chip": folder,
        "chip_flat": folder.replace("_", ""),
        "chip_dash": folder.replace("_", "-"),
        "CHIP": folder.replace("_", "").upper() if not args.switch else args.switch,
        "Bus": bus,
        "BusBase": base,
        "MockBus": mock,
        "driver_repo": args.driver_repo,
        "driver_header": args.driver_header,
        "driver_ns": ns,
        "DriverClass": args.driver_class or args.chip.upper(),
        "PortBase": args.port_base or f"{bus}Interface",
        "ErrorEnum": args.error_enum,
        "ErrorOk": oks[0],
        "ResultType": args.result_type,
        "InitMethod": args.init_method,
        "error_cases": cases,
        "error_values": "\n".join(f"      E::{e}," for e in errors),
        "driver_settings": settings,
        "DRIVER_VAR": var,
        "_has_sources": "1" if has_sources else "",
    }


def render(template: Path, v: dict[str, str]) -> str:
    text = template.read_text(encoding="utf-8")
    for key, value in v.items():
        if not key.startswith("_"):
            text = text.replace(f"@{key}@", value)
    left = sorted(set(PLACEHOLDER.findall(text)))
    if left:
        fail(f"{template.name}: unknown placeholders {left}")
    return text


# ---------------------------------------------------------------------------------------------------------------
# Files
# ---------------------------------------------------------------------------------------------------------------


def outputs(v: dict[str, str], root: Path, example_dir: Path | None) -> dict[Path, str]:
    tpl = root / "templates" / "handler"
    handler_dir = (example_dir / v["chip"]) if example_dir else root / "handlers" / v["chip"]
    test_path = handler_dir if example_dir else root / "tests" / "host"
    return {
        handler_dir / f"{v['Chip']}Handler.h": render(tpl / "Handler.h.in", v),
        handler_dir / f"{v['Chip']}Handler.cpp": render(tpl / "Handler.cpp.in", v),
        handler_dir / f"Hal{v['Bus']}{v['Chip']}Comm.h": render(tpl / "HalComm.h.in", v),
        test_path / f"test_{v['chip_flat']}.cpp": render(tpl / "test.cpp.in", v),
    }


# ---------------------------------------------------------------------------------------------------------------
# Registration
# ---------------------------------------------------------------------------------------------------------------


def _insert_before(text: str, anchor: str, block: str, where: str) -> str:
    i = text.find(anchor)
    if i < 0:
        fail(f"anchor not found in {where}: {anchor.splitlines()[0]!r}")
    return text[:i] + block + text[i:]


def register_build_settings(text: str, v: dict[str, str], root: Path) -> str:
    chip = v["CHIP"]
    if f"HF_CORE_ENABLE_{chip})" in text or f"HF_CORE_ENABLE_{chip} " in text:
        fail(f"HF_CORE_ENABLE_{chip} already exists in cmake/hf_core_build_settings.cmake")
    cm = root / "templates" / "handler" / "cmake"
    sources = render(cm / "sources.cmake.in", v)
    if not v["_has_sources"]:
        sources = "".join(ln for ln in sources.splitlines(True) if "_SOURCE_FILES" not in ln)
    where = "cmake/hf_core_build_settings.cmake"
    text = _insert_before(text, "\n# ── Logger (almost always needed)", render(cm / "default.cmake.in", v), where)
    text = _insert_before(text, "# ── Logger Handler (MCU-agnostic", sources, where)
    text = _insert_before(text, 'if(HF_CORE_ENABLE_LOGGER)\n    list(APPEND HF_CORE_INCLUDE_DIRS',
                          render(cm / "include.cmake.in", v), where)
    text = _insert_before(text, "if(HF_CORE_ENABLE_LOGGER)\n    list(APPEND HF_CORE_COMPILE_DEFINITIONS",
                          render(cm / "define.cmake.in", v), where)
    text = _insert_before(text, 'if(HF_CORE_ENABLE_LOGGER)\n    string(APPEND _hf_enabled_features',
                          render(cm / "summary.cmake.in", v), where)
    auto = {"Uart": r"(if\((?:HF_CORE_ENABLE_\w+\s+OR\s+)*HF_CORE_ENABLE_\w+)(\)\s*\n\s*set\(HF_CORE_ENABLE_UART ON\))",
            "Adc": r"(if\((?:HF_CORE_ENABLE_\w+\s+OR\s+)*HF_CORE_ENABLE_\w+)(\)\s*\n\s*set\(HF_CORE_ENABLE_ADC ON\))"}
    if v["Bus"] in auto:
        text, n = re.subn(auto[v["Bus"]], rf"\1 OR HF_CORE_ENABLE_{chip}\2", text, count=1)
        if n != 1:
            fail(f"no {v['Bus'].upper()} auto-enable block in {where}")
    return text


def register_host_build(text: str, v: dict[str, str]) -> str:
    """Turn the switch on in the host build (the first foreach list, the ON one)."""
    m = re.search(r"(foreach\(_feature\s+)(.*?)(\)\s*\n\s*if\(NOT DEFINED HF_CORE_ENABLE_\$\{_feature\}\)\s*\n"
                  r"\s*set\(HF_CORE_ENABLE_\$\{_feature\} ON\))", text, re.S)
    if not m:
        fail("no ON feature list in CMakeLists.txt")
    names = m.group(2).split()
    if v["CHIP"] in names:
        return text
    body = m.group(2).rstrip() + f"\n        {v['CHIP']}"
    return text[: m.start(2)] + body + text[m.end(2):]


def register_test(text: str, v: dict[str, str]) -> str:
    line = f"hf_core_host_test(test_{v['chip_flat']} test_{v['chip_flat']}.cpp)\n"
    if line in text:
        return text
    last = list(re.finditer(r"^hf_core_host_test\([^\n]*\)\n", text, re.M))
    if not last:
        fail("no hf_core_host_test() line in tests/host/CMakeLists.txt")
    i = last[-1].end()
    return text[:i] + line + text[i:]


def register_family(text: str, v: dict[str, str], family: str) -> str:
    m = re.search(rf'(\n\s*"{re.escape(family)}": \([^\[]*\[)([^\]]*)(\]\),)', text)
    if not m:
        fail(f"no family {family!r} in tools/docs/generators/hfcore.py FAMILIES")
    if f'"{v["chip"]}"' in m.group(2):
        return text
    return text[: m.end(2)] + f', "{v["chip"]}"' + text[m.end(2):]


def plan(args: argparse.Namespace, root: Path) -> tuple[dict[Path, str], list[Path]]:
    """(every file to write with its content, the new handler and test files among them)."""
    v = values(args, root)
    example = (root / args.example_dir) if args.example_dir else None
    files = outputs(v, root, example)
    new = list(files)
    if example:
        return files, new
    if not args.family:
        fail("--family is required (a page under docs/handlers/, e.g. environment)")
    settings = root / "cmake" / "hf_core_build_settings.cmake"
    host = root / "CMakeLists.txt"
    tests = root / "tests" / "host" / "CMakeLists.txt"
    gen = root / "tools" / "docs" / "generators" / "hfcore.py"
    page = root / "docs" / "handlers" / f"{args.family}.md"
    if not page.exists():
        fail(f"no docs/handlers/{args.family}.md")
    files[settings] = register_build_settings(settings.read_text(encoding="utf-8"), v, root)
    files[host] = register_host_build(host.read_text(encoding="utf-8"), v)
    files[tests] = register_test(tests.read_text(encoding="utf-8"), v)
    files[gen] = register_family(gen.read_text(encoding="utf-8"), v, args.family)
    page_text = page.read_text(encoding="utf-8")
    section = render(root / "templates" / "handler" / "docs_section.md.in", v)
    if f"generated: handler-{v['chip_dash']} " not in page_text:
        page_text = page_text.rstrip("\n") + "\n\n" + section
    files[page] = page_text
    return files, new


def main(argv: list[str] | None = None) -> int:
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--chip", required=True, help="PascalCase name: Bmp581 -> Bmp581Handler")
    p.add_argument("--bus", required=True, choices=sorted(BUSES))
    p.add_argument("--driver-repo", required=True, help="folder under hf-core-drivers/external/")
    p.add_argument("--driver-header", required=True, help="umbrella header, as included: bmp581.hpp")
    p.add_argument("--folder", help="handlers/<folder> (default: chip in lower case)")
    p.add_argument("--switch", help="HF_CORE_ENABLE_<SWITCH> (default: folder in upper case)")
    p.add_argument("--family", help="docs/handlers/<family>.md page (required unless --example-dir)")
    p.add_argument("--driver-ns", help="driver namespace (default: folder)")
    p.add_argument("--driver-class", help="driver class template (default: CHIP)")
    p.add_argument("--port-base", help="driver port CRTP base (default: <Bus>Interface)")
    p.add_argument("--error-enum", default="Error", help="driver error enum (default: Error)")
    p.add_argument("--result-type", default="Result", help="driver result template (default: Result)")
    p.add_argument("--init-method", default="Initialize", help="driver start-up method (default: Initialize)")
    p.add_argument("--example-dir", help="write the four files to <dir>/<folder>/ and register nothing")
    p.add_argument("--root", default=str(REPO), help=argparse.SUPPRESS)
    p.add_argument("--check", action="store_true", help="compare with what is on disk; exit 1 on a difference")
    p.add_argument("--force", action="store_true", help="overwrite existing handler files")
    args = p.parse_args(argv)
    root = Path(args.root).resolve()

    files, new = plan(args, root)
    if args.check:
        stale = [f for f, t in files.items() if not f.exists() or f.read_text(encoding="utf-8") != t]
        for f in stale:
            print(f"[new_handler] differs: {f.relative_to(root)}", file=sys.stderr)
        if stale:
            print("[new_handler] regenerate with the same command without --check", file=sys.stderr)
            return 1
        print(f"[new_handler] {len(files)} file(s) match the template")
        return 0
    existing = [f for f in new if f.exists()]
    if existing and not args.force and not args.example_dir:
        fail("already exists: " + ", ".join(str(f.relative_to(root)) for f in existing) + " (use --force)")
    for f, text in files.items():
        f.parent.mkdir(parents=True, exist_ok=True)
        f.write_text(text, encoding="utf-8")
        print(f"[new_handler] wrote {f.relative_to(root)}")
    if not args.example_dir:
        print("[new_handler] next: add the typed API and the config, review ToStatus() in the adapter, then run\n"
              "  cmake --preset host-debug && cmake --build --preset host-debug && ctest --preset host-debug\n"
              "  hardfoc-docs gen && hardfoc-docs check && mkdocs build --strict")
    return 0


if __name__ == "__main__":
    sys.exit(main())
