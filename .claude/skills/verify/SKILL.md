---
name: verify
description: Run every check hf-core's CI runs, locally, before pushing. Use before every push and before opening a PR.
---

1. Submodules at the pinned commits: `git submodule update --init --recursive`.
2. Host build and tests (Clang or Apple Clang):
   `cmake --preset host-debug && cmake --build --preset host-debug && ctest --preset host-debug`.
3. The same with GCC when it is installed:
   `CC=gcc CXX=g++ cmake -S . -B build/gcc -G Ninja && cmake --build build/gcc && ctest --test-dir build/gcc`.
4. clang-tidy gate: `bash tools/lint/run-clang-tidy.sh build/host-debug` (set `CLANG_TIDY` to a clang-tidy 18 or
   newer). On macOS with Homebrew LLVM, run it per file with
   `--extra-arg=-isysroot$(xcrun --show-sdk-path) --extra-arg=-isystem$(xcrun --show-sdk-path)/usr/include/c++/v1`.
5. cppcheck gate: `bash tools/lint/run-cppcheck.sh build/host-debug`.
6. Formatting of the files you added: `clang-format --dry-run --style=file:<org .clang-format> <files>` with the
   config from `hardfoc/.github/standards/config/`. Do not reformat files you did not otherwise change.
7. Docs, when `docs/`, `handlers/`, `cmake/` or `templates/` changed, in a venv with
   `pip install "git+https://github.com/hardfoc/hf-general-ci-tools@v1#subdirectory=packages/hardfoc-mkdocs-theme"`:
   `hardfoc-docs gen` (commit what it rewrites), `hardfoc-docs check`, `mkdocs build --strict -d /tmp/site`.
   `hardfoc-docs check` can report figure errors from the submodules' own `docs/` in a full checkout; those
   pages are not part of this site, and `hardfoc-docs gen --check` must still pass.
8. Remove the build directories (`build/`, `/tmp/site`) and `clang-tidy-report.txt` / `cppcheck-report.txt`.

The ESP32 examples are checked by CI (`🚀 ESP32 Examples Build CI`); do not run ESP-IDF or Docker builds unless the
task asks for one.
