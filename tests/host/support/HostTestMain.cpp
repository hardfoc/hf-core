/**
 * @file HostTestMain.cpp
 * @brief Runner for HF_TEST cases: runs all, or those whose name contains argv[1].
 *
 * @copyright HardFOC
 */

#include "HostTest.h"

#include "FakeClock.h"

#include <cstring>

namespace hf::test {

namespace {
int g_failures = 0;
}  // namespace

std::vector<TestCase>& Registry() {
    static std::vector<TestCase> cases;
    return cases;
}

void ReportFailure(const char* file, int line, const char* expr) {
    ++g_failures;
    std::printf("  FAIL %s:%d: %s\n", file, line, expr);
}

void ReportFailureValues(const char* file, int line, const char* expr, const char* lhs, const char* rhs) {
    ++g_failures;
    std::printf("  FAIL %s:%d: %s\n       got %s, expected %s\n", file, line, expr, lhs, rhs);
}

}  // namespace hf::test

int main(int argc, char** argv) {
    const char* filter = (argc > 1) ? argv[1] : nullptr;
    int run = 0;
    int failed_cases = 0;
    for (const auto& tc : hf::test::Registry()) {
        if (filter != nullptr && std::strstr(tc.name, filter) == nullptr) {
            continue;
        }
        hf::host::FakeClock::Reset();
        const int before = hf::test::g_failures;
        bool required_failed = false;
        try {
            tc.fn();
        } catch (const hf::test::RequireFailed&) {
            required_failed = true;  // the failure itself was reported by REQUIRE
        }
        if (required_failed) {
            std::printf("  (case stopped at a failed REQUIRE)\n");
        }
        ++run;
        const bool ok = hf::test::g_failures == before;
        if (!ok) {
            ++failed_cases;
        }
        std::printf("[%s] %s\n", ok ? "  OK  " : " FAIL ", tc.name);
    }
    std::printf("%d case(s), %d failed\n", run, failed_cases);
    if (run == 0) {
        std::printf("no test cases matched\n");
        return 1;
    }
    return failed_cases == 0 ? 0 : 1;
}
