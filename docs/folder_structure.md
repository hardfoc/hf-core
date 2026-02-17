---
layout: default
title: Folder Structure
nav_order: 3
---

# Folder Structure

## Repository Layout

```
lib/core/
├── .github/
│   └── workflows/                      # CI/CD pipeline definitions
│       ├── esp32-examples-build-ci.yml  #   ESP32 build matrix
│       ├── ci-cpp-analysis.yml          #   Cppcheck static analysis
│       ├── ci-cpp-lint.yml              #   clang-format + clang-tidy
│       ├── ci-docs-linkcheck.yml        #   Documentation link check
│       ├── ci-docs-publish.yml          #   Jekyll + Doxygen publish
│       ├── ci-markdown-lint.yml         #   Markdown linting
│       ├── ci-yaml-lint.yml             #   YAML linting
│       └── release.yml                  #   Tagged release automation
│
├── _config/                            # Code quality configurations
│   ├── .clang-format                   #   C++ formatting (LLVM, 120 col, 2-space)
│   ├── .clang-tidy                     #   Static analysis checks
│   ├── .markdownlint.json              #   Markdown linting rules
│   ├── .yamllint                       #   YAML linting rules
│   ├── _config.yml                     #   Jekyll site configuration
│   ├── Doxyfile                        #   Doxygen API docs generation
│   └── lychee.toml                     #   Link checker config
│
├── docs/                               # Documentation site source
│   ├── index.md                        #   Home page
│   ├── architecture.md                 #   System architecture overview
│   ├── folder_structure.md             #   This file
│   ├── handlers/                       #   Per-handler API documentation
│   │   ├── index.md
│   │   ├── as5047u_handler.md
│   │   ├── bno08x_handler.md
│   │   ├── pca9685_handler.md
│   │   ├── pcal95555_handler.md
│   │   ├── ntc_temperature_handler.md
│   │   ├── tmc9660_handler.md
│   │   ├── tmc5160_handler.md
│   │   ├── tle92466ed_handler.md
│   │   ├── max22200_handler.md
│   │   ├── ws2812_handler.md
│   │   └── logger.md
│   ├── utils/                          #   Utility library documentation
│   │   ├── index.md
│   │   ├── general_utilities.md
│   │   ├── rtos_wrappers.md
│   │   └── canopen.md
│   └── testing/                        #   Testing infrastructure docs
│       ├── testing_guide.md
│       └── ci_pipelines.md
│
├── examples/                           # Platform-specific test projects
│   └── esp32/                          #   ESP-IDF test project
│       ├── CMakeLists.txt              #     Root CMake (APP_TYPE driven)
│       ├── app_config.yml              #     Build matrix configuration
│       ├── sdkconfig.defaults          #     ESP-IDF defaults
│       ├── components/
│       │   └── hf_core/               #     ESP-IDF component wrapper
│       │       ├── CMakeLists.txt
│       │       └── idf_component.yml
│       ├── main/
│       │   ├── CMakeLists.txt          #     Source selection by APP_TYPE
│       │   ├── TestFramework.h         #     Shared test macros
│       │   ├── esp32_bus_setup.hpp     #     Shared bus factories
│       │   ├── esp32_test_config.hpp   #     Pin/address constants
│       │   ├── handler_tests/          #     Handler test apps (10 files)
│       │   ├── utils_tests/            #     Utility test apps (4 files)
│       │   └── integration_tests/      #     System integration test
│       └── scripts/                    #     Build tools (git submodule)
│
├── handlers/                           # Handler source code
│   ├── as5047u/
│   │   ├── As5047uHandler.cpp
│   │   └── As5047uHandler.h
│   ├── bno08x/
│   │   ├── Bno08xHandler.cpp
│   │   └── Bno08xHandler.h
│   ├── common/
│   │   └── HandlerCommon.h
│   ├── logger/
│   │   ├── Logger.cpp
│   │   └── Logger.h
│   ├── max22200/
│   │   ├── Max22200Handler.cpp
│   │   └── Max22200Handler.h
│   ├── ntc/
│   │   ├── NtcTemperatureHandler.cpp
│   │   └── NtcTemperatureHandler.h
│   ├── pca9685/
│   │   ├── Pca9685Handler.cpp
│   │   └── Pca9685Handler.h
│   ├── pcal95555/
│   │   ├── Pcal95555Handler.cpp
│   │   └── Pcal95555Handler.h
│   ├── tle92466ed/
│   │   ├── Tle92466edHandler.cpp
│   │   └── Tle92466edHandler.h
│   ├── tmc5160/
│   │   ├── Tmc5160Handler.cpp
│   │   └── Tmc5160Handler.h
│   ├── tmc9660/
│   │   ├── Tmc9660AdcWrapper.cpp
│   │   ├── Tmc9660AdcWrapper.h
│   │   ├── Tmc9660Handler.cpp
│   │   └── Tmc9660Handler.h
│   └── ws2812/
│       ├── Ws2812Handler.cpp
│       └── Ws2812Handler.h
│
├── hf-core-drivers/                    # Driver libraries (git submodule)
│   ├── external/                       #   CRTP device drivers
│   │   ├── hf-as5047u-driver/
│   │   ├── hf-bno08x-driver/
│   │   ├── hf-max22200-driver/
│   │   ├── hf-ntc-thermistor-driver/
│   │   ├── hf-pca9685-driver/
│   │   ├── hf-pcal95555-driver/
│   │   ├── hf-tle92466ed-driver/
│   │   ├── hf-tmc5160-driver/
│   │   ├── hf-tmc9660-driver/
│   │   └── hf-ws2812-rmt-driver/
│   └── internal/                       #   Platform abstractions
│       ├── hf-internal-interface-wrap/  #     Base interfaces + ESP32 impls
│       └── hf-pincfg/                  #     Pin configuration
│
├── hf-core-utils/                      # Utility libraries (git submodule)
│   ├── hf-utils-general/               #   Header-only data structures & algorithms
│   ├── hf-utils-rtos-wrap/             #   FreeRTOS C++ wrappers
│   └── hf-utils-canopen/               #   CANopen protocol helpers
│
└── README.md                           # Module overview
```

## Key Concepts

### APP_TYPE Build System

The `examples/esp32/` project uses `APP_TYPE` to select which test application to build.
Each app maps to a single `.cpp` source file:

```bash
# Build a specific test
./build_app.sh bno08x_handler_test Debug
```

The `app_config.yml` file defines all available apps, their source files, categories,
and CI enablement.

### Component Wrapper

The `components/hf_core/CMakeLists.txt` wraps the entire hf-core platform as a single
ESP-IDF component, collecting:
- All handler `.cpp` files
- All external driver source files
- All utility library source files  
- All ESP32 platform implementation files

This allows any test app to `#include` any handler or utility directly.

### Git Submodules

Both `hf-core-drivers/` and `hf-core-utils/` are git submodules. Clone recursively:

```bash
git clone --recursive <repo-url>
# or
git submodule update --init --recursive
```
