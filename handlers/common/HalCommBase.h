/**
 * @file HalCommBase.h
 * @brief What every `Hal{Bus}CommBase` adapter base shares: the delay hooks and the port chaining.
 *
 * A driver's port is a CRTP class whose hooks the adapter implements (standards/driver.md). The
 * `Hal{Bus}CommBase` classes implement the standard hooks on an interface-wrap `Base*` object, so a
 * handler's adapter only names its driver's port:
 *
 * @code
 * class HalSpiFooComm final
 *     : public hf::HalSpiCommBase<HalSpiFooComm, foo::SpiInterface<HalSpiFooComm>> {
 * public:
 *   using HalSpiCommBase::HalSpiCommBase;
 * };
 * @endcode
 *
 * The second template argument is the driver's port base, which the adapter base derives from: its
 * hooks then also hide any forwarding stub an older port declares, so the adapter needs no `using`
 * declarations. Bases chain, `HalSpiCommBase<D, HalGpioCommBase<D, foo::SpiInterface<D>>>`, to add
 * the control-pin hooks to a bus adapter.
 *
 * A hook that fails returns `false` and keeps the interface code (`LastBusError()`), so the handler
 * can log it; the driver reports the failure as its `Error::Bus`.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include "Delay.h"
#include "hf/iiw/Status.h"

namespace hf {

/// Default port base of an adapter base: no driver port.
struct NoPort {};

/// Largest length one `Base*` transfer takes (`hf_u16_t`).
inline constexpr std::size_t kMaxTransferBytes = 0xFFFFU;

/**
 * @brief The delay hooks, for the bus adapter bases to inherit.
 * @tparam PortBase The driver's port base (or `NoPort`).
 */
template <typename PortBase>
class HalDelayHooks : public PortBase {
public:
  /// Wait at least @p us microseconds (`hf::DelayUs()`).
  void DelayUs(std::uint32_t us) noexcept {
    hf::DelayUs(us);
  }
  /// Wait at least @p ms milliseconds (`hf::DelayMs()`).
  void DelayMs(std::uint32_t ms) noexcept {
    hf::DelayMs(ms);
  }

protected:
  HalDelayHooks() noexcept = default;
  ~HalDelayHooks() = default;
};

} // namespace hf
