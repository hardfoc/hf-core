/**
 * @file HostTest.h
 * @brief Minimal test harness for hf-core host tests (no dependencies).
 *
 * @details
 * Each test file defines cases with HF_TEST(name) and links
 * hf_core_host_test_main, which runs every case and returns non-zero if any
 * CHECK failed. A failed CHECK prints file:line and the expression and lets
 * the case continue; REQUIRE stops the case.
 *
 * @code
 * HF_TEST(RingBuffer_WrapsAround) {
 *     RingBuffer<int, 4> rb;
 *     CHECK(rb.empty());
 *     CHECK_EQ(rb.size(), 0u);
 * }
 * @endcode
 *
 * @copyright HardFOC
 */

#pragma once

#include <cmath>
#include <cstdio>
#include <functional>
#include <type_traits>
#include <vector>

namespace hf::test {

struct TestCase {
    const char* name;
    void (*fn)();
};

std::vector<TestCase>& Registry();
void ReportFailure(const char* file, int line, const char* expr);
void ReportFailureValues(const char* file, int line, const char* expr, const char* lhs, const char* rhs);

struct Registrar {
    Registrar(const char* name, void (*fn)()) { Registry().push_back({name, fn}); }
};

/** Thrown by REQUIRE to abort the current case. */
struct RequireFailed {};

template <typename T>
void Format(char* buf, size_t size, const T& v) {
    if constexpr (std::is_floating_point_v<T>) {
        std::snprintf(buf, size, "%g", static_cast<double>(v));
    } else if constexpr (std::is_enum_v<T>) {
        std::snprintf(buf, size, "%lld", static_cast<long long>(static_cast<std::underlying_type_t<T>>(v)));
    } else if constexpr (std::is_pointer_v<T>) {
        std::snprintf(buf, size, "%p", static_cast<const void*>(v));
    } else if constexpr (std::is_signed_v<T>) {
        std::snprintf(buf, size, "%lld", static_cast<long long>(v));
    } else if constexpr (std::is_integral_v<T>) {
        std::snprintf(buf, size, "%llu (0x%llx)", static_cast<unsigned long long>(v),
                      static_cast<unsigned long long>(v));
    } else {
        std::snprintf(buf, size, "?");
    }
}

template <typename A, typename B>
bool CheckEq(const A& a, const B& b, const char* file, int line, const char* expr) {
    if (a == b) {
        return true;
    }
    char lhs[64];
    char rhs[64];
    Format(lhs, sizeof(lhs), a);
    Format(rhs, sizeof(rhs), b);
    ReportFailureValues(file, line, expr, lhs, rhs);
    return false;
}

inline bool CheckNear(double a, double b, double tol, const char* file, int line, const char* expr) {
    if (std::fabs(a - b) <= tol) {
        return true;
    }
    char lhs[64];
    char rhs[64];
    std::snprintf(lhs, sizeof(lhs), "%g", a);
    std::snprintf(rhs, sizeof(rhs), "%g (+/- %g)", b, tol);
    ReportFailureValues(file, line, expr, lhs, rhs);
    return false;
}

}  // namespace hf::test

#define HF_TEST_CAT2(a, b) a##b
#define HF_TEST_CAT(a, b) HF_TEST_CAT2(a, b)

/** Define and register a test case. */
#define HF_TEST(name)                                                              \
    static void name();                                                            \
    static const ::hf::test::Registrar HF_TEST_CAT(hf_test_reg_, name){#name, &(name)}; \
    static void name()

#define CHECK(cond)                                                         \
    do {                                                                    \
        if (!(cond)) ::hf::test::ReportFailure(__FILE__, __LINE__, #cond);  \
    } while (0)

#define CHECK_EQ(a, b) (void)::hf::test::CheckEq((a), (b), __FILE__, __LINE__, #a " == " #b)

#define CHECK_NEAR(a, b, tol) \
    (void)::hf::test::CheckNear(static_cast<double>(a), static_cast<double>(b), (tol), __FILE__, __LINE__, #a " ~= " #b)

#define REQUIRE(cond)                                                       \
    do {                                                                    \
        if (!(cond)) {                                                      \
            ::hf::test::ReportFailure(__FILE__, __LINE__, #cond);           \
            throw ::hf::test::RequireFailed{};                              \
        }                                                                   \
    } while (0)
