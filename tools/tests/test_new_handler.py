"""tools/new_handler.py and tools/status_rules.py, on a scratch copy of the files the tool edits.

Run: python3 -m unittest discover -s tools/tests -v   (ctest runs it as new_handler_tool)
"""

from __future__ import annotations

import shutil
import sys
import tempfile
import unittest
from pathlib import Path

TOOLS = Path(__file__).resolve().parent.parent
REPO = TOOLS.parent
sys.path.insert(0, str(TOOLS))

import new_handler  # noqa: E402
import status_rules  # noqa: E402

COPIED = [
    "cmake/hf_core_build_settings.cmake",
    "CMakeLists.txt",
    "tests/host/CMakeLists.txt",
    "tools/docs/generators/hfcore.py",
    "docs/handlers/environment.md",
]

FAKE_HEADER = """#pragma once
#include <cstdint>
namespace foo42 {
/// Errors.
enum class Error : std::uint8_t {
    Ok = 0,            ///< success
    Bus,               // a transfer failed
    Timeout,
    InvalidArgument,
    DeviceIdMismatch,
    Crc,
};
template <typename Derived> class UartInterface {};
template <typename Port> class FOO42 {};
template <typename T> struct Result { T value{}; Error error{Error::Ok}; bool ok() const { return error == Error::Ok; } };
}  // namespace foo42
"""


class NewHandlerTest(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = Path(tempfile.mkdtemp(prefix="new_handler_"))
        for rel in COPIED:
            (self.tmp / rel).parent.mkdir(parents=True, exist_ok=True)
            shutil.copy(REPO / rel, self.tmp / rel)
        shutil.copytree(REPO / "templates" / "handler", self.tmp / "templates" / "handler",
                        ignore=shutil.ignore_patterns("example"))
        drv = self.tmp / "hf-core-drivers" / "external" / "hf-foo42-driver"
        (drv / "cmake").mkdir(parents=True)
        (drv / "inc").mkdir()
        (drv / "cmake" / "hf_foo42_build_settings.cmake").write_text(
            'set(HF_FOO42_PUBLIC_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/../inc")\n')
        (drv / "inc" / "foo42.hpp").write_text(FAKE_HEADER)

    def tearDown(self) -> None:
        shutil.rmtree(self.tmp)

    def run_tool(self, *extra: str) -> int:
        return new_handler.main(["--chip", "Foo42", "--bus", "uart", "--driver-repo", "hf-foo42-driver",
                                 "--driver-header", "foo42.hpp", "--family", "environment",
                                 "--root", str(self.tmp), *extra])

    def read(self, rel: str) -> str:
        return (self.tmp / rel).read_text(encoding="utf-8")

    def test_scaffolds_and_registers_once(self) -> None:
        self.assertEqual(self.run_tool(), 0)
        for rel in ("handlers/foo42/Foo42Handler.h", "handlers/foo42/Foo42Handler.cpp",
                    "handlers/foo42/HalUartFoo42Comm.h", "tests/host/test_foo42.cpp"):
            self.assertTrue((self.tmp / rel).exists(), rel)

        settings = self.read("cmake/hf_core_build_settings.cmake")
        self.assertEqual(settings.count("if(NOT DEFINED HF_CORE_ENABLE_FOO42)"), 1)
        self.assertEqual(settings.count('"${HF_CORE_HANDLER_ROOT}/foo42/Foo42Handler.cpp"'), 1)
        self.assertEqual(settings.count('list(APPEND HF_CORE_INCLUDE_DIRS "${HF_CORE_HANDLER_ROOT}/foo42")'), 1)
        self.assertEqual(settings.count("HARDFOC_FOO42_SUPPORT=1"), 1)
        self.assertEqual(settings.count('string(APPEND _hf_enabled_features " FOO42")'), 1)
        self.assertIn("OR HF_CORE_ENABLE_FOO42)\n    set(HF_CORE_ENABLE_UART ON)", settings)
        self.assertNotIn("HF_FOO42_SOURCE_FILES", settings)  # header-only driver: no source list
        # The new default sits with the other drivers, before the logger.
        self.assertLess(settings.index("HF_CORE_ENABLE_FOO42)"), settings.index("# ── Logger (almost always needed)"))

        self.assertRegex(self.read("CMakeLists.txt"), r"foreach\(_feature\s[^)]*\bFOO42\b[^)]*\)\s*\n\s*if")
        self.assertIn("hf_core_host_test(test_foo42 test_foo42.cpp)", self.read("tests/host/CMakeLists.txt"))
        self.assertRegex(self.read("tools/docs/generators/hfcore.py"), r'"environment": \([^\]]*"foo42"\]')
        self.assertIn("<!-- generated: handler-foo42 -->", self.read("docs/handlers/environment.md"))

        comm = self.read("handlers/foo42/HalUartFoo42Comm.h")
        self.assertIn("using Foo42Port = foo42::UartInterface<HalUartFoo42Comm>;", comm)
        self.assertIn("public HalUartCommBase<HalUartFoo42Comm, Foo42Port>", comm)
        self.assertIn("case foo42::Error::DeviceIdMismatch:\n    return Status::NotFound;", comm)
        self.assertIn("case foo42::Error::Crc:\n    return Status::Bus;", comm)
        handler = self.read("handlers/foo42/Foo42Handler.h")
        self.assertIn("using Foo42Driver = foo42::FOO42<HalUartFoo42Comm>;", handler)
        self.assertIn("EmplaceDriver(comm_).Initialize()", self.read("handlers/foo42/Foo42Handler.cpp"))

        # A second run refuses: the switch and the files exist.
        with self.assertRaises(SystemExit):
            self.run_tool()

    def test_check_mode_reports_drift(self) -> None:
        args = ["--example-dir", "out"]
        self.assertEqual(self.run_tool(*args), 0)
        self.assertEqual(self.run_tool(*args, "--check"), 0)
        (self.tmp / "out/foo42/Foo42Handler.cpp").write_text("// edited\n")
        self.assertEqual(self.run_tool(*args, "--check"), 1)
        self.assertNotIn("FOO42", self.read("cmake/hf_core_build_settings.cmake"))  # example mode registers nothing

    def test_rejects_bad_input(self) -> None:
        with self.assertRaises(SystemExit):
            new_handler.main(["--chip", "foo42", "--bus", "uart", "--driver-repo", "hf-foo42-driver",
                              "--driver-header", "foo42.hpp", "--family", "environment", "--root", str(self.tmp)])
        with self.assertRaises(SystemExit):
            self.run_tool("--error-enum", "Missing")
        with self.assertRaises(SystemExit):
            new_handler.main(["--chip", "Foo42", "--bus", "uart", "--driver-repo", "hf-foo42-driver",
                              "--driver-header", "foo42.hpp", "--family", "nope", "--root", str(self.tmp)])


class StatusRulesTest(unittest.TestCase):
    def test_names(self) -> None:
        cases = {
            ("SPI_SUCCESS", "SPI"): "Ok",
            ("None", ""): "Ok",
            ("I2C_ERR_DEVICE_NACK", "I2C"): "Bus",
            ("I2C_ERR_BUS_BUSY", "I2C"): "Busy",
            ("UART_ERR_TIMEOUT", "UART"): "Timeout",
            ("CAN_ERR_BUS_NOT_AVAILABLE", "CAN"): "DependencyNotReady",
            ("NVS_ERR_KEY_NOT_FOUND", "NVS"): "NotFound",
            ("ADC_ERR_OUT_OF_MEMORY", "ADC"): "NoMemory",
            ("GPIO_ERR_NOT_SUPPORTED", "GPIO"): "Unsupported",
            ("InvalidChipId", ""): "NotFound",
            ("NotInitialized", ""): "NotInitialized",
            ("InvalidParameter", ""): "InvalidArgument",
            ("DeviceFault", ""): "Fault",
            ("SPI_ERR_UNKNOWN", "SPI"): "Unknown",
            ("GPIO_ERR_COUNT", "GPIO"): "Unknown",
        }
        for (name, prefix), want in cases.items():
            with self.subTest(name=name):
                self.assertEqual(status_rules.classify(name, prefix), want)
        self.assertTrue(all(s in status_rules.STATUSES for s in status_rules.EXACT.values()))
        self.assertTrue(all(s in status_rules.STATUSES for _p, s in status_rules.RULES))

    def test_snake(self) -> None:
        self.assertEqual(status_rules.snake("InvalidChipId"), "INVALID_CHIP_ID")
        self.assertEqual(status_rules.snake("NvmNotReady"), "NVM_NOT_READY")
        self.assertEqual(status_rules.snake("SPI_ERR_BUS_BUSY"), "SPI_ERR_BUS_BUSY")


if __name__ == "__main__":
    unittest.main()
