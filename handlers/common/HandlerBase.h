/**
 * @file HandlerBase.h
 * @brief `hf::HandlerBase`: the one handler shape (locking, start-up, driver access, diagnostics).
 *
 * A handler derives from `HandlerBase<Self, Driver>` (CRTP) and supplies one private hook; the base
 * supplies the public contract every handler shares:
 *
 * - `EnsureInitialized()`: builds the driver in place and runs the handler's `InitializeLocked()` once.
 *   A failure caches nothing, so the next call retries.
 * - `IsInitialized()`: lock-free check.
 * - `Deinitialize()`: runs the optional `DeinitializeLocked(Driver&)` hook and destroys the driver.
 * - `VisitDriver(fn)`: under the locks, after `EnsureInitialized()`, calls `fn(driver)`. It is the only
 *   way to reach the driver.
 * - `GetDiagnostics()`: the common `HandlerDiagnostics` header.
 *
 * @code
 * class FooHandler : public hf::HandlerBase<FooHandler, foo::FOO<HalI2cFooComm>> {
 * public:
 *     explicit FooHandler(BaseI2c& i2c, hf::HandlerMutex* bus_mutex = nullptr) noexcept
 *         : HandlerBase(bus_mutex), comm_(i2c) {}
 *     ~FooHandler() noexcept { DestroyDriver(); }  // the driver refers to comm_: destroy it first
 *
 *     hf::Result<float> ReadCelsius() noexcept {
 *         return VisitDriver([](Driver& d) { return ToResult(d.ReadCelsius()); });
 *     }
 *
 * private:
 *     friend HandlerBase;
 *     hf::Result<void> InitializeLocked() noexcept {      // locks held, driver not built yet
 *         if (!comm_.EnsureInitialized()) return hf::Fail(hf::Status::DependencyNotReady);
 *         return ToResult(EmplaceDriver(comm_).Initialize());
 *     }
 *     HalI2cFooComm comm_;
 * };
 * @endcode
 *
 * Why CRTP and not a member object: the public contract above is then written once and cannot drift
 * between handlers (today's two families differ exactly there), while dispatch stays static, the driver
 * stays in-place (`std::optional`, no heap) and nothing here names an MCU or an RTOS beyond rtos-wrap.
 * See docs/handler-model.md.
 *
 * Locking: every call into the driver holds the handler's own recursive `HandlerMutex` and then, when one
 * was given, the shared bus mutex, always in that order. `GetDiagnostics()` takes only the handler mutex,
 * so it never waits for a bus.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */
#pragma once

#include <atomic>
#include <cstdint>
#include <optional>
#include <type_traits>
#include <utility>

#include "HfResult.h"
#include "HfTime.h"
#include "RtosMutex.h"

namespace hf {

/// The lock every handler uses: rtos-wrap's recursive mutex.
using HandlerMutex = RtosMutex;
/// RAII guard for a `HandlerMutex`.
using HandlerLockGuard = MutexLockGuard;

/// Default clock of a handler's diagnostics: the MCU-agnostic monotonic microsecond counter.
struct MonotonicClock {
    /// Microseconds since boot.
    [[nodiscard]] static std::uint64_t NowUs() noexcept {
        return hf_time::MonotonicUs();
    }
};

/**
 * @brief The diagnostics header every handler reports.
 *
 * A handler with more to say returns its own struct whose first member is this one.
 */
struct HandlerDiagnostics {
    bool initialized{false};                    ///< `EnsureInitialized()` has succeeded.
    Status last_status{Status::NotInitialized}; ///< Outcome of the last tracked operation.
    std::uint32_t last_detail{0};               ///< Layer-native code of that outcome.
    std::uint32_t ok_count{0};                  ///< Tracked operations that succeeded.
    std::uint32_t error_count{0};               ///< Tracked operations that failed.
    std::uint32_t consecutive_errors{0};        ///< Failures since the last success.
    std::uint32_t bus_errors{0};                ///< Failures with `Status::Bus`.
    std::uint32_t timeouts{0};                  ///< Failures with `Status::Timeout`.
    std::uint32_t init_attempts{0};             ///< Times the initialisation sequence ran.
    std::uint32_t init_failures{0};             ///< Times it failed.
    std::uint64_t last_ok_us{0};                ///< `hf_time::MonotonicUs()` of the last success.
    std::uint64_t last_error_us{0};             ///< `hf_time::MonotonicUs()` of the last failure.
};

namespace detail {

template <typename R>
struct VisitResultOf {
    using type = Result<R>;
};
template <>
struct VisitResultOf<void> {
    using type = Result<void>;
};
template <typename T>
struct VisitResultOf<Result<T>> {
    using type = Result<T>;
};

} // namespace detail

/// What `VisitDriver(fn)` returns: `fn`'s `Result<T>` as is, `Result<void>` for `void`, else `Result<R>`.
template <typename Fn, typename Driver>
using VisitResult = typename detail::VisitResultOf<std::remove_cvref_t<std::invoke_result_t<Fn, Driver&>>>::type;

/**
 * @brief CRTP base of every handler.
 *
 * @tparam Derived The handler. It provides, privately with `friend HandlerBase;`:
 *   - `hf::Result<void> InitializeLocked() noexcept` (required): called with the locks held and no
 *     driver; builds it with `EmplaceDriver(...)`, probes the chip and applies the configuration;
 *   - `hf::Result<void> DeinitializeLocked(Driver&) noexcept` (optional): puts the chip in a safe state
 *     before the driver is destroyed.
 * @tparam DriverT The driver type, stored in place.
 * @tparam Clock Source of the diagnostics timestamps (`NowUs()`); host tests substitute a fake.
 */
template <typename Derived, typename DriverT, typename Clock = MonotonicClock>
class HandlerBase {
public:
    using Driver = DriverT;

    HandlerBase(const HandlerBase&) = delete;
    HandlerBase& operator=(const HandlerBase&) = delete;
    HandlerBase(HandlerBase&&) = delete;
    HandlerBase& operator=(HandlerBase&&) = delete;

    /**
     * @brief Bring the chip up once; return at once when it already is.
     * @return `Ok`, or the failure of the initialisation sequence. Nothing is cached on failure.
     */
    Result<void> EnsureInitialized() noexcept {
        if (initialized_.load(std::memory_order_acquire)) {
            return {};
        }
        const DeviceLock lock(*this);
        if (!lock.Held()) {
            return Fail(Status::Busy);
        }
        return EnsureInitializedLocked();
    }

    /// True once `EnsureInitialized()` has succeeded (and `Deinitialize()` has not run since).
    [[nodiscard]] bool IsInitialized() const noexcept {
        return initialized_.load(std::memory_order_acquire);
    }

    /**
     * @brief Run the handler's `DeinitializeLocked()` hook, if any, then destroy the driver.
     * @return The hook's result; the driver is destroyed either way. `Ok` when nothing was initialised.
     */
    Result<void> Deinitialize() noexcept {
        const DeviceLock lock(*this);
        if (!lock.Held()) {
            return Fail(Status::Busy);
        }
        if (!driver_.has_value()) {
            initialized_.store(false, std::memory_order_release);
            return {};
        }
        const Result<void> result = Self().DeinitializeLocked(*driver_);
        driver_.reset();
        initialized_.store(false, std::memory_order_release);
        return Track(result);
    }

    /**
     * @brief Call @p fn with the driver, under the locks, after `EnsureInitialized()`.
     *
     * This is the only way to reach the driver. When @p fn returns an `hf::Result`, the outcome is
     * recorded in the diagnostics.
     *
     * @return The initialisation failure, or what @p fn returns (see `VisitResult`).
     */
    template <typename Fn>
    VisitResult<Fn, Driver> VisitDriver(Fn&& fn) noexcept {
        using R = std::remove_cvref_t<std::invoke_result_t<Fn, Driver&>>;
        const DeviceLock lock(*this);
        if (!lock.Held()) {
            return Fail(Status::Busy);
        }
        const Result<void> init = EnsureInitializedLocked();
        if (!init.ok()) {
            return init.error();
        }
        if constexpr (std::is_void_v<R>) {
            std::forward<Fn>(fn)(*driver_);
            return {};
        } else if constexpr (kIsResult<R>) {
            return Track(R(std::forward<Fn>(fn)(*driver_)));
        } else {
            return Result<R>(std::forward<Fn>(fn)(*driver_));
        }
    }

    /// The common diagnostics header.
    [[nodiscard]] HandlerDiagnostics GetDiagnostics() const noexcept {
        const HandlerLockGuard lock(mutex_);
        HandlerDiagnostics d = diag_;
        d.initialized = initialized_.load(std::memory_order_acquire);
        return d;
    }

protected:
    /**
     * @param bus_mutex Mutex shared by every handler on the same bus (an RS-485 line, an I2C bus with
     *                  several devices), or null when the handler has its bus to itself.
     */
    explicit HandlerBase(HandlerMutex* bus_mutex = nullptr) noexcept : bus_mutex_(bus_mutex) {}
    ~HandlerBase() = default;

    /// Holds the handler mutex, then the bus mutex if there is one.
    class DeviceLock {
    public:
        explicit DeviceLock(const HandlerBase& handler) noexcept : own_(handler.mutex_) {
            if (handler.bus_mutex_ != nullptr) {
                bus_.emplace(*handler.bus_mutex_);
            }
        }
        /// Both mutexes were taken.
        [[nodiscard]] bool Held() const noexcept {
            return own_.IsLocked() && (!bus_ || bus_->IsLocked());
        }

    private:
        HandlerLockGuard own_;
        std::optional<HandlerLockGuard> bus_;
    };

    /// Build the driver in place (inside `InitializeLocked()`), replacing any previous one.
    template <typename... Args>
    Driver& EmplaceDriver(Args&&... args) noexcept {
        return driver_.emplace(std::forward<Args>(args)...);
    }

    /// Destroy the driver without the deinitialisation hook. Call it from the handler's destructor
    /// when the driver refers to members of the handler (its adapter): they are destroyed before this base.
    void DestroyDriver() noexcept {
        driver_.reset();
        initialized_.store(false, std::memory_order_release);
    }

    /// The driver, or null before initialisation. Only with the locks held.
    [[nodiscard]] Driver* DriverIfPresent() noexcept {
        return driver_.has_value() ? &*driver_ : nullptr;
    }

    /// Default for the optional hook: nothing to do.
    Result<void> DeinitializeLocked(Driver& /*driver*/) noexcept {
        return {};
    }

    /// Record @p result in the diagnostics and return it unchanged.
    template <typename T>
    Result<T> Track(Result<T> result) noexcept {
        Record(result.status(), result.detail());
        return result;
    }

    /// Record an outcome in the diagnostics.
    void Record(Status status, std::uint32_t detail) noexcept {
        const HandlerLockGuard lock(mutex_);
        diag_.last_status = status;
        diag_.last_detail = detail;
        const std::uint64_t now = Clock::NowUs();
        if (status == Status::Ok) {
            ++diag_.ok_count;
            diag_.consecutive_errors = 0;
            diag_.last_ok_us = now;
            return;
        }
        ++diag_.error_count;
        ++diag_.consecutive_errors;
        diag_.last_error_us = now;
        if (status == Status::Bus) {
            ++diag_.bus_errors;
        } else if (status == Status::Timeout) {
            ++diag_.timeouts;
        }
    }

    /// `EnsureInitialized()` for code that already holds the locks (inside `VisitDriver()`).
    Result<void> EnsureInitializedLocked() noexcept {
        if (initialized_.load(std::memory_order_acquire)) {
            return {};
        }
        {
            const HandlerLockGuard lock(mutex_);
            ++diag_.init_attempts;
        }
        Result<void> result = Self().InitializeLocked();
        if (result.ok() && !driver_.has_value()) {
            result = Fail(Status::Fault); // InitializeLocked() must build the driver
        }
        if (!result.ok()) {
            driver_.reset();
            {
                const HandlerLockGuard lock(mutex_);
                ++diag_.init_failures;
            }
            return Track(result);
        }
        initialized_.store(true, std::memory_order_release);
        return Track(result);
    }

private:
    Derived& Self() noexcept {
        return static_cast<Derived&>(*this);
    }

    mutable HandlerMutex mutex_;
    HandlerMutex* bus_mutex_;
    std::optional<Driver> driver_;
    std::atomic<bool> initialized_{false};
    HandlerDiagnostics diag_{};
};

} // namespace hf
