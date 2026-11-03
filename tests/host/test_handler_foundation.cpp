/**
 * @file test_handler_foundation.cpp
 * @brief hf::Status, hf::Result, the interface-wrap mappings, HandlerBase and the Hal{Bus}Comm bases.
 */

#include <cstdint>
#include <cstring>
#include <type_traits>

#include "HalAdcComm.h"
#include "HalGpioComm.h"
#include "HalI2cComm.h"
#include "HalSpiComm.h"
#include "HalUartComm.h"
#include "HandlerBase.h"
#include "HfCoreMocks.h"
#include "HfResult.h"
#include "HfStatus.h"
#include "HostTest.h"
#include "IiwStatus.h"

using hf::Fail;
using hf::Result;
using hf::Status;
using hf::host::FakeClock;

// ── Compile-time contract ───────────────────────────────────────────────────

namespace {

struct NonTrivial {
    NonTrivial() noexcept = default;
    NonTrivial(const NonTrivial& other) noexcept : v(other.v) {}
    NonTrivial& operator=(const NonTrivial&) noexcept = default;
    int v{0};
};

struct Sample {
    float pressure_pa{0.0F};
    float temperature_c{0.0F};
};

} // namespace

static_assert(std::is_trivially_copyable_v<Result<int>>);
static_assert(std::is_trivially_copyable_v<Result<Sample>>);
static_assert(std::is_trivially_copyable_v<Result<void>>);
static_assert(!std::is_trivially_copyable_v<Result<NonTrivial>>);
static_assert(sizeof(Result<void>) == 8);
static_assert(hf::kIsResult<Result<int>> && hf::kIsResult<Result<void>> && !hf::kIsResult<int>);
static_assert(Result<int>(7).ok() && Result<int>(7).value() == 7);
static_assert(!Result<int>(Fail(Status::Bus, 3)).ok());
static_assert(Result<int>(Fail(Status::Ok)).status() == Status::Unknown, "Fail(Ok) is still a failure");
static_assert(hf::ToStatus(hf_spi_err_t::SPI_SUCCESS) == Status::Ok);
static_assert(hf::ToStatus(hf_i2c_err_t::I2C_ERR_DEVICE_NACK) == Status::Bus);
static_assert(hf::ToStatus(hf_uart_err_t::UART_ERR_TIMEOUT) == Status::Timeout);
static_assert(hf::kIsIiwError<hf_temp_err_t> && !hf::kIsIiwError<int>);

// ── Status ───────────────────────────────────────────────────────────────────

HF_TEST(Status_EveryValueHasItsOwnName) {
    const char* names[hf::kStatusCount] = {};
    for (std::uint8_t i = 0; i < hf::kStatusCount; ++i) {
        names[i] = hf::ToString(static_cast<Status>(i));
        REQUIRE(names[i] != nullptr);
        CHECK(std::strcmp(names[i], "Unknown") != 0 || i == hf::kStatusCount - 1);
        for (std::uint8_t j = 0; j < i; ++j) {
            CHECK(std::strcmp(names[i], names[j]) != 0);
        }
    }
    CHECK(std::strcmp(hf::ToString(Status::DependencyNotReady), "DependencyNotReady") == 0);
    CHECK(std::strcmp(hf::ToString(static_cast<Status>(200)), "Unknown") == 0);
    CHECK(hf::IsOk(Status::Ok) && !hf::IsOk(Status::Fault));
}

// ── Result ───────────────────────────────────────────────────────────────────

namespace {

Result<Sample> ReadSample(bool bus_ok) {
    if (!bus_ok) {
        return Fail(Status::Bus, 42U);
    }
    return Sample{101325.0F, 21.5F};
}

Result<void> Forward(bool bus_ok) {
    const auto s = ReadSample(bus_ok);
    if (!s.ok()) {
        return s.error();
    }
    return hf::Ok();
}

} // namespace

HF_TEST(Result_CarriesValueOrStatusAndDetail) {
    const auto ok = ReadSample(true);
    REQUIRE(ok.ok());
    CHECK(static_cast<bool>(ok));
    CHECK_EQ(ok.status(), Status::Ok);
    CHECK_EQ(ok.detail(), 0U);
    CHECK_NEAR(ok->pressure_pa, 101325.0F, 0.01);
    CHECK_NEAR((*ok).temperature_c, 21.5F, 0.001);

    const auto bad = ReadSample(false);
    CHECK(!bad.ok());
    CHECK_EQ(bad.status(), Status::Bus);
    CHECK_EQ(bad.detail(), 42U);
    CHECK_NEAR(bad.value().pressure_pa, 0.0F, 0.0); // T{} on failure, never garbage
    CHECK_NEAR(Result<float>(Fail(Status::Timeout)).value_or(-1.0F), -1.0F, 0.0);
    CHECK_NEAR(Result<float>(2.0F).value_or(-1.0F), 2.0F, 0.0);
}

HF_TEST(Result_VoidForwardsAFailureUnchanged) {
    CHECK(Forward(true).ok());
    const auto r = Forward(false);
    CHECK_EQ(r.status(), Status::Bus);
    CHECK_EQ(r.detail(), 42U);
    CHECK(Result<void>{}.ok());
}

// ── Interface-wrap mappings ─────────────────────────────────────────────────

// Every code of every table: success and only success is Ok, and the value is a real Status.
#define HF_CHECK_TABLE(LIST, ENUM, SUCCESS)                                                                            \
    do {                                                                                                               \
        LIST(HF_CHECK_ONE)                                                                                             \
    } while (0)
#define HF_CHECK_ONE(NAME, VALUE, DESC)                                                                                \
    {                                                                                                                  \
        const Status s = hf::ToStatus(static_cast<kEnum>(VALUE));                                                      \
        CHECK(static_cast<std::uint8_t>(s) < hf::kStatusCount);                                                        \
        CHECK_EQ(s == Status::Ok, static_cast<kEnum>(VALUE) == kSuccess);                                              \
        const Result<void> r = hf::ToResult(static_cast<kEnum>(VALUE));                                                \
        CHECK_EQ(r.status(), s);                                                                                       \
        CHECK_EQ(r.detail(), r.ok() ? 0U : static_cast<std::uint32_t>(VALUE));                                         \
    }

HF_TEST(IiwStatus_EveryCodeMapsAndOnlySuccessIsOk) {
    {
        using kEnum = hf_spi_err_t;
        constexpr kEnum kSuccess = hf_spi_err_t::SPI_SUCCESS;
        HF_CHECK_TABLE(HF_SPI_ERR_LIST, kEnum, kSuccess);
    }
    {
        using kEnum = hf_i2c_err_t;
        constexpr kEnum kSuccess = hf_i2c_err_t::I2C_SUCCESS;
        HF_CHECK_TABLE(HF_I2C_ERR_LIST, kEnum, kSuccess);
    }
    {
        using kEnum = hf_uart_err_t;
        constexpr kEnum kSuccess = hf_uart_err_t::UART_SUCCESS;
        HF_CHECK_TABLE(HF_UART_ERR_LIST, kEnum, kSuccess);
    }
    {
        using kEnum = hf_adc_err_t;
        constexpr kEnum kSuccess = hf_adc_err_t::ADC_SUCCESS;
        HF_CHECK_TABLE(HF_ADC_ERR_LIST, kEnum, kSuccess);
    }
    {
        using kEnum = hf_gpio_err_t;
        constexpr kEnum kSuccess = hf_gpio_err_t::GPIO_SUCCESS;
        HF_CHECK_TABLE(HF_GPIO_ERR_LIST, kEnum, kSuccess);
    }
    {
        using kEnum = hf_can_err_t;
        constexpr kEnum kSuccess = hf_can_err_t::CAN_SUCCESS;
        HF_CHECK_TABLE(HF_CAN_ERR_LIST, kEnum, kSuccess);
    }
    {
        using kEnum = hf_nvs_err_t;
        constexpr kEnum kSuccess = hf_nvs_err_t::NVS_SUCCESS;
        HF_CHECK_TABLE(HF_NVS_ERR_LIST, kEnum, kSuccess);
    }
    {
        using kEnum = hf_temp_err_t;
        constexpr kEnum kSuccess = TEMP_SUCCESS;
        HF_CHECK_TABLE(HF_TEMP_ERR_LIST, kEnum, kSuccess);
    }
}

#undef HF_CHECK_ONE
#undef HF_CHECK_TABLE

HF_TEST(IiwStatus_RepresentativeCodes) {
    CHECK_EQ(hf::ToStatus(hf_spi_err_t::SPI_ERR_BUS_BUSY), Status::Busy);
    CHECK_EQ(hf::ToStatus(hf_spi_err_t::SPI_ERR_NOT_INITIALIZED), Status::NotInitialized);
    CHECK_EQ(hf::ToStatus(hf_i2c_err_t::I2C_ERR_DEVICE_NOT_FOUND), Status::NotFound);
    CHECK_EQ(hf::ToStatus(hf_i2c_err_t::I2C_ERR_INVALID_ADDRESS), Status::InvalidArgument);
    CHECK_EQ(hf::ToStatus(hf_i2c_err_t::I2C_ERR_OUT_OF_MEMORY), Status::NoMemory);
    CHECK_EQ(hf::ToStatus(hf_uart_err_t::UART_ERR_PARITY_ERROR), Status::Bus);
    CHECK_EQ(hf::ToStatus(hf_adc_err_t::ADC_ERR_UNSUPPORTED_OPERATION), Status::Unsupported);
    CHECK_EQ(hf::ToStatus(hf_gpio_err_t::GPIO_ERR_HARDWARE_FAULT), Status::Fault);
    CHECK_EQ(hf::ToStatus(hf_can_err_t::CAN_ERR_BUS_NOT_AVAILABLE), Status::DependencyNotReady);
    CHECK_EQ(hf::ToStatus(hf_spi_err_t::SPI_ERR_UNKNOWN), Status::Unknown);
}

// ── HandlerBase ──────────────────────────────────────────────────────────────

namespace {

struct TestClock {
    static std::uint64_t NowUs() noexcept {
        return FakeClock::NowUs();
    }
};

struct FakeDriver {
    explicit FakeDriver(int& constructed) noexcept {
        ++constructed;
    }
    int reads{0};
};

class FakeHandler : public hf::HandlerBase<FakeHandler, FakeDriver, TestClock> {
public:
    explicit FakeHandler(hf::HandlerMutex* bus_mutex = nullptr) noexcept : HandlerBase(bus_mutex) {}
    ~FakeHandler() noexcept {
        DestroyDriver();
    }

    Result<int> Read() noexcept {
        return VisitDriver([this](FakeDriver& d) -> Result<int> {
            if (fail_reads) {
                return Fail(Status::Bus, 7U);
            }
            return ++d.reads;
        });
    }

    Status init_status{Status::Ok};
    bool fail_reads{false};
    int constructed{0};
    int init_calls{0};
    int deinit_calls{0};

private:
    friend HandlerBase;

    Result<void> InitializeLocked() noexcept {
        ++init_calls;
        EmplaceDriver(constructed);
        if (init_status != Status::Ok) {
            return Fail(init_status, 99U);
        }
        return {};
    }

    Result<void> DeinitializeLocked(FakeDriver& /*driver*/) noexcept {
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
    Result<void> InitializeLocked() noexcept {
        EmplaceDriver(constructed_);
        return {};
    }
    int constructed_{0};
};

class ForgetfulHandler : public hf::HandlerBase<ForgetfulHandler, FakeDriver> {
private:
    friend HandlerBase;
    Result<void> InitializeLocked() noexcept {
        return {};
    } // forgets EmplaceDriver()
};

} // namespace

HF_TEST(HandlerBase_ConstructionDoesNothingUntilFirstUse) {
    FakeHandler h;
    CHECK(!h.IsInitialized());
    CHECK_EQ(h.constructed, 0);
    CHECK_EQ(h.init_calls, 0);
    const auto d = h.GetDiagnostics();
    CHECK(!d.initialized);
    CHECK_EQ(d.last_status, Status::NotInitialized);
}

HF_TEST(HandlerBase_FailedInitCachesNothingAndRetries) {
    FakeHandler h;
    h.init_status = Status::NotFound;
    const auto r = h.EnsureInitialized();
    CHECK_EQ(r.status(), Status::NotFound);
    CHECK_EQ(r.detail(), 99U);
    CHECK(!h.IsInitialized());
    CHECK_EQ(h.Read().status(), Status::NotFound); // VisitDriver retries and reports the same failure
    CHECK_EQ(h.init_calls, 2);

    h.init_status = Status::Ok;
    CHECK(h.EnsureInitialized().ok());
    CHECK(h.IsInitialized());
    CHECK(h.EnsureInitialized().ok());
    CHECK_EQ(h.init_calls, 3); // the successful sequence runs once
    CHECK_EQ(h.constructed, 3);

    const auto d = h.GetDiagnostics();
    CHECK(d.initialized);
    CHECK_EQ(d.init_attempts, 3U);
    CHECK_EQ(d.init_failures, 2U);
}

HF_TEST(HandlerBase_VisitDriverReturnsAndTracksResults) {
    FakeClock::Reset();
    FakeHandler h;
    FakeClock::AdvanceUs(5000);
    const auto first = h.Read();
    REQUIRE(first.ok());
    CHECK_EQ(first.value(), 1);

    const Result<int> plain = h.VisitDriver([](FakeDriver& d) { return d.reads * 10; });
    CHECK_EQ(plain.value(), 10);
    const Result<void> nothing = h.VisitDriver([](FakeDriver& d) { d.reads = 0; });
    CHECK(nothing.ok());

    h.fail_reads = true;
    FakeClock::AdvanceUs(1000);
    const auto bad = h.Read();
    CHECK_EQ(bad.status(), Status::Bus);
    CHECK_EQ(bad.detail(), 7U);

    const auto d = h.GetDiagnostics();
    CHECK_EQ(d.last_status, Status::Bus);
    CHECK_EQ(d.last_detail, 7U);
    CHECK_EQ(d.ok_count, 2U); // the init and the first Read(); plain values are not tracked
    CHECK_EQ(d.error_count, 1U);
    CHECK_EQ(d.bus_errors, 1U);
    CHECK_EQ(d.consecutive_errors, 1U);
    CHECK(d.last_ok_us >= 5000U);
    CHECK(d.last_error_us > d.last_ok_us);
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
}

HF_TEST(HandlerBase_InitThatBuildsNoDriverIsAFault) {
    ForgetfulHandler f;
    CHECK_EQ(f.EnsureInitialized().status(), Status::Fault);
    CHECK(!f.IsInitialized());
}

// ── Hal{Bus}Comm ─────────────────────────────────────────────────────────────

namespace {

// A driver port in the standard shape: CRTP forwarding stubs the adapter base must hide.
template <typename Derived>
struct FakeSpiPort {  // NOLINT(bugprone-crtp-constructor-accessibility): built by the adapter chain
    bool Transfer(const std::uint8_t* tx, std::uint8_t* rx, std::size_t len) noexcept {
        return static_cast<Derived*>(this)->Transfer(tx, rx, len);
    }
    void DelayUs(std::uint32_t us) noexcept {
        static_cast<Derived*>(this)->DelayUs(us);
    }
};

enum class CtrlPin : std::uint8_t { RESET, WAKE };
enum class GpioSignal : std::uint8_t { INACTIVE, ACTIVE };

class SpiAdapter final : public hf::HalSpiComm<SpiAdapter, hf::HalGpioComm<SpiAdapter, FakeSpiPort<SpiAdapter>>> {
public:
    static constexpr std::uint32_t kBusTimeoutMs = 5;
    SpiAdapter(BaseSpi& spi, BaseGpio& reset) noexcept : HalSpiComm(spi), reset_(reset) {}
    BaseGpio* GpioFor(CtrlPin pin) noexcept {
        return pin == CtrlPin::RESET ? &reset_ : nullptr;
    }

private:
    BaseGpio& reset_;
};

class I2cAdapter final : public hf::HalI2cComm<I2cAdapter> {
public:
    static constexpr std::size_t kMaxRegisterWrite = 4;
    using HalI2cComm::HalI2cComm;
};

class UartAdapter final : public hf::HalUartComm<UartAdapter> {
public:
    using HalUartComm::HalUartComm;
};

class AdcAdapter final : public hf::HalAdcComm<AdcAdapter> {
public:
    using HalAdcComm::HalAdcComm;
};

// Calls the hooks the way a driver does: through the port base.
template <typename Port>
bool DriverTransfer(Port& port, const std::uint8_t* tx, std::uint8_t* rx, std::size_t len) {
    return static_cast<FakeSpiPort<SpiAdapter>&>(port).Transfer(tx, rx, len);
}

} // namespace

HF_TEST(HalSpiComm_TransfersThroughTheDriverPortAndKeepsTheBusCode) {
    hf::host::MockSpi spi;
    hf::host::MockGpio reset(1, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT);
    SpiAdapter port(spi, reset);
    REQUIRE(port.EnsureInitialized());

    spi.QueueResponse({0xAB, 0xCD});
    const std::uint8_t tx[2] = {0x01, 0x02};
    std::uint8_t rx[2] = {};
    REQUIRE(DriverTransfer(port, tx, rx, 2));
    CHECK_EQ(rx[0], 0xAB);
    CHECK_EQ(rx[1], 0xCD);
    CHECK_EQ(port.LastBusError(), hf_spi_err_t::SPI_SUCCESS);

    spi.FailNext(hf_spi_err_t::SPI_ERR_TRANSFER_TIMEOUT);
    CHECK(!port.Write(tx, 2));
    CHECK_EQ(port.LastBusError(), hf_spi_err_t::SPI_ERR_TRANSFER_TIMEOUT);
    CHECK_EQ(port.LastBusResult().status(), Status::Timeout);
    CHECK(!port.Read(rx, 0x10000));
    CHECK_EQ(port.LastBusError(), hf_spi_err_t::SPI_ERR_TRANSFER_TOO_LONG);
    port.ClearLastBusError();
    CHECK(port.LastBusResult().ok());
}

HF_TEST(HalSpiComm_BusThatWillNotStartFailsEnsureInitialized) {
    hf::host::MockSpi spi;
    spi.SetInitializeResult(false);
    hf::host::MockGpio reset;
    SpiAdapter port(spi, reset);
    CHECK(!port.EnsureInitialized());
    CHECK_EQ(port.LastBusResult().status(), Status::NotInitialized);
}

HF_TEST(HalGpioComm_DrivesLogicalPinsAndRefusesUnwiredOnes) {
    hf::host::MockSpi spi;
    hf::host::MockGpio reset(1, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT,
                             hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
    SpiAdapter port(spi, reset);
    REQUIRE(port.GpioSet(CtrlPin::RESET, GpioSignal::ACTIVE));
    CHECK_EQ(reset.LevelHistory().back(), hf_gpio_level_t::HF_GPIO_LEVEL_LOW); // active-low: ACTIVE is LOW
    GpioSignal sig = GpioSignal::INACTIVE;
    REQUIRE(port.GpioRead(CtrlPin::RESET, sig));
    CHECK(sig == GpioSignal::ACTIVE);
    REQUIRE(port.GpioSet(CtrlPin::RESET, GpioSignal::INACTIVE));
    CHECK_EQ(reset.LevelHistory().back(), hf_gpio_level_t::HF_GPIO_LEVEL_HIGH);

    CHECK(!port.GpioSet(CtrlPin::WAKE, GpioSignal::ACTIVE));
    CHECK_EQ(port.LastGpioError(), hf_gpio_err_t::GPIO_ERR_PIN_NOT_FOUND);
    CHECK_EQ(port.LastGpioResult().status(), Status::NotFound);

    reset.FailWrites();
    CHECK(!port.GpioSet(CtrlPin::RESET, GpioSignal::ACTIVE));
    CHECK(port.LastGpioError() != hf_gpio_err_t::GPIO_SUCCESS);
}

HF_TEST(HalCommBase_DelaysRoundUpToWholeMilliseconds) {
    hf::host::MockSpi spi;
    hf::host::MockGpio reset;
    SpiAdapter port(spi, reset);
    FakeClock::Reset();
    static_cast<FakeSpiPort<SpiAdapter>&>(port).DelayUs(1500);
    CHECK_EQ(FakeClock::TotalDelayUs(), 2000U);
    port.DelayUs(0);
    CHECK_EQ(FakeClock::TotalDelayUs(), 2000U);
    port.DelayMs(70000); // more than os_delay_msec() takes at once
    CHECK_EQ(FakeClock::TotalDelayUs(), 2000U + 70000000U);
}

HF_TEST(HalI2cComm_RegisterHooksAndLimits) {
    hf::host::MockI2c i2c(0x46);
    i2c.EnableRegisterModel();
    I2cAdapter port(i2c);
    REQUIRE(port.EnsureInitialized());

    const std::uint8_t data[3] = {0x11, 0x22, 0x33};
    REQUIRE(port.WriteRegs(0x10, data, 3));
    CHECK_EQ(i2c.GetRegister(0x10), 0x11);
    CHECK_EQ(i2c.GetRegister(0x12), 0x33);
    std::uint8_t out[3] = {};
    REQUIRE(port.ReadRegs(0x10, out, 3));
    CHECK_EQ(out[1], 0x22);

    const std::uint8_t big[5] = {};
    CHECK(!port.WriteRegs(0x00, big, 5)); // above this adapter's kMaxRegisterWrite
    CHECK_EQ(port.LastBusError(), hf_i2c_err_t::I2C_ERR_DATA_TOO_LONG);
    CHECK(!port.ReadRegs(0x00, nullptr, 1));
    CHECK_EQ(port.LastBusError(), hf_i2c_err_t::I2C_ERR_INVALID_PARAMETER);

    i2c.FailNext(hf_i2c_err_t::I2C_ERR_DEVICE_NACK);
    CHECK(!port.ReadRegs(0x10, out, 1));
    CHECK_EQ(port.LastBusResult().status(), Status::Bus);
    CHECK_EQ(port.LastBusResult().detail(), static_cast<std::uint32_t>(hf_i2c_err_t::I2C_ERR_DEVICE_NACK));
}

HF_TEST(HalUartComm_WriteFailureIsReported) {
    hf::host::MockUart uart;
    UartAdapter port(uart);
    REQUIRE(port.EnsureInitialized());
    const std::uint8_t msg[3] = {'A', '\r', '\n'};
    REQUIRE(port.Write(msg, 3));
    CHECK_EQ(uart.TxBytes().size(), 3U);

    uart.FailNextWrite(hf_uart_err_t::UART_ERR_TRANSMISSION_FAILED);
    CHECK(!port.Write(msg, 3));
    CHECK_EQ(port.LastBusResult().status(), Status::Bus);

    uart.InjectRx("OK");
    std::uint8_t in[2] = {};
    REQUIRE(port.Read(in, 2, 10));
    CHECK_EQ(in[0], static_cast<std::uint8_t>('O'));
    CHECK(!port.Read(in, 2, 10)); // nothing left
    CHECK_EQ(port.LastBusError(), hf_uart_err_t::UART_ERR_TIMEOUT);
    CHECK(port.FlushRx());
}

HF_TEST(HalAdcComm_ReadsVoltsAndCounts) {
    hf::host::MockAdc adc(4, 3.3F);
    AdcAdapter port(adc);
    REQUIRE(port.EnsureInitialized());
    CHECK(port.IsInitialized());
    CHECK(port.IsChannelAvailable(3));
    adc.SetVoltage(2, 1.25F);
    float v = 0.0F;
    REQUIRE(port.ReadChannelV(2, v));
    CHECK_NEAR(v, 1.25F, 1e-4);

    adc.FailChannel(2);
    std::uint32_t count = 123;
    CHECK(!port.ReadChannelCount(2, count));
    CHECK_EQ(count, 0U);
    CHECK_EQ(port.LastBusError(), hf_adc_err_t::ADC_ERR_CHANNEL_READ_ERR);
    CHECK_EQ(port.LastBusResult().status(), Status::Bus);
}
