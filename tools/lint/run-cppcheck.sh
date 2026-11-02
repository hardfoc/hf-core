#!/usr/bin/env bash
# Run cppcheck over hf-core's own sources through the host compilation
# database, so it sees the include paths and defines the compiler sees. Same
# file set as run-clang-tidy.sh (tools/lint/own_sources.py).
#
#   cmake --preset host-debug
#   bash tools/lint/run-cppcheck.sh build/host-debug
#
# Exits non-zero on any error/warning/performance/portability finding. The
# full report is written to cppcheck-report.txt in the repository root.
set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "${HERE}/../.." && pwd)"
cd "${ROOT}"

DB="${1:-build/host-debug}"
CPPCHECK="${CPPCHECK:-cppcheck}"
if [[ ! -f "${DB}/compile_commands.json" ]]; then
  echo "[cppcheck] missing ${DB}/compile_commands.json; configure the host-debug preset first" >&2
  exit 2
fi
if ! command -v "${CPPCHECK}" >/dev/null 2>&1; then
  echo "[cppcheck] ${CPPCHECK} is not installed" >&2
  exit 2
fi

cmake --build "${DB}" --target hf_core_deps >/dev/null

FILTERED="${DB}/compile_commands.own.json"
python3 "${HERE}/own_sources.py" "${DB}/compile_commands.json" --db "${FILTERED}"
JOBS="$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 2)"
echo "[cppcheck] $("${CPPCHECK}" --version) on ${FILTERED}"

set +e
"${CPPCHECK}" --quiet --project="${FILTERED}" -j "${JOBS}" \
  --enable=warning,performance,portability --inline-suppr \
  --suppressions-list="${HERE}/cppcheck-suppressions.txt" \
  --template='{file}:{line}:{column}: {severity}: {message} [{id}]' \
  2> "${ROOT}/cppcheck-report.txt" > /dev/null
rc=$?
set -e

pattern=':[0-9]+:[0-9]+: (error|warning|performance|portability): '
grep -E "${pattern}" "${ROOT}/cppcheck-report.txt" | sed "s#^${ROOT}/##" || true
count="$(grep -cE "${pattern}" "${ROOT}/cppcheck-report.txt" || true)"
if [[ ${rc} -ne 0 && ${count} -eq 0 ]]; then
  cat "${ROOT}/cppcheck-report.txt" >&2
  echo "[cppcheck] failed to run (exit ${rc})" >&2
  exit 2
fi
echo "[cppcheck] ${count} finding(s); full output in cppcheck-report.txt"
[[ ${count} -eq 0 ]]
