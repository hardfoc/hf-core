/**
 * @file At25040Handler.h
 * @brief SPI AT25040B handler — BaseSpi + optional /WP, lazy Probe.
 *
 * @details Lightweight constructor only. Do not SPI at static init; call
 *          Probe from an explicit console / HIL / host attach path.
 *          Identity and calibration parsing stay with the caller.
 */
#pragma once

#include "core/hf-core-drivers/external/hf-at25040-driver/inc/at25040.hpp"
#include "base/BaseSpi.h"
#include "base/BaseGpio.h"
#include "RtosMutex.h"

#include <cstdint>

/**
 * @brief CRTP adapter mapping `BaseSpi::Transfer` onto `at25040::SpiInterface`.
 */
class At25040SpiAdapter : public at25040::SpiInterface<At25040SpiAdapter> {
 public:
  /** @param spi Host SPI device; must outlive this adapter. */
  explicit At25040SpiAdapter(BaseSpi& spi) noexcept : spi_(spi) {}

  /**
   * @brief Full-duplex transfer used by the AT25040 driver.
   * @param tx Transmit buffer (may be nullptr to clock zeros).
   * @param rx Receive buffer (may be nullptr to discard).
   * @param len Byte count.
   * @param timeout_ms Transfer deadline.
   * @return true when `BaseSpi::Transfer` reports success.
   */
  bool transfer(const uint8_t* tx, uint8_t* rx, std::size_t len,
                uint32_t timeout_ms) {
    return spi_.Transfer(tx, rx, static_cast<uint16_t>(len), timeout_ms) ==
           hf_spi_err_t::SPI_SUCCESS;
  }

 private:
  BaseSpi& spi_;
};

/**
 * @brief Thread-safe AT25040B EEPROM façade (`BaseSpi` + optional `/WP` GPIO).
 */
class At25040Handler {
 public:
  /** @brief Result of a non-destructive presence probe. */
  enum class ProbeResult : uint8_t {
    NotBound = 0,  ///< SPI not attached / constructor-only state.
    Absent = 1,    ///< Bus answered with no device.
    Present = 2,   ///< RDSR / identity bytes look like AT25040.
  };

  /**
   * @param spi Host SPI (CS already bound on the `BaseSpi` instance).
   * @param wp  Optional write-protect GPIO; nullptr if `/WP` is hard-wired.
   */
  explicit At25040Handler(BaseSpi& spi, BaseGpio* wp = nullptr) noexcept
      : adapter_(spi), device_(adapter_), wp_(wp) {}

  /** @brief SPI probe; does not parse product identity. */
  ProbeResult Probe() noexcept;
  /** @return Last RDSR byte captured by the driver. */
  uint8_t LastRdsr() const noexcept { return device_.LastRdsr(); }
  /**
   * @brief Read `len` bytes starting at `addr`.
   * @param addr EEPROM address.
   * @param dst  Destination buffer (must hold `len` bytes).
   * @param len  Byte count.
   * @return true on a complete read.
   */
  bool Read(uint16_t addr, uint8_t* dst, uint16_t len) noexcept;
  /**
   * @brief Page-loop write. Caller owns identity / calibration policy.
   * @param addr EEPROM address.
   * @param src  Source buffer.
   * @param len  Byte count.
   * @return true on a complete write.
   */
  bool Write(uint16_t addr, const uint8_t* src, uint16_t len) noexcept;
  /** @brief Drive `/WP` inactive so page writes can proceed. */
  void ReleaseWriteProtect() noexcept;
  /** @brief Drive `/WP` active (protect) after a write burst. */
  void ParkWriteProtect() noexcept;

 private:
  At25040SpiAdapter adapter_;
  at25040::Device<At25040SpiAdapter> device_;
  BaseGpio* wp_{nullptr};
  mutable RtosMutex mutex_{};
};
