"""Generated regions for the hf-core site, read from the build settings and the handler headers.

``feature-switches``
    Every ``HF_CORE_ENABLE_*`` switch in ``cmake/hf_core_build_settings.cmake``: group, default, what turns it on
    automatically, the compile definition it adds, the handler and driver it compiles, and the host-build value
    from the top-level ``CMakeLists.txt``.
``handler-catalogue``
    One row per handler folder under ``handlers/``: class, family page, driver, the ``Base*`` interfaces its
    constructors take, the switch, and the host test when there is one.
``handler-<folder>``
    The reference block of one handler: brief, constructors, public methods by group.
"""

from __future__ import annotations

import re
import textwrap
from pathlib import Path

from hardfoc_mkdocs_theme.paths import repo_root

ROOT = repo_root()
SETTINGS = "cmake/hf_core_build_settings.cmake"
HOST_CMAKE = "CMakeLists.txt"
HANDLERS = ROOT / "handlers"

# Handler folder -> family page (docs/handlers/<page>.md). Every folder must be listed: the generator fails on a
# new handler that has no family, so the catalogue cannot silently miss it.
FAMILIES = {
    "motion": ("Motor and motion control", ["tmc9660", "tmc5160", "dynamixel"]),
    "position": ("Position and inertial sensing", ["as5047u", "bno08x"]),
    "actuators": ("Solenoid and load drivers", ["tle92466ed", "max22200"]),
    "io": ("I/O, PWM, LEDs and displays", ["pcal95555", "pca9685", "ws2812", "isf15acp4"]),
    "analog": ("Analog and temperature", ["ads7952", "ads9324", "ntc", "mcp9700"]),
    "environment": ("Pressure, flow and gas", ["bmp581", "sfm", "alicat_basis2", "fdo2"]),
    "power-security": ("Power, memory and security", ["pf1550", "at25040", "se050"]),
    "logger": ("Logger", ["logger"]),
}
NOT_HANDLERS = {"common"}
GROUP_TITLES = {
    "Optional Utility Groups": "Utility groups",
    "Optional Interface Implementations": "MCU interfaces",
    "External Drivers + Handlers": "Drivers and handlers",
}


def _read(rel: str) -> str:
    return (ROOT / rel).read_text(encoding="utf-8")


def _family_of(folder: str) -> str:
    for page, (_title, folders) in FAMILIES.items():
        if folder in folders:
            return page
    raise SystemExit(f"[gen] handlers/{folder}/ has no family in tools/docs/generators/hfcore.py FAMILIES")


# --------------------------------------------------------------------------------------------------------------
# Build settings
# --------------------------------------------------------------------------------------------------------------


def _blocks(text: str):
    """Yield (condition, body) for every top-level ``if(HF_CORE_ENABLE_X)`` ... ``endif()`` block."""
    for m in re.finditer(r"^[ \t]*if\((HF_CORE_ENABLE_\w+)\)\n(.*?)^[ \t]*endif\(\)", text, re.M | re.S):
        yield m.group(1), m.group(2)


def switches() -> list[dict]:
    text = _read(SETTINGS)
    out: dict[str, dict] = {}
    group = ""
    for line in text.splitlines():
        if "Auto-Enable Dependencies" in line:
            group = "Peripherals a driver turns on"
        g = re.match(r"^# ── (.+?) ─", line)
        if g:
            group = g.group(1).strip()
            group = re.sub(r"\s*\(.*\)$", "", group)
        d = re.match(r"^\s*if\(NOT DEFINED (HF_CORE_ENABLE_\w+)\)", line)
        if d and d.group(1) not in out:
            out[d.group(1)] = {"name": d.group(1), "group": group, "default": None, "auto": [], "define": "",
                               "handler": "", "driver": "", "host": "", "ifaces": []}
            cur = d.group(1)
        s = re.match(r"^\s*set\((HF_CORE_ENABLE_\w+) (ON|OFF)\)", line)
        if s and s.group(1) in out and out[s.group(1)]["default"] is None:
            out[s.group(1)]["default"] = s.group(2)
    # Auto-enable: if(A OR B ...) set(X ON)
    auto = text.split("Auto-Enable Dependencies", 1)[1].split("MCU-SPECIFIC", 1)[0]
    for m in re.finditer(r"if\(([^)]*?)\)\s*\n\s*set\((HF_CORE_ENABLE_\w+) ON\)", auto, re.S):
        causes = re.findall(r"HF_CORE_ENABLE_(\w+)", m.group(1))
        if m.group(2) in out:
            out[m.group(2)]["auto"] = causes
    for cond, body in _blocks(text):
        if cond not in out:
            continue
        e = out[cond]
        dm = re.search(r"HF_CORE_COMPILE_DEFINITIONS (\w+)=1", body)
        if dm:
            e["define"] = dm.group(1)
        hm = re.search(r'HF_CORE_INCLUDE_DIRS "\$\{HF_CORE_HANDLER_ROOT\}/(\w+)"', body)
        if hm and (HANDLERS / hm.group(1)).is_dir() and hm.group(1) not in NOT_HANDLERS:
            e["handler"] = hm.group(1)
        for src in re.findall(r"_HF_MCU_SRC\}/((?:Esp|Stm)\w+)\.cpp", body):
            if src not in e["ifaces"]:
                e["ifaces"].append(src)
        um = re.search(r"HF_CORE_UTILS_ROOT\}/(hf-utils-[a-z-]+)", body)
        if um and not e["driver"]:
            e["driver"] = um.group(1)
        rm = re.search(r"HF_CORE_DRIVER_EXT\}/(hf-[a-z0-9-]+)", body)
        if rm and not e["driver"]:
            e["driver"] = rm.group(1)
    gates = dict((g.split(":")[0], g.split(":", 1)[1]) for g in re.findall(r'^\s*"(\w+:\w+:[^"]+)"', text, re.M))
    host = _read(HOST_CMAKE)
    for m in re.finditer(r"foreach\(_feature\s+(.*?)\)\s*\n\s*if\(NOT DEFINED HF_CORE_ENABLE_\$\{_feature\}\)\s*\n"
                         r"\s*set\(HF_CORE_ENABLE_\$\{_feature\} (ON|OFF)\)", host, re.S):
        for f in m.group(1).split():
            if f"HF_CORE_ENABLE_{f}" in out:
                out[f"HF_CORE_ENABLE_{f}"]["host"] = m.group(2)
    for e in out.values():
        if not e["host"] and e["auto"]:
            e["host"] = "ON" if any(out.get("HF_CORE_ENABLE_" + a, {}).get("host") == "ON" for a in e["auto"]) else "OFF"
        e["group"] = GROUP_TITLES.get(e["group"], e["group"])
        short = e["name"].removeprefix("HF_CORE_ENABLE_")
        e["gate"] = gates.get(short, "")
        if e["default"] is None:
            raise SystemExit(f"[gen] no default found for {e['name']} in {SETTINGS}")
    return list(out.values())


def feature_switches(page) -> str:
    rows = ["| Switch | Default | Host build | Turned on by | Compiles | Defines |", "| --- | --- | --- | --- | --- | --- |"]
    group = None
    for e in switches():
        if e["group"] != group:
            group = e["group"]
            rows.append(f"| **{group}** | | | | | |")
        compiles = []
        if e["handler"]:
            compiles.append(f"[`handlers/{e['handler']}/`]({_family_of(e['handler'])}-link)")
        if e["driver"]:
            compiles.append(f"[{e['driver']}](https://github.com/hardfoc/{e['driver']})")
        if e["ifaces"]:
            compiles.append(" / ".join(f"`{i}`" for i in e["ifaces"]))
        auto = ", ".join(f"`{a}`" for a in e["auto"]) or ""
        host = e["host"] or e["default"]
        note = f" (needs {e['gate'].split(':')[0]}: {e['gate'].split(':')[1]})" if e["gate"] else ""
        rows.append(
            f"| `{e['name']}` | {e['default']} | {host} | {auto} | {' + '.join(compiles)}{note} | "
            f"{'`' + e['define'] + '`' if e['define'] else ''} |"
        )
    body = "\n".join(rows)
    body = re.sub(r"\]\(([a-z-]+)-link\)", lambda m: f"]({page.rel('docs/handlers/' + m.group(1) + '.md')})", body)
    return body + f"\n\nRead from [`{SETTINGS}`]({page.rel(SETTINGS)}) and [`{HOST_CMAKE}`]({page.rel(HOST_CMAKE)}).\n"


# --------------------------------------------------------------------------------------------------------------
# Handler headers
# --------------------------------------------------------------------------------------------------------------


def _strip_comments(src: str) -> str:
    src = re.sub(r"/\*.*?\*/", lambda m: "\n" * m.group(0).count("\n"), src, flags=re.S)
    return re.sub(r"//[^\n]*", "", src)


def _main_header(folder: str) -> Path:
    d = HANDLERS / folder
    cands = sorted(d.glob("*Handler.h")) or sorted(d.glob("Logger.h"))
    if not cands:
        raise SystemExit(f"[gen] no handler header in handlers/{folder}/")
    return cands[0]


def _class_body(src: str, name: str) -> str:
    m = re.search(rf"^class {name}\b[^;{{]*\{{", src, re.M)
    if not m:
        raise SystemExit(f"[gen] class {name} not found")
    depth, i = 1, m.end()
    while depth and i < len(src):
        depth += {"{": 1, "}": -1}.get(src[i], 0)
        i += 1
    return src[m.end(): i - 1]


def _constructors(body: str, name: str) -> list[str]:
    """Public constructor signatures (copy and move constructors left out), with balanced parentheses."""
    out = []
    access = "private"
    for m in re.finditer(rf"\b(public|private|protected)\s*:|(?<![~\w])(?:explicit\s+)?{name}\s*\(", body):
        if m.group(1):
            access = m.group(1)
            continue
        if access != "public":
            continue
        i, depth = body.index("(", m.start()), 0
        j = i
        while j < len(body):
            depth += {"(": 1, ")": -1}.get(body[j], 0)
            if depth == 0:
                break
            j += 1
        args = " ".join(body[i + 1: j].split())
        if re.fullmatch(rf"(const\s+)?{name}\s*&{{1,2}}\s*\w*", args):
            continue
        out.append(f"{name}({args})")
    return out


def handler_info(folder: str) -> dict:
    path = _main_header(folder)
    raw = path.read_text(encoding="utf-8")
    brief = re.search(r"@brief\s+(.+?)(?:\n\s*\*\s*\n|\n\s*\*\s*@|\*/)", raw, re.S)
    brief = re.sub(r"\s*\n\s*\*\s*", " ", brief.group(1)).strip() if brief else ""
    name = path.stem
    src = _strip_comments(raw)
    body = _class_body(src, name)
    # Walk the class body at depth 0, keeping public declarations; @name groups come from the raw header.
    groups_raw = _class_body(raw, name)
    access = "private"
    depth = 0
    stmt = ""
    group = ""
    methods: list[tuple[str, str]] = []
    ctors: list[str] = []
    # Map each statement to the nearest preceding @name in the raw body by order of appearance.
    names_pos = [(m.start(), m.group(1).strip()) for m in re.finditer(r"@name\s+(.+)", groups_raw)]
    raw_cursor = 0
    for ch in body:
        if depth == 0 and ch in ";{":
            s = " ".join(stmt.split())
            stmt = ""
            if ch == "{":
                depth = 1
            am = re.fullmatch(r"(public|private|protected)\s*:\s*(.*)", s)
            if am:
                access, s = am.group(1), am.group(2)
            for acc in ("public", "private", "protected"):
                if s.startswith(acc + ":"):
                    access, s = acc, s[len(acc) + 1:].strip()
            if access != "public" or "(" not in s:
                continue
            if re.search(r"=\s*delete|operator\s*=", s) or s.startswith(("using ", "friend ", "static_assert")):
                continue
            # Find this declaration in the raw body to get its @name group.
            fn = re.match(r"(?:template\s*<.*?>\s*)?(?:[\w:<>,*&\s\[\]]+?\s)?[*&]*~?(\w+)\s*\(", s)
            if not fn:
                continue
            ident = fn.group(1)
            pos = groups_raw.find(ident + "(", raw_cursor)
            if pos < 0:
                pos = groups_raw.find(ident + "(")
            else:
                raw_cursor = pos
            group = ""
            for p, g in names_pos:
                if p <= pos:
                    group = g
            if ident == name:
                continue
            elif ident.startswith("~") or ident == "~" + name:
                continue
            elif s.split("(")[0].strip().endswith("~" + name):
                continue
            else:
                methods.append((group, ident))
            continue
        if depth > 0:
            depth += {"{": 1, "}": -1}.get(ch, 0)
            continue
        stmt += ch
        if re.search(r"(public|private|protected)\s*:\s*$", stmt):
            access = re.search(r"(public|private|protected)\s*:\s*$", stmt).group(1)
            stmt = ""
    ctors = _constructors(body, name)
    ifaces = sorted({b for c in ctors for b in re.findall(r"\b(Base[A-Z]\w+)\b", c)})
    if name == "Logger":
        ifaces = ["BaseLogger"]
    return {"folder": folder, "class": name, "header": path.relative_to(ROOT).as_posix(), "brief": brief,
            "ctors": ctors, "methods": methods, "ifaces": ifaces}


def _split_args(args: str) -> list[str]:
    """Split a parameter list at top-level commas."""
    parts, depth, cur = [], 0, ""
    for ch in args:
        if ch in "(<{[":
            depth += 1
        elif ch in ")>}]":
            depth -= 1
        if ch == "," and depth == 0:
            parts.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        parts.append(cur.strip())
    return parts


def _switch_by_handler() -> dict:
    return {e["handler"]: e for e in switches() if e["handler"]}


def _host_test(folder: str) -> str:
    key = folder.replace("_", "")
    for t in sorted((ROOT / "tests/host").glob("test_*.cpp")):
        if t.stem.removeprefix("test_") == key:
            return t.relative_to(ROOT).as_posix()
    return ""


def handler_folders() -> list[str]:
    return sorted(p.name for p in HANDLERS.iterdir() if p.is_dir() and p.name not in NOT_HANDLERS)


def handler_catalogue(page) -> str:
    sw = _switch_by_handler()
    rows = ["| Handler | Family | Driver | Takes | Switch | Host test |", "| --- | --- | --- | --- | --- | --- |"]
    for folder in sorted(handler_folders(), key=lambda f: (list(FAMILIES).index(_family_of(f)), f)):
        h = handler_info(folder)
        fam = _family_of(folder)
        e = sw.get(folder, {})
        drv = e.get("driver", "")
        drv = f"[{drv}](https://github.com/hardfoc/{drv})" if drv else "(in hf-core)"
        test = _host_test(folder)
        test = f"[`{Path(test).name}`]({page.rel(test)})" if test else ""
        anchor = h["class"].lower()
        rows.append(
            f"| [`{h['class']}`]({page.rel('docs/handlers/' + fam + '.md')}#{anchor}) | {FAMILIES[fam][0]} | {drv} | "
            f"{', '.join('`' + i + '`' for i in h['ifaces'])} | `{e.get('name', '')}` | {test} |"
        )
    return "\n".join(rows) + f"\n\nRead from the headers in [`handlers/`]({page.rel('handlers/')}) and [`{SETTINGS}`]({page.rel(SETTINGS)}).\n"


def _handler_block(folder: str):
    def render(page) -> str:
        h = handler_info(folder)
        e = _switch_by_handler().get(folder, {})
        out = textwrap.wrap(h["brief"].rstrip(".") + ".", 118) + [""]
        facts = [f"Header: [`{h['header']}`]({page.rel(h['header'])})"]
        if e:
            facts.append(f"Switch: `{e['name']}` (default {e['default']})")
            if e.get("driver"):
                facts.append(f"Driver: [{e['driver']}](https://github.com/hardfoc/{e['driver']})")
        test = _host_test(folder)
        if test:
            facts.append(f"Host test: [`{test}`]({page.rel(test)})")
        out.extend("- " + f for f in facts)
        out.append("")
        if h["ctors"]:
            out.append("```cpp")
            for c in h["ctors"]:
                if len(c) <= 100:
                    out.append(c + ";")
                    continue
                head, args = c.split("(", 1)
                parts = _split_args(args[:-1])
                out.append(head + "(")
                out.extend("    " + a + ("," if i < len(parts) - 1 else ");") for i, a in enumerate(parts))
            out.append("```")
            out.append("")
        groups: dict[str, list[str]] = {}
        for g, m in h["methods"]:
            key = g or "All"
            groups.setdefault(key, [])
            if m not in groups[key]:
                groups[key].append(m)
        if groups:
            out.append("| Group | Public methods |")
            out.append("| --- | --- |")
            for g, ms in groups.items():
                out.append(f"| {g} | {', '.join('`' + m + '()`' for m in ms)} |")
        return "\n".join(out) + "\n"

    return render


REGIONS = {"feature-switches": feature_switches, "handler-catalogue": handler_catalogue}
for _f in handler_folders():
    REGIONS["handler-" + _f.replace("_", "-")] = _handler_block(_f)


def _section(path: str, name: str) -> str:
    text = _read(path)
    m = re.search(rf"--8<-- \[start:{name}\]\n(.*?)\n[^\n]*--8<-- \[end:{name}\]", text, re.S)
    if not m:
        raise SystemExit(f"[gen] no section {name!r} in {path}")
    return m.group(1)


def quickstart_code() -> str:
    """The README copy of the Quickstart example (the site includes tests/docs/quickstart.cpp directly)."""
    return "```cpp\n" + _section("tests/docs/quickstart.cpp", "quickstart") + "\n```\n"


REGIONS["quickstart-code"] = quickstart_code
