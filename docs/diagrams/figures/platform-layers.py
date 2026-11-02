"""The HardFOC platform, from the device drivers up to the products, and where hf-core sits in it.

Reads: From the bottom up. Each device driver is its own repository and talks to its chip through a small CRTP
interface; hf-core-drivers collects them as submodules, together with hf-internal-interface-wrap (the Base*
interfaces and their ESP32 and STM32 implementations) and hf-pincfg. hf-core-utils collects the RTOS wrapper,
the general utilities and the CANopen helpers. hf-core's handlers sit on top of those three: each one wraps one
driver behind a thread-safe API that takes Base* interfaces. A board HAL creates the MCU peripherals, constructs
the handlers it needs and exposes managers; a product is firmware on one board HAL. The driver list is read from
cmake/hf_core_build_settings.cmake and the handler list from handlers/.
"""

import re
from pathlib import Path

from hardfoc_mkdocs_theme.figkit import Fig

ROOT = Path(__file__).resolve().parents[3]
SETTINGS = "cmake/hf_core_build_settings.cmake"


def data():
    text = (ROOT / SETTINGS).read_text()
    drivers = sorted(set(re.findall(r"HF_CORE_DRIVER_EXT\}/(hf-[a-z0-9-]+)", text)))
    utils = sorted(set(re.findall(r"HF_CORE_UTILS_ROOT\}/(hf-utils-[a-z-]+)", text)))
    handlers = sorted(p.name for p in (ROOT / "handlers").iterdir() if p.is_dir() and p.name != "common")
    return {
        "drivers": drivers,
        "utils": utils,
        "handlers": handlers,
        "sources": [f"{SETTINGS}:driver and utility paths", "handlers/:handler folders"],
    }


def _short(name: str) -> str:
    return name.removeprefix("hf-").removesuffix("-driver").removesuffix("-rmt").upper()


def build(d):
    f = Fig(
        "platform-layers",
        "Where hf-core sits",
        alt="Layer diagram of the HardFOC platform. At the bottom, the MCU SDK and RTOS and the device driver "
        "repositories. Above them hf-core-drivers (drivers, interface wrap, pin config) and hf-core-utils (RTOS "
        "wrapper, general and CANopen utilities). On those, the hf-core handlers. Above the handlers, the board HALs, "
        "and at the top the products.",
        width=1200,
        kicker="Platform",
        subtitle="Handlers turn a device driver into a thread-safe object that takes Base* interfaces, "
        "so a board HAL wires chips to buses and nothing above it sees the MCU.",
        profile="overview",
        legend=[("brand", "hf-core"), ("teal", "hf-core submodules"), ("slate", "around hf-core")],
    )
    W = f.body_w
    gap = 40
    prod = f.card(0, 0, W, "Products", ["firmware on one board HAL: tasks, control loops, protocols"], tone="slate")
    hal = f.card(0, prod.b + gap, W, "Board HALs: hf-hal-vortex-v1, hf-hal-flux-v1",
                 ["create the MCU peripherals, construct the handlers, expose managers; pin maps from hf-pincfg"],
                 tone="slate")
    top = hal.b + gap
    names = [n.upper().replace("_", " ") for n in d["handlers"]]
    ch = f.chips(16, f.region_inner_top(top), W - 32, names, tone="brand")
    reg = f.region(0, top, W, f.region_inner_top(top) - top + ch + 18, f"Handlers ({len(names)})", tone="brand",
                   sub="handlers/: thread-safe, lazy init, Base* in, driver inside")
    row = reg.b + gap
    cw = (W - 2 * 24) / 3
    drv = f.card(0, row, cw, "hf-core-drivers", [f"{len(d['drivers'])} device drivers as submodules", "+ hf-pincfg"],
                 tone="teal", accent=True)
    iw = f.card(cw + 24, row, cw, "hf-internal-interface-wrap", ["Base* interfaces", "Esp* today, Stm* next"],
                tone="teal", accent=True)
    ut = f.card(2 * (cw + 24), row, cw, "hf-core-utils", [", ".join(u.removeprefix("hf-utils-") for u in d["utils"])],
                tone="teal", accent=True)
    h = max(drv.h, iw.h, ut.h)
    bottom = row + h + gap
    dchips = [_short(n) for n in d["drivers"]]
    dch = f.chips(16, f.region_inner_top(bottom), 2 * cw + 24 - 32, dchips, tone="slate")
    dreg = f.region(0, bottom, 2 * cw + 24, f.region_inner_top(bottom) - bottom + dch + 18, "Device drivers",
                    tone="slate", sub="one repository per chip")
    sdk = f.card(2 * (cw + 24), bottom, cw, "MCU SDK and RTOS", ["ESP-IDF and FreeRTOS today", "STM32 HAL, CAN FD next"],
                 tone="slate", h=dreg.h)
    for a, b in ((prod, hal), (hal, reg)):
        f.connect(a, b, "bottom", "top", tone="rail")
    for b in (drv, iw, ut):
        f.connect(reg, b, "bottom", "top", at=(b.cx - reg.x) / reg.w, tone="rail")
    f.connect(drv, dreg, "bottom", "top", at=0.5, bt=(drv.cx - dreg.x) / dreg.w, tone="rail")
    f.connect(iw, sdk, "bottom", "top", bt=0.3, tone="rail")
    f.connect(ut, sdk, "bottom", "top", tone="rail")
    return f
