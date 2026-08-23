/**
 * @file At25040Handler.h
 * @brief SPI AT25040B handler — BaseSpi + optional /WP, lazy Probe.
 *
 * @details Lightweight constructor only. Do not SPI at static init; call
 *          Probe from an explicit console / HIL / manager attach path.
 *          Product identity parsing stays in AccessoryIdentityManager.
 */
#pragma once

#include "core/hf-core-drivers/external/hf-at25040-driver/inc/at25040.hpp"
#include "base/BaseSpi.h"
#include "base/BaseGpio.h"
#include "RtosMutex.h"

#include <cstdint>

class At25040SpiAdapter : public at25040::SpiInterface<At25040SpiAdapter> {
 public:
  explicit At25040SpiAdapter(BaseSpi& spi) noexcept : spi_(spi) {}

  bool transfer(const uint8_t* tx, uint8_t* rx, std::size_t len,
                uint32_t timeout_ms) {
    return spi_.Transfer(tx, rx, static_cast<uint16_t>(len), timeout_ms) ==
           hf_spi_err_t::SPI_SUCCESS;
  }

 private:
  BaseSpi& spi_;
};

class At25040Handler {
 public:
  enum class ProbeResult : uint8_t {
    NotBound = 0,
    Absent = 1,
    Present = 2,
  };

  explicit At25040Handler(BaseSpi& spi, BaseGpio* wp = nullptr) noexcept
      : adapter_(spi), device_(adapter_), wp_(wp) {}

  ProbeResult Probe() noexcept;
  uint8_t LastRdsr() const noexcept { return device_.LastRdsr(); }
  bool Read(uint16_t addr, uint8_t* dst, uint16_t len) noexcept;
  void ReleaseWriteProtect() noexcept;

 private:
  At25040SpiAdapter adapter_;
  at25040::Device<At25040SpiAdapter> device_;
  BaseGpio* wp_{nullptr};
  mutable RtosMutex mutex_{};
};
