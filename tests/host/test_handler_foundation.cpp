/**
 * @file test_handler_foundation.cpp
 * @brief HandlerBase, the Hal{Bus}CommBase adapter bases and hf::DelayUs() over the host mocks.
 *
 * hf::Status, hf::Result and hf::ToStatus() are interface-wrap's and tested there.
 */

#include <cstdint>
#include <type_traits>

#include "Delay.h"
#include "HalAdcCommBase.h"
#include "HalGpioCommBase.h"
#include "HalI2cCommBase.h"
#include "HalSpiCommBase.h"
#include "HalUartCommBase.h"
#include "HandlerBase.h"
#include "HfCoreMocks.h"
#include "HostTest.h"

using hf::Result;
using hf::Status;
using hf::host::FakeClock;

// ── hf::DelayUs() / hf::DelayMs() ──────────────────────────────────────────────

HF_TEST(Delay_ShortWaitsBusyWaitOnTheClock) {
  FakeClock::Reset();
  const uint64_t start = hf_time::MonotonicUs();
  hf::DelayUs(250);
  CHECK(hf_time::MonotonicUs() - start >= 250U);
  CHECK_EQ(FakeClock::DelayCount(), 0U); // no RTOS call below 1 ms
}

HF_TEST(Delay_LongWaitsRoundUpToWholeMilliseconds) {
  FakeClock::Reset();
  hf::DelayUs(2500);
  CHECK_EQ(FakeClock::TotalDelayUs(), 3000U); // a 2.5 ms minimum never becomes 2 ms
  hf::DelayUs(0);
  hf::DelayMs(0);
  CHECK_EQ(FakeClock::TotalDelayUs(), 3000U);
  hf::DelayMs(70000); // more than one os_delay_msec() call takes
  CHECK_EQ(FakeClock::TotalDelayUs(), 3000U + 70000000U);
}

// ── HandlerBase ────────────────────────────────────────────────────────────────

namespace {

struct FakeDriver {
  explicit FakeDriver(int& constructed) noexcept {
    ++constructed;
  }
  int reads{0};
};

class FakeHandler : public hf::HandlerBase<FakeHandler, FakeDriver> {
public:
  explicit FakeHandler(hf::HandlerMutex* bus_mutex = nullptr) noexcept : HandlerBase(bus_mutex) {}
  ~FakeHandler() noexcept {
    DestroyDriver();
  }

  [[nodiscard]] hf::DiagnosticsHeader GetDiagnostics() const noexcept {
    return Header();
  }

  Result<int> Read() noexcept {
    return CallDriver([this](FakeDriver& d) -> Result<int> {
      if (fail_reads) {
        return {0, Status::Bus, 7};
      }
      return {++d.reads};
    });
  }

  Status init_status{Status::Ok};
  bool fail_reads{false};
  int constructed{0};
  int init_calls{0};
  int deinit_calls{0};

private:
  friend HandlerBase;

  Result<void> initializeLocked() noexcept {
    ++init_calls;
    EmplaceDriver(constructed);
    return {init_status, init_status == Status::Ok ? uint16_t{0} : uint16_t{99}};
  }

  Result<void> deinitializeLocked(FakeDriver& /*driver*/) noexcept {
    ++deinit_calls;
    return {};
  }
};

class NoHookHandler : public hf::HandlerBase<NoHookHandler, FakeDriver> {
public:
  NoHookHandler() noexcept = default;
  ~NoHookHandler() noexcept {
    DestroyDriver();
  }

private:
  friend HandlerBase;
  Result<void> initializeLocked() noexcept {
    EmplaceDriver(constructed_);
    return {};
  }
  int constructed_{0};
};

class ForgetfulHandler : public hf::HandlerBase<ForgetfulHandler, FakeDriver> {
private:
  friend HandlerBase;
  Result<void> initializeLocked() noexcept {
    return {}; // forgets EmplaceDriver()
  }
};

} // namespace

HF_TEST(HandlerBase_ConstructionDoesNothingUntilFirstUse) {
  FakeHandler h;
  CHECK(!h.IsInitialized());
  CHECK_EQ(h.constructed, 0);
  CHECK_EQ(h.init_calls, 0);
  const hf::DiagnosticsHeader d = h.GetDiagnostics();
  CHECK(!d.initialized);
  CHECK_EQ(d.ok_count + d.error_count, 0U);
}

HF_TEST(HandlerBase_FailedInitCachesNothingAndRetries) {
  FakeHandler h;
  h.init_status = Status::NotFound;
  const auto r = h.EnsureInitialized();
  CHECK_EQ(r.status, Status::NotFound);
  CHECK_EQ(r.detail, 99U);
  CHECK(!h.IsInitialized());
  CHECK_EQ(h.Read().status, Status::NotFound); // a typed call retries and reports the same
  CHECK_EQ(h.init_calls, 2);

  h.init_status = Status::Ok;
  CHECK(h.EnsureInitialized().ok());
  CHECK(h.IsInitialized());
  CHECK(h.EnsureInitialized().ok());
  CHECK_EQ(h.init_calls, 3); // the successful sequence runs once
  CHECK_EQ(h.constructed, 3);

  const auto d = h.GetDiagnostics();
  CHECK(d.initialized);
  CHECK_EQ(d.error_count, 2U);
  CHECK_EQ(d.ok_count, 1U);
  CHECK_EQ(d.last_error, Status::NotFound);
  CHECK_EQ(d.last_error_detail, 99U);
}

HF_TEST(HandlerBase_VisitDriverRefusesBeforeInitAndCounts) {
  FakeHandler h;
  bool called = false;
  CHECK_EQ(h.VisitDriver([&](FakeDriver&) {
    called = true;
    return Status::Ok;
  }),
           Status::NotInitialized);
  CHECK(!called);
  CHECK_EQ(h.init_calls, 0); // VisitDriver() never initialises

  REQUIRE(h.EnsureInitialized().ok());
  int reads = -1;
  CHECK_EQ(h.VisitDriver([&](FakeDriver& d) {
    reads = d.reads;
    return Status::Ok;
  }),
           Status::Ok);
  CHECK_EQ(reads, 0);
  CHECK_EQ(h.VisitDriver([](FakeDriver&) { return Status::Timeout; }), Status::Timeout);

  const auto d = h.GetDiagnostics();
  CHECK_EQ(d.ok_count, 2U);    // the init and one visit
  CHECK_EQ(d.error_count, 2U); // the refused visit and the timeout
  CHECK_EQ(d.last_error, Status::Timeout);
}

HF_TEST(HandlerBase_TypedCallsTrackTheResult) {
  FakeClock::Reset();
  FakeHandler h;
  const auto first = h.Read();
  REQUIRE(first.ok());
  CHECK_EQ(first.value, 1);

  h.fail_reads = true;
  FakeClock::AdvanceUs(5000);
  const auto bad = h.Read();
  CHECK_EQ(bad.status, Status::Bus);
  CHECK_EQ(bad.detail, 7U);

  const auto d = h.GetDiagnostics();
  CHECK_EQ(d.ok_count, 2U); // the init and the first read
  CHECK_EQ(d.error_count, 1U);
  CHECK_EQ(d.last_error, Status::Bus);
  CHECK_EQ(d.last_error_detail, 7U);
  CHECK(d.last_error_us >= 5000U);
}

HF_TEST(HandlerBase_DeinitializeRunsTheHookAndAllowsReinit) {
  hf::HandlerMutex bus;
  FakeHandler h(&bus);
  CHECK(h.Deinitialize().ok()); // nothing to undo
  CHECK_EQ(h.deinit_calls, 0);
  REQUIRE(h.EnsureInitialized().ok());
  CHECK(h.Deinitialize().ok());
  CHECK_EQ(h.deinit_calls, 1);
  CHECK(!h.IsInitialized());
  CHECK(h.Read().ok());
  CHECK_EQ(h.init_calls, 2);

  NoHookHandler n;
  REQUIRE(n.EnsureInitialized().ok());
  CHECK(n.Deinitialize().ok());
  CHECK(!n.IsInitialized());
}

HF_TEST(HandlerBase_InitThatBuildsNoDriverIsAFault) {
  ForgetfulHandler f;
  CHECK_EQ(f.EnsureInitialized().status, Status::Fault);
  CHECK(!f.IsInitialized());
}

// ── Hal{Bus}CommBase ───────────────────────────────────────────────────────────

namespace {

// A port that still declares forwarding stubs (today's drivers): the adapter base must hide them.
template <typename Derived>
struct LegacySpiPort { // NOLINT(bugprone-crtp-constructor-accessibility): built by the chain
  bool Transfer(const uint8_t* tx, uint8_t* rx, std::size_t len) noexcept {
    return static_cast<Derived*>(this)->Transfer(tx, rx, len);
  }
  void DelayUs(uint32_t us) noexcept {
    static_cast<Derived*>(this)->DelayUs(us);
  }
};

enum class CtrlPin : uint8_t { Reset, Wake };
enum class GpioSignal : uint8_t { INACTIVE, ACTIVE };

class SpiAdapter final
    : public hf::HalSpiCommBase<SpiAdapter,
                                hf::HalGpioCommBase<SpiAdapter, LegacySpiPort<SpiAdapter>>> {
public:
  static constexpr uint32_t kBusTimeoutMs = 5;
  SpiAdapter(BaseSpi& spi, BaseGpio& reset) noexcept : HalSpiCommBase(spi), reset_(reset) {}
  BaseGpio* GpioFor(CtrlPin pin) noexcept {
    return pin == CtrlPin::Reset ? &reset_ : nullptr;
  }

private:
  BaseGpio& reset_;
};

class I2cAdapter final : public hf::HalI2cCommBase<I2cAdapter> {
public:
  static constexpr std::size_t kMaxRegisterWrite = 4;
  using HalI2cCommBase::HalI2cCommBase;
};

class UartAdapter final : public hf::HalUartCommBase<UartAdapter> {
public:
  using HalUartCommBase::HalUartCommBase;
};

class AdcAdapter final : public hf::HalAdcCommBase<AdcAdapter> {
public:
  using HalAdcCommBase::HalAdcCommBase;
};

} // namespace

HF_TEST(HalSpiCommBase_TransfersThroughTheDriverPortAndKeepsTheBusCode) {
  hf::host::MockSpi spi;
  hf::host::MockGpio reset(1, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT);
  SpiAdapter port(spi, reset);
  REQUIRE(port.EnsureInitialized());

  spi.QueueResponse({0xAB, 0xCD});
  const uint8_t tx[2] = {0x01, 0x02};
  uint8_t rx[2] = {};
  auto& driver_view = static_cast<LegacySpiPort<SpiAdapter>&>(port); // how a driver calls it
  REQUIRE(driver_view.Transfer(tx, rx, 2));
  CHECK_EQ(rx[0], 0xAB);
  CHECK_EQ(rx[1], 0xCD);

  spi.FailNext(hf_spi_err_t::SPI_ERR_TRANSFER_TIMEOUT);
  CHECK(!port.Write(tx, 2));
  CHECK_EQ(port.LastBusError(), hf_spi_err_t::SPI_ERR_TRANSFER_TIMEOUT);
  CHECK_EQ(port.LastBusResult().status, Status::Timeout);
  CHECK(!port.Read(rx, 0x10000));
  CHECK_EQ(port.LastBusError(), hf_spi_err_t::SPI_ERR_TRANSFER_TOO_LONG);
  port.ClearLastBusError();
  CHECK(port.LastBusResult().ok());

  FakeClock::Reset();
  driver_view.DelayUs(1500);
  CHECK_EQ(FakeClock::TotalDelayUs(), 2000U);
}

HF_TEST(HalSpiCommBase_BusThatWillNotStartFailsEnsureInitialized) {
  hf::host::MockSpi spi;
  spi.SetInitializeResult(false);
  hf::host::MockGpio reset;
  SpiAdapter port(spi, reset);
  CHECK(!port.EnsureInitialized());
  CHECK_EQ(port.LastBusResult().status, Status::NotInitialized);
}

HF_TEST(HalGpioCommBase_DrivesLogicalPinsAndRefusesUnwiredOnes) {
  hf::host::MockSpi spi;
  hf::host::MockGpio reset(1, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT,
                           hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
  SpiAdapter port(spi, reset);
  REQUIRE(port.GpioSet(CtrlPin::Reset, GpioSignal::ACTIVE));
  CHECK_EQ(reset.LevelHistory().back(), hf_gpio_level_t::HF_GPIO_LEVEL_LOW); // active-low
  GpioSignal sig = GpioSignal::INACTIVE;
  REQUIRE(port.GpioRead(CtrlPin::Reset, sig));
  CHECK(sig == GpioSignal::ACTIVE);
  REQUIRE(port.GpioSet(CtrlPin::Reset, GpioSignal::INACTIVE));
  CHECK_EQ(reset.LevelHistory().back(), hf_gpio_level_t::HF_GPIO_LEVEL_HIGH);

  CHECK(!port.GpioSet(CtrlPin::Wake, GpioSignal::ACTIVE));
  CHECK_EQ(port.LastGpioError(), hf_gpio_err_t::GPIO_ERR_PIN_NOT_FOUND);
  CHECK_EQ(port.LastGpioResult().status, Status::NotFound);

  reset.FailWrites();
  CHECK(!port.GpioSet(CtrlPin::Reset, GpioSignal::ACTIVE));
  CHECK(port.LastGpioError() != hf_gpio_err_t::GPIO_SUCCESS);
}

HF_TEST(HalI2cCommBase_StandardAndRegisterHooks) {
  constexpr uint8_t kAddr = 0x46;
  hf::host::MockI2c i2c(kAddr);
  i2c.EnableRegisterModel();
  I2cAdapter port(i2c);
  REQUIRE(port.EnsureInitialized());
  CHECK_EQ(port.Address(), kAddr);

  const uint8_t frame[3] = {0x10, 0x11, 0x22};
  REQUIRE(port.Write(kAddr, frame, 3));
  const uint8_t reg = 0x10;
  uint8_t out[2] = {};
  REQUIRE(port.WriteRead(kAddr, &reg, 1, out, 2));
  CHECK_EQ(out[0], 0x11);
  CHECK_EQ(out[1], 0x22);
  CHECK(!port.Write(0x47, frame, 3)); // another device: refused, nothing sent
  CHECK_EQ(port.LastBusError(), hf_i2c_err_t::I2C_ERR_INVALID_ADDRESS);

  const uint8_t data[3] = {0xA1, 0xA2, 0xA3};
  REQUIRE(port.WriteRegs(0x20, data, 3));
  uint8_t back[3] = {};
  REQUIRE(port.ReadRegs(0x20, back, 3));
  CHECK_EQ(back[2], 0xA3);
  const uint8_t big[5] = {};
  CHECK(!port.WriteRegs(0x00, big, 5)); // above this adapter's kMaxRegisterWrite
  CHECK_EQ(port.LastBusError(), hf_i2c_err_t::I2C_ERR_DATA_TOO_LONG);

  i2c.FailNext(hf_i2c_err_t::I2C_ERR_DEVICE_NACK);
  CHECK(!port.ReadRegs(0x10, out, 1));
  CHECK_EQ(port.LastBusResult().status, Status::Bus);
  CHECK_EQ(port.LastBusResult().detail, static_cast<uint16_t>(hf_i2c_err_t::I2C_ERR_DEVICE_NACK));
}

HF_TEST(HalUartCommBase_WriteFailureIsReported) {
  hf::host::MockUart uart;
  UartAdapter port(uart);
  REQUIRE(port.EnsureInitialized());
  const uint8_t msg[3] = {'A', '\r', '\n'};
  REQUIRE(port.Write(msg, 3));
  CHECK_EQ(uart.TxBytes().size(), 3U);

  uart.FailNextWrite(hf_uart_err_t::UART_ERR_TRANSMISSION_FAILED);
  CHECK(!port.Write(msg, 3));
  CHECK_EQ(port.LastBusResult().status, Status::Bus);

  uart.InjectRx("OK");
  uint8_t in[2] = {};
  REQUIRE(port.Read(in, 2, 10));
  CHECK_EQ(in[0], static_cast<uint8_t>('O'));
  CHECK(!port.Read(in, 2, 10)); // nothing left
  CHECK_EQ(port.LastBusError(), hf_uart_err_t::UART_ERR_TIMEOUT);
  CHECK(port.FlushInput());
}

HF_TEST(HalAdcCommBase_ReadsMillivoltsVoltsAndCounts) {
  hf::host::MockAdc adc(4, 3.3F);
  AdcAdapter port(adc);
  REQUIRE(port.EnsureInitialized());
  CHECK(port.IsInitialized());
  CHECK(port.IsChannelAvailable(3));
  adc.SetVoltage(2, 1.25F);
  uint32_t mv = 0;
  REQUIRE(port.ReadVoltageMv(2, mv));
  CHECK_EQ(mv, 1250U);
  float v = 0.0F;
  REQUIRE(port.ReadChannelV(2, v));
  CHECK_NEAR(v, 1.25F, 1e-4);

  adc.FailChannel(2);
  uint32_t count = 123;
  CHECK(!port.ReadChannelCount(2, count));
  CHECK_EQ(count, 0U);
  CHECK(!port.ReadVoltageMv(2, mv));
  CHECK_EQ(port.LastBusError(), hf_adc_err_t::ADC_ERR_CHANNEL_READ_ERR);
  CHECK_EQ(port.LastBusResult().status, Status::Bus);
}
