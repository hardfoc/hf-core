#!/usr/bin/env bash
# Run tools/lint/host.clang-tidy over hf-core's own sources (handlers/,
# tests/host/) using the host compilation database.
#
#   cmake --preset host-debug
#   bash tools/lint/run-clang-tidy.sh build/host-debug
#
# Exits non-zero on any finding. The full report is written to
# clang-tidy-report.txt in the repository root.
set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "${HERE}/../.." && pwd)"
cd "${ROOT}"

DB="${1:-build/host-debug}"
if [[ ! -f "${DB}/compile_commands.json" ]]; then
  echo "[clang-tidy] missing ${DB}/compile_commands.json; configure the host-debug preset first" >&2
  exit 2
fi

CLANG_TIDY="${CLANG_TIDY:-clang-tidy}"
# Extra compiler arguments, e.g. CLANG_TIDY_EXTRA_ARG="-isysroot $(xcrun --show-sdk-path)"
# when running a Homebrew clang-tidy against an Apple clang database on macOS.
EXTRA=()
if [[ -n "${CLANG_TIDY_EXTRA_ARG:-}" ]]; then
  EXTRA=("--extra-arg=${CLANG_TIDY_EXTRA_ARG}")
fi
if ! command -v "${CLANG_TIDY}" >/dev/null 2>&1; then
  echo "[clang-tidy] ${CLANG_TIDY} is not installed" >&2
  exit 2
fi

# Generated headers (driver version headers) exist only after a build.
cmake --build "${DB}" --target hf_core_deps >/dev/null

LIST="${DB}/clang-tidy-files.txt"
python3 "${HERE}/own_sources.py" "${DB}/compile_commands.json" > "${LIST}"
JOBS="$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 2)"
echo "[clang-tidy] $("${CLANG_TIDY}" --version | grep -m1 -i version), $(wc -l < "${LIST}" | tr -d ' ') translation unit(s)"

# clang-tidy exits 0 even when it prints warnings, so count them here.
set +e
xargs -P "${JOBS}" -n 4 < "${LIST}" \
  "${CLANG_TIDY}" -p "${DB}" --quiet --config-file="${HERE}/host.clang-tidy" ${EXTRA[@]+"${EXTRA[@]}"} \
  > "${ROOT}/clang-tidy-report.txt" 2>&1
set -e

python3 - "${ROOT}/clang-tidy-report.txt" <<'PY'
import re, sys
lines = open(sys.argv[1], errors="replace").read().splitlines()
# Reports located in submodule code belong to those repositories.
SUBMODULE = ("/hf-core-drivers/", "/hf-core-utils/", "/hf_core_dep_root/")
hits = sorted({ln for ln in lines
               if re.search(r": (warning|error): .*\[[A-Za-z0-9_.,-]+\]\s*$", ln)
               and not any(s in ln.split(":", 1)[0] for s in SUBMODULE)})
for ln in hits:
    print(ln)
print(f"[clang-tidy] {len(hits)} finding(s); full output in clang-tidy-report.txt")
sys.exit(1 if hits else 0)
PY
