/**
 * @file HalCommBase.h
 * @brief Pieces shared by the `Hal{Bus}Comm` adapter bases: the delay hooks and the port chaining.
 *
 * A driver's port is a CRTP class whose hooks the adapter implements. The `Hal{Bus}Comm` bases implement
 * the standard hooks on top of an interface-wrap `Base*` object, so a handler's adapter only names its
 * driver's port:
 *
 * @code
 * // The driver calls Transfer(), DelayUs() and EnsureInitialized() on its port.
 * class HalSpiFooComm final : public hf::HalSpiComm<HalSpiFooComm, foo::SpiInterface<HalSpiFooComm>> {
 * public:
 *     using HalSpiComm::HalSpiComm;
 * };
 * @endcode
 *
 * The second template argument is the driver's port base: each `Hal{Bus}Comm` derives from it, so its
 * hooks hide the port's forwarding stubs and the adapter needs no `using` declarations. Bases chain:
 * `HalSpiComm<D, HalGpioComm<D, foo::SpiInterface<D>>>` adds the GPIO hooks to an SPI adapter.
 *
 * Delays go through rtos-wrap (`os_delay_msec`), the only delay used above the platform bind. A delay
 * below one millisecond is rounded up to one: the hooks promise a lower bound. An adapter that needs a
 * shorter busy-wait defines its own `DelayUs()`, which hides this one.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include "OsUtility.h"

namespace hf {

/// Default port base of a `Hal{Bus}Comm`: no driver port.
struct NoPort {};

namespace comm {

/// Block for at least @p ms milliseconds (rtos-wrap; yields to other tasks).
inline void DelayMs(std::uint32_t ms) noexcept {
    while (ms > 0U) {
        const std::uint32_t chunk = ms > 0xFFFFU ? 0xFFFFU : ms;
        os_delay_msec(static_cast<std::uint16_t>(chunk));
        ms -= chunk;
    }
}

/// Block for at least @p us microseconds, rounded up to whole milliseconds.
inline void DelayUs(std::uint32_t us) noexcept {
    if (us == 0U) {
        return;
    }
    DelayMs(us / 1000U + ((us % 1000U) != 0U ? 1U : 0U));
}

/// Largest length a `Base*` transfer takes (`hf_u16_t`).
inline constexpr std::size_t kMaxTransfer = 0xFFFFU;

} // namespace comm

/**
 * @brief The delay hooks, as members, for the adapter bases to inherit.
 * @tparam PortBase The driver's port base (or `NoPort`).
 */
template <typename PortBase>
class HalDelayHooks : public PortBase {
public:
    /// Block for at least @p us microseconds (rounded up to whole milliseconds).
    void DelayUs(std::uint32_t us) noexcept {
        comm::DelayUs(us);
    }
    /// Block for at least @p ms milliseconds.
    void DelayMs(std::uint32_t ms) noexcept {
        comm::DelayMs(ms);
    }

protected:
    HalDelayHooks() noexcept = default;
    ~HalDelayHooks() = default;
};

} // namespace hf
