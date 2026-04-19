/**
 * @file AlicatBasis2Handler.cpp
 * @brief Implementation of AlicatBasis2Handler.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */

#include "AlicatBasis2Handler.h"

#include <cstring>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace al = alicat_basis2;

//==============================================================================
// HalUartAlicatBasis2Comm
//==============================================================================

void HalUartAlicatBasis2Comm::write(const std::uint8_t* data,
                                    std::size_t length) noexcept {
    if (length == 0 || data == nullptr) return;
    (void)uart_.Write(data, static_cast<hf_u16_t>(length));
}

std::size_t HalUartAlicatBasis2Comm::read(std::uint8_t* out, std::size_t max,
                                          std::uint32_t timeout_ms) noexcept {
    if (out == nullptr || max == 0) return 0;
    // BaseUart::Read returns an error code; on success it has filled `max`
    // bytes (see BaseUart contract).
    const auto rc = uart_.Read(out, static_cast<hf_u16_t>(max),
                               static_cast<hf_u32_t>(timeout_ms));
    return (rc == hf_uart_err_t::UART_SUCCESS) ? max : 0;
}

void HalUartAlicatBasis2Comm::flush_rx() noexcept {
    (void)uart_.FlushRx();
}

void HalUartAlicatBasis2Comm::delay_ms_impl(std::uint32_t ms) noexcept {
    if (ms == 0) return;
    vTaskDelay(pdMS_TO_TICKS(ms));
}

//==============================================================================
// AlicatBasis2Handler
//==============================================================================

AlicatBasis2Handler::AlicatBasis2Handler(BaseUart& uart,
                                         const AlicatBasis2HandlerConfig& config,
                                         RtosMutex* bus_mutex) noexcept
    : config_(config),
      comm_(uart),
      flow_decimals_(config.flow_decimals_default),
      bus_mutex_(bus_mutex == nullptr ? &private_mutex_ : bus_mutex) {
    driver_ = std::make_unique<DriverType>(comm_, config_.modbus_address,
                                           config_.timeout_ms);
}

bool AlicatBasis2Handler::EnsureInitialized() noexcept {
    if (initialized_.load(std::memory_order_acquire)) return true;
    MutexLockGuard lock(*bus_mutex_);
    return EnsureInitializedLocked();
}

bool AlicatBasis2Handler::EnsureInitializedLocked() noexcept {
    if (initialized_.load(std::memory_order_acquire)) return true;
    if (!driver_) return false;

    auto rc = driver_->ReadIdentity();
    if (!rc.ok()) return false;
    identity_      = rc.value;
    flow_decimals_ = config_.flow_decimals_default;  // datasheet: not reported via Modbus
    initialized_.store(true, std::memory_order_release);
    return true;
}

al::DriverResult<al::InstantaneousData>
AlicatBasis2Handler::ReadInstantaneous() noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return al::DriverResult<al::InstantaneousData>::failure(al::DriverError::NotInitialized);
    }
    return driver_->ReadInstantaneous(flow_decimals_);
}

al::DriverResult<al::InstrumentIdentity>
AlicatBasis2Handler::RereadIdentity() noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!driver_) {
        return al::DriverResult<al::InstrumentIdentity>::failure(al::DriverError::NotInitialized);
    }
    auto rc = driver_->ReadIdentity();
    if (rc.ok()) identity_ = rc.value;
    return rc;
}

#define BASIS2_FORWARD_VOID(method, ...)                                       \
    do {                                                                       \
        MutexLockGuard lock(*bus_mutex_);                                      \
        if (!EnsureInitializedLocked()) {                                      \
            return al::DriverResult<void>::failure(al::DriverError::NotInitialized); \
        }                                                                      \
        return driver_->method(__VA_ARGS__);                                   \
    } while (0)

#define BASIS2_FORWARD_T(T, method, ...)                                       \
    do {                                                                       \
        MutexLockGuard lock(*bus_mutex_);                                      \
        if (!EnsureInitializedLocked()) {                                      \
            return al::DriverResult<T>::failure(al::DriverError::NotInitialized); \
        }                                                                      \
        return driver_->method(__VA_ARGS__);                                   \
    } while (0)

al::DriverResult<void> AlicatBasis2Handler::Tare() noexcept              { BASIS2_FORWARD_VOID(Tare); }
al::DriverResult<void> AlicatBasis2Handler::SetGas(al::Gas g) noexcept   { BASIS2_FORWARD_VOID(SetGas, g); }

al::DriverResult<void> AlicatBasis2Handler::SetSetpoint(float user_units) noexcept {
    BASIS2_FORWARD_VOID(SetSetpoint, user_units, flow_decimals_);
}

al::DriverResult<void> AlicatBasis2Handler::SetSetpointSource(al::SetpointSource s) noexcept {
    BASIS2_FORWARD_VOID(SetSetpointSource, s);
}

al::DriverResult<void> AlicatBasis2Handler::SetCommWatchdogMs(std::uint16_t ms) noexcept {
    BASIS2_FORWARD_VOID(SetCommWatchdogMs, ms);
}

al::DriverResult<void> AlicatBasis2Handler::SetMaxSetpointRamp(std::uint32_t pct_per_ms_x_10e7) noexcept {
    BASIS2_FORWARD_VOID(SetMaxSetpointRamp, pct_per_ms_x_10e7);
}

al::DriverResult<void> AlicatBasis2Handler::SetAutotareEnabled(bool enabled) noexcept {
    BASIS2_FORWARD_VOID(SetAutotareEnabled, enabled);
}

al::DriverResult<void> AlicatBasis2Handler::ResetTotalizer() noexcept {
    BASIS2_FORWARD_VOID(ResetTotalizer);
}

al::DriverResult<void> AlicatBasis2Handler::SetTotalizerLimitMode(al::TotalizerLimitMode m) noexcept {
    BASIS2_FORWARD_VOID(SetTotalizerLimitMode, m);
}

al::DriverResult<void> AlicatBasis2Handler::SetTotalizerBatch(std::uint32_t value_scaled) noexcept {
    BASIS2_FORWARD_VOID(SetTotalizerBatch, value_scaled);
}

al::DriverResult<void> AlicatBasis2Handler::SetFlowAveragingMs(std::uint16_t ms) noexcept {
    BASIS2_FORWARD_VOID(SetFlowAveragingMs, ms);
}

al::DriverResult<void> AlicatBasis2Handler::SetReferenceTemperatureC(float c) noexcept {
    BASIS2_FORWARD_VOID(SetReferenceTemperatureC, c);
}

al::DriverResult<void> AlicatBasis2Handler::ConfigureMeasurementTrigger(std::uint16_t bits) noexcept {
    BASIS2_FORWARD_VOID(ConfigureMeasurementTrigger, bits);
}

al::DriverResult<void> AlicatBasis2Handler::StartMeasurementSamples(std::uint16_t n) noexcept {
    BASIS2_FORWARD_VOID(StartMeasurementSamples, n);
}

al::DriverResult<al::MeasurementData> AlicatBasis2Handler::ReadMeasurement() noexcept {
    BASIS2_FORWARD_T(al::MeasurementData, ReadMeasurement);
}

al::DriverResult<void> AlicatBasis2Handler::SetModbusAddress(std::uint8_t addr) noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return al::DriverResult<void>::failure(al::DriverError::NotInitialized);
    }
    auto rc = driver_->SetModbusAddress(addr);
    if (rc.ok()) {
        config_.modbus_address = addr;
    }
    return rc;
}

al::DriverResult<void> AlicatBasis2Handler::SetAsciiUnitId(char id) noexcept {
    BASIS2_FORWARD_VOID(SetAsciiUnitId, id);
}

al::DriverResult<void> AlicatBasis2Handler::SetBaudRate(al::BaudRate br) noexcept {
    BASIS2_FORWARD_VOID(SetBaudRate, br);
}

al::DriverResult<void> AlicatBasis2Handler::FactoryRestore() noexcept {
    BASIS2_FORWARD_VOID(FactoryRestore);
}

al::DriverResult<std::uint8_t>
AlicatBasis2Handler::Discover(std::uint8_t* present_bitmap, std::size_t bitmap_bytes,
                              std::uint16_t probe_timeout_ms) noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!driver_) {
        return al::DriverResult<std::uint8_t>::failure(al::DriverError::NotInitialized);
    }
    return driver_->DiscoverPresentAddresses(present_bitmap, bitmap_bytes, probe_timeout_ms);
}

//==============================================================================
// Multi-baud discovery + bus normalisation
//==============================================================================

namespace {

constexpr std::uint32_t kAllBaudsBps[] = { 4800, 9600, 19200, 38400, 57600, 115200 };

inline al::BaudRate BaudFromBps(std::uint32_t bps) noexcept {
    switch (bps) {
        case 4800:   return al::BaudRate::Bps_4800;
        case 9600:   return al::BaudRate::Bps_9600;
        case 19200:  return al::BaudRate::Bps_19200;
        case 38400:  return al::BaudRate::Bps_38400;
        case 57600:  return al::BaudRate::Bps_57600;
        case 115200: return al::BaudRate::Bps_115200;
    }
    return al::BaudRate::Bps_38400;
}

inline bool IsSupportedBaudBps(std::uint32_t bps) noexcept {
    for (auto v : kAllBaudsBps) if (v == bps) return true;
    return false;
}

inline void SetBitmap(std::uint8_t* bm, std::size_t bytes, std::uint8_t addr) noexcept {
    if (!bm) return;
    const std::size_t idx = addr / 8;
    if (idx >= bytes) return;
    bm[idx] |= static_cast<std::uint8_t>(1U << (addr % 8));
}

}  // namespace

al::DriverResult<std::size_t>
AlicatBasis2Handler::DiscoverAcrossBauds(DiscoveredDevice* out,
                                         std::size_t       max_devices,
                                         const HostBaudSetter& set_host_baud,
                                         std::uint16_t     probe_timeout_ms,
                                         std::uint32_t     settle_ms,
                                         const std::uint32_t* baud_list_bps,
                                         std::size_t       baud_list_count) noexcept {
    if (!out || max_devices == 0 || !set_host_baud) {
        return al::DriverResult<std::size_t>::failure(al::DriverError::InvalidParameter);
    }

    const std::uint32_t* bauds  = baud_list_bps ? baud_list_bps : kAllBaudsBps;
    const std::size_t    n_baud = baud_list_bps ? baud_list_count
                                                 : (sizeof(kAllBaudsBps) / sizeof(kAllBaudsBps[0]));

    MutexLockGuard lock(*bus_mutex_);
    if (!driver_) {
        return al::DriverResult<std::size_t>::failure(al::DriverError::NotInitialized);
    }

    // Track every (addr, baud) found so we don't double-count if a device
    // somehow answers on more than one baud (shouldn't happen, but be safe).
    std::uint8_t seen_bitmap[32] = {0};
    std::size_t  found = 0;
    std::size_t  any_switch_ok = 0;

    for (std::size_t i = 0; i < n_baud; ++i) {
        const std::uint32_t bps = bauds[i];

        if (!set_host_baud(bps)) {
            // Caller declined this baud — skip silently.
            continue;
        }
        ++any_switch_ok;
        if (settle_ms) vTaskDelay(pdMS_TO_TICKS(settle_ms));
        (void)comm_.flush_rx();

        std::uint8_t present[32] = {0};
        const auto rc = driver_->DiscoverPresentAddresses(present, sizeof(present),
                                                          probe_timeout_ms);
        if (!rc.ok()) continue;  // Bus error at this baud — try the next one.

        for (std::uint16_t a = 1; a <= 247; ++a) {
            const bool here = (present[a / 8] & (1U << (a % 8))) != 0;
            const bool already = (seen_bitmap[a / 8] & (1U << (a % 8))) != 0;
            if (!here || already) continue;

            if (found >= max_devices) {
                return al::DriverResult<std::size_t>::failure(al::DriverError::BufferTooSmall);
            }
            out[found].address  = static_cast<std::uint8_t>(a);
            out[found].baud_bps = bps;
            ++found;
            SetBitmap(seen_bitmap, sizeof(seen_bitmap), static_cast<std::uint8_t>(a));
        }
    }

    if (any_switch_ok == 0) {
        return al::DriverResult<std::size_t>::failure(al::DriverError::SerialError);
    }
    return al::DriverResult<std::size_t>::success(found);
}

al::DriverResult<std::size_t>
AlicatBasis2Handler::NormalizeBusBaud(std::uint32_t           target_bps,
                                      const DiscoveredDevice* devices,
                                      std::size_t             device_count,
                                      const HostBaudSetter&   set_host_baud,
                                      std::uint8_t*           failed_bitmap,
                                      std::size_t             failed_bitmap_bytes,
                                      std::uint16_t           verify_timeout_ms) noexcept {
    if (!devices || device_count == 0 || !set_host_baud) {
        return al::DriverResult<std::size_t>::failure(al::DriverError::InvalidParameter);
    }
    if (!IsSupportedBaudBps(target_bps)) {
        return al::DriverResult<std::size_t>::failure(al::DriverError::InvalidParameter);
    }

    MutexLockGuard lock(*bus_mutex_);
    if (!driver_) {
        return al::DriverResult<std::size_t>::failure(al::DriverError::NotInitialized);
    }

    if (failed_bitmap && failed_bitmap_bytes) {
        std::memset(failed_bitmap, 0, failed_bitmap_bytes);
    }

    const std::uint8_t  saved_addr    = driver_->GetAddress();
    const std::uint16_t saved_timeout = driver_->GetTimeoutMs();
    driver_->SetTimeoutMs(verify_timeout_ms);

    std::size_t ok_count = 0;

    auto verify_at = [&](std::uint8_t addr) -> bool {
        driver_->SetAddress(addr);
        (void)comm_.flush_rx();
        return driver_->ReadIdentity().ok();
    };

    for (std::size_t i = 0; i < device_count; ++i) {
        const auto& dev = devices[i];

        // Step 1 — get host onto the device's current baud.
        if (!set_host_baud(dev.baud_bps)) {
            SetBitmap(failed_bitmap, failed_bitmap_bytes, dev.address);
            continue;
        }
        vTaskDelay(pdMS_TO_TICKS(15));
        (void)comm_.flush_rx();

        // Step 2 — if it's already at the target, just verify.
        if (dev.baud_bps == target_bps) {
            if (verify_at(dev.address)) ++ok_count;
            else SetBitmap(failed_bitmap, failed_bitmap_bytes, dev.address);
            continue;
        }

        // Step 3 — issue the baud-change command. The device switches
        // instantly so the ACK may be lost or garbled — datasheet
        // explicitly calls this out. We treat *any* return as advisory.
        driver_->SetAddress(dev.address);
        (void)driver_->SetBaudRate(BaudFromBps(target_bps));

        // Step 4 — retune host to the target baud and verify.
        vTaskDelay(pdMS_TO_TICKS(15));
        if (!set_host_baud(target_bps)) {
            SetBitmap(failed_bitmap, failed_bitmap_bytes, dev.address);
            continue;
        }
        vTaskDelay(pdMS_TO_TICKS(20));
        (void)comm_.flush_rx();

        if (verify_at(dev.address)) {
            ++ok_count;
        } else {
            SetBitmap(failed_bitmap, failed_bitmap_bytes, dev.address);
        }
    }

    // Restore the host UART to the target baud regardless of outcomes,
    // and the driver's saved address / timeout so the caller's other
    // operations are unaffected.
    (void)set_host_baud(target_bps);
    driver_->SetAddress(saved_addr);
    driver_->SetTimeoutMs(saved_timeout);

    return al::DriverResult<std::size_t>::success(ok_count);
}

#undef BASIS2_FORWARD_VOID
#undef BASIS2_FORWARD_T
