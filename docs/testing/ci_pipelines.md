---
layout: default
title: CI Pipelines
parent: Testing
nav_order: 2
---

# CI Pipelines

## Overview

hf-core uses **8 GitHub Actions workflows** that mirror the standard CI pattern across
all HardFOC repositories. Workflows use reusable workflow definitions from two shared
CI tool repositories:

| Repository | Purpose |
|:-----------|:--------|
| `hardfoc/hf-espidf-ci-tools` | ESP-IDF build, flash, release workflows |
| `hardfoc/hf-general-ci-tools` | C++ analysis, linting, documentation workflows |

All reusable workflows are pinned at `@main`.

---

## Workflow Descriptions

### 1. ESP-IDF Build (`esp32-examples-build-ci.yml`)

**Trigger:** Push/PR to `main`, `develop`, `feature/**`, `release/**`

Pipeline stages:
1. **Validate** — Checks `app_config.yml` structure and validates APP_TYPE entries
2. **Build Matrix** — `generate_matrix.py` reads `app_config.yml`, produces a matrix of
   `{app_type, build_type, idf_version}` combinations where `ci_enabled: true`
3. **Build** — Each matrix entry runs `build_app.sh` in a Docker container with ESP-IDF
4. **Lint** — Runs clang-format/clang-tidy on modified files (excludes submodule dirs)

Only apps with `ci_enabled: true` in `app_config.yml` are included in the matrix.
Currently, `canopen_utils_test` and `full_integration_test` are disabled because they
require specific hardware or network configuration.

### 2. C++ Static Analysis (`ci-cpp-analysis.yml`)

**Trigger:** Push/PR modifying `handlers/**` or `_config/**`

Runs **Cppcheck** with `--std=c++20` on the `handlers/` directory. Catches common
bugs, undefined behavior, and style issues.

### 3. C++ Lint (`ci-cpp-lint.yml`)

**Trigger:** Push/PR modifying `handlers/**` or `_config/**`

Runs **clang-format** (style check) and **clang-tidy** (static analysis) using the
configurations in `_config/`. Excludes `hf-core-drivers/` and `hf-core-utils/`
submodules from analysis.

### 4. Documentation Link Check (`ci-docs-linkcheck.yml`)

**Trigger:** Push/PR modifying `docs/**` or `README.md`

Uses **lychee** to verify all hyperlinks in documentation are reachable. Configuration
in `_config/lychee.toml` excludes known-flaky URLs (shields.io badges, GNU licenses).

### 5. Documentation Publish (`ci-docs-publish.yml`)

**Trigger:** Push to `main` (deploy), PR to `main` (build-only)

Two-stage pipeline:
1. **Jekyll** — Builds the docs site using `_config/_config.yml` (Just the Docs theme)
2. **Doxygen** — Generates API reference from handler source code, outputs to
   `docs/api/` within the Jekyll site

Deploys to GitHub Pages on push to `main`.

### 6. Markdown Lint (`ci-markdown-lint.yml`)

**Trigger:** Push/PR modifying `**/*.md`

Validates Markdown files against `.markdownlint.json` rules. Excludes submodule
directories (`hf-core-drivers/**`, `hf-core-utils/**`).

### 7. YAML Lint (`ci-yaml-lint.yml`)

**Trigger:** Push/PR modifying `**/*.yml` or `**/*.yaml`

Validates YAML files against `.yamllint` rules. Excludes submodule directories.

### 8. Release (`release.yml`)

**Trigger:** Push of a tag matching `v*`

Creates a GitHub Release with:
- Auto-generated changelog from commits since the last tag
- Source archive attached as a release artifact
- Release notes template

---

## Build Matrix Generation

The build matrix is generated from `app_config.yml` by `generate_matrix.py`:

```yaml
# app_config.yml excerpt
apps:
  handler_testing:
    as5047u_handler_test:
      source_file: "handler_tests/as5047u_handler_comprehensive_test.cpp"
      ci_enabled: true
      description: "AS5047U magnetic encoder handler tests"
```

The script produces a JSON matrix like:

```json
[
  {"app_type": "as5047u_handler_test", "build_type": "debug", "idf_version": "v5.5"},
  {"app_type": "as5047u_handler_test", "build_type": "release", "idf_version": "v5.4"}
]
```

---

## Running CI Locally

You can simulate the CI build locally:

```bash
cd examples/esp32/scripts

# Build a single app
./build_app.sh --app general_utils_test --build-type debug

# Build all CI-enabled apps
./build_app.sh --all --build-type debug

# Generate the matrix (for debugging)
python3 generate_matrix.py ../app_config.yml
```

---

## Adding a New CI-Tested App

1. Add the app entry to `app_config.yml` with `ci_enabled: true`
2. Create the source file under the appropriate `main/` subdirectory
3. Add the `APP_TYPE` conditional to `main/CMakeLists.txt`
4. Push — the CI will automatically pick up the new app in the build matrix
