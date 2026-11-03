/**
 * @file HandlerBase.h
 * @brief `hf::HandlerBase`: the one handler shape of standards/handler.md, written once.
 *
 * A handler derives from `HandlerBase<Self, Driver>` (CRTP) and gets the contract every handler
 * shares:
 *
 * - `EnsureInitialized()`: builds the driver in place (`std::optional`, no heap) and runs the
 *   handler's private `initializeLocked()`; on failure the driver is destroyed, so the next call
 *   retries from scratch.
 * - `Deinitialize()`: runs the optional private `deinitializeLocked(Driver&)`, then destroys the
 * driver.
 * - `IsInitialized()`: no lock, no I/O.
 * - `VisitDriver(fn)`: under the locks, `fn(driver)` returns an `hf::Status`; `NotInitialized`
 *   without calling `fn` before initialisation. The only way to reach the driver.
 * - `Header()` (protected): a copy of the `DiagnosticsHeader`. Each handler's `GetDiagnostics()`
 *   returns its own struct whose first member is that header.
 *
 * Typed methods use `CallDriver(fn)`: locks, initialises on demand, calls `fn(driver)` (which
 * returns an `hf::Result`) and counts the outcome.
 *
 * @code
 * class FooHandler : public hf::HandlerBase<FooHandler, foo::FOO<HalI2cFooComm>> {
 * public:
 *   explicit FooHandler(BaseI2c& i2c, hf::HandlerMutex* bus_mutex = nullptr) noexcept
 *       : HandlerBase(bus_mutex), comm_(i2c) {}
 *   ~FooHandler() noexcept { DestroyDriver(); }  // the driver refers to comm_
 *
 *   hf::Result<float> ReadTemperatureC() noexcept {
 *     return CallDriver([](Driver& d) { return ToResult(d.ReadTemperatureC()); });
 *   }
 *
 * private:
 *   friend HandlerBase;
 *   hf::Result<void> initializeLocked() noexcept {  // locks held, no driver yet
 *     if (!comm_.EnsureInitialized()) return {hf::Status::DependencyNotReady, 0};
 *     return ToResult(EmplaceDriver(comm_).Initialize());
 *   }
 *   HalI2cFooComm comm_;
 * };
 * @endcode
 *
 * Why CRTP rather than a member object: the public contract is then written once and cannot drift
 * between handlers (today's two families differ exactly there), dispatch stays static, and nothing
 * here names an MCU: only rtos-wrap, `hf_time` and `hf::Status`.
 *
 * Locking: the handler's own recursive `HandlerMutex`, then the shared bus mutex if one was given,
 * always in that order, for the whole call. `Header()` takes only the handler mutex.
 */
#pragma once

#include <atomic>
#include <cstdint>
#include <optional>
#include <type_traits>
#include <utility>

#include "HfTime.h"
#include "RtosMutex.h"
#include "hf/iiw/Status.h"

namespace hf {

/// The lock of every handler: rtos-wrap's recursive mutex.
using HandlerMutex = RtosMutex;
/// RAII guard for a `HandlerMutex`.
using HandlerLockGuard = MutexLockGuard;

/// The diagnostics header every handler reports first (standards/handler.md).
struct DiagnosticsHeader {
  bool initialized = false;       ///< `EnsureInitialized()` has succeeded.
  uint32_t ok_count = 0;          ///< Calls that returned `Ok`.
  uint32_t error_count = 0;       ///< Calls that returned anything else.
  Status last_error = Status::Ok; ///< The last failure.
  uint16_t last_error_detail = 0; ///< Its detail: the driver's `Error` value (or interface code).
  uint64_t last_error_us = 0;     ///< `hf_time::MonotonicUs()` at the last failure.
};

/**
 * @brief CRTP base of every handler.
 *
 * @tparam Derived The handler. Privately, with `friend HandlerBase;`, it defines
 *   `hf::Result<void> initializeLocked() noexcept` (required: build the driver with
 *   `EmplaceDriver()`, probe the chip, apply the configuration) and may define
 *   `hf::Result<void> deinitializeLocked(Driver&) noexcept` (put the chip in a safe state).
 * @tparam DriverT The driver type, stored in place.
 */
template <typename Derived, typename DriverT>
class HandlerBase {
public:
  using Driver = DriverT;

  HandlerBase(const HandlerBase&) = delete;
  HandlerBase& operator=(const HandlerBase&) = delete;
  HandlerBase(HandlerBase&&) = delete;
  HandlerBase& operator=(HandlerBase&&) = delete;

  /// Bring the chip up once; `Ok` at once when it already is. Nothing is cached on failure.
  Result<void> EnsureInitialized() noexcept {
    const DeviceLock lock(*this);
    if (!lock.Held()) {
      return Track(Result<void>{Status::Busy, 0});
    }
    return EnsureInitializedLocked();
  }

  /// Run the `deinitializeLocked()` hook, if any, and destroy the driver. `Ok` when not
  /// initialised.
  Result<void> Deinitialize() noexcept {
    const DeviceLock lock(*this);
    if (!lock.Held()) {
      return Track(Result<void>{Status::Busy, 0});
    }
    Driver* driver = DriverIfPresent();
    if (driver == nullptr) {
      return {};
    }
    Result<void> result{};
    if constexpr (requires(Derived& handler, Driver& d) { handler.deinitializeLocked(d); }) {
      result = self().deinitializeLocked(*driver);
    }
    DestroyDriver();
    return Track(result);
  }

  /// True once `EnsureInitialized()` has succeeded and `Deinitialize()` has not run since. No I/O.
  [[nodiscard]] bool IsInitialized() const noexcept {
    return initialized_.load(std::memory_order_acquire);
  }

  /**
   * @brief Call @p fn with the driver, under the locks.
   *
   * @p fn takes `Driver&` and returns an `hf::Status`; values leave by capture. It must not keep
   * the reference. The outcome is counted in the diagnostics.
   * @return `NotInitialized` (without calling @p fn) before initialisation, else what @p fn
   * returns.
   */
  template <typename Fn>
  Status VisitDriver(Fn&& fn) noexcept {
    static_assert(std::is_same_v<std::invoke_result_t<Fn, Driver&>, Status>,
                  "VisitDriver: fn(Driver&) must return hf::Status");
    const DeviceLock lock(*this);
    if (!lock.Held()) {
      return Track(Result<void>{Status::Busy, 0}).status;
    }
    Driver* driver = DriverIfPresent();
    if (driver == nullptr || !IsInitialized()) {
      return Track(Result<void>{Status::NotInitialized, 0}).status;
    }
    return Track(Result<void>{std::forward<Fn>(fn)(*driver), 0}).status;
  }

protected:
  /**
   * @param bus_mutex Mutex shared by every handler on the same bus (an RS-485 line, an I2C bus with
   *                  several devices), or null when the handler has the bus to itself.
   */
  // NOLINTNEXTLINE(bugprone-crtp-constructor-accessibility): the handler's constructor calls it
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

  /**
   * @brief The body of a typed method: locks, initialises on demand, calls @p fn and counts.
   * @param fn Takes `Driver&` and returns an `hf::Result<T>`.
   * @return The initialisation failure, or what @p fn returns.
   */
  template <typename Fn>
  std::invoke_result_t<Fn, Driver&> CallDriver(Fn&& fn) noexcept {
    using R = std::invoke_result_t<Fn, Driver&>;
    const DeviceLock lock(*this);
    if (!lock.Held()) {
      return Track(failure<R>(Status::Busy, 0));
    }
    const Result<void> init = EnsureInitializedLocked();
    Driver* driver = DriverIfPresent();
    if (!init.ok() || driver == nullptr) {
      // Already counted by EnsureInitializedLocked().
      return failure<R>(init.ok() ? Status::Fault : init.status, init.detail);
    }
    return Track(std::forward<Fn>(fn)(*driver));
  }

  /// Build the driver in place (inside `initializeLocked()`), replacing any previous one.
  template <typename... Args>
  Driver& EmplaceDriver(Args&&... args) noexcept {
    return driver_.emplace(std::forward<Args>(args)...);
  }

  /// Destroy the driver without the hook. A handler whose driver refers to its own members (the
  /// adapter) calls this from its destructor: those members are destroyed before this base.
  void DestroyDriver() noexcept {
    driver_.reset();
    initialized_.store(false, std::memory_order_release);
  }

  /// The driver, or null before initialisation. Only with the locks held.
  [[nodiscard]] Driver* DriverIfPresent() noexcept {
    return driver_.has_value() ? &driver_.value() : nullptr;
  }

  /// Count @p result in the diagnostics and return it unchanged.
  template <typename R>
  R Track(R result) noexcept {
    Record(result.status, result.detail);
    return result;
  }

  /// Count one outcome in the diagnostics.
  void Record(Status status, uint16_t detail) noexcept {
    const HandlerLockGuard lock(mutex_);
    if (status == Status::Ok) {
      ++diag_.ok_count;
      return;
    }
    ++diag_.error_count;
    diag_.last_error = status;
    diag_.last_error_detail = detail;
    diag_.last_error_us = hf_time::MonotonicUs();
  }

  /// A copy of the diagnostics header, for a handler's own `GetDiagnostics()`.
  [[nodiscard]] DiagnosticsHeader Header() const noexcept {
    const HandlerLockGuard lock(mutex_);
    DiagnosticsHeader header = diag_;
    header.initialized = IsInitialized();
    return header;
  }

  /// `EnsureInitialized()` for code that already holds the locks.
  Result<void> EnsureInitializedLocked() noexcept {
    if (IsInitialized()) {
      return {};
    }
    Result<void> result = self().initializeLocked();
    if (result.ok() && !driver_.has_value()) {
      result = {Status::Fault, 0}; // initializeLocked() must build the driver
    }
    if (!result.ok()) {
      driver_.reset();
      return Track(result);
    }
    initialized_.store(true, std::memory_order_release);
    return Track(result);
  }

private:
  template <typename R>
  static constexpr R failure(Status status, uint16_t detail) noexcept {
    R r{};
    r.status = status;
    r.detail = detail;
    return r;
  }

  Derived& self() noexcept {
    return static_cast<Derived&>(*this);
  }

  mutable HandlerMutex mutex_;
  HandlerMutex* bus_mutex_;
  std::optional<Driver> driver_;
  std::atomic<bool> initialized_{false};
  DiagnosticsHeader diag_{};
};

} // namespace hf
