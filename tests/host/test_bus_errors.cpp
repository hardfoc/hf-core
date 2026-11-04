/**
 * @file test_bus_errors.cpp
 * @brief Bus errors reach the driver through hf-core's adapters, and the handlers report them:
 *        - Alicat BASIS-2 and FDO2: the UART adapter's write() returns false when BaseUart::Write
 *          fails, and the driver fails the call with SerialError (it used to wait for a reply and
 *          report Timeout);
 *        - ISF15ACP4 and MAX22200: the adapter's GpioSet() returns false when the GPIO write
 *          fails, so the driver's GpioControl / PowerSequence / GPIO_ERROR paths are reachable;
 *        - AT25040: a failed SPI transfer is Status::BusError, and At25040Handler::Probe() says
 *          ProbeResult::BusError instead of "absent".
 */

#include "AlicatBasis2Handler.h"
#include "At25040Handler.h"
#include "Fdo2Handler.h"
#include "HfCoreMocks.h"
#include "HostTest.h"
#include "Isf15acp4Handler.h"
#include "Max22200Handler.h"

#include <string>
#include <vector>

using hf::host::MockGpio;
using hf::host::MockSpi;
using hf::host::MockUart;

namespace {
constexpr auto kOut = hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT;
}  // namespace

// ── UART write hooks ─────────────────────────────────────────────────────────

HF_TEST(BusErrors_AlicatWriteReportsAFailedUartWrite) {
    MockUart uart;
    HalUartAlicatBasis2Comm comm(uart);
    const uint8_t frame[4] = {1, 3, 0, 0};
    CHECK(comm.write(frame, sizeof(frame)));
    uart.FailNextWrite(hf_uart_err_t::UART_ERR_TRANSMISSION_FAILED, 1);
    CHECK(!comm.write(frame, sizeof(frame)));
}

HF_TEST(BusErrors_AlicatDriverFailsWithSerialError) {
    MockUart uart;
    HalUartAlicatBasis2Comm comm(uart);
    alicat_basis2::Driver<HalUartAlicatBasis2Comm> drv(comm, 1);
    uart.FailNextWrite(hf_uart_err_t::UART_ERR_TRANSMISSION_FAILED, 1);
    uint16_t out = 0;
    CHECK(drv.ReadHoldingRegisters(2100, &out, 1) == alicat_basis2::DriverError::SerialError);
}

HF_TEST(BusErrors_Fdo2WriteReportsAFailedUartWrite) {
    MockUart uart;
    HalUartFdo2Comm comm(uart);
    const uint8_t cmd[] = {'#', 'V', 'E', 'R', 'S', '\r'};
    CHECK(comm.write(cmd, sizeof(cmd)));
    uart.FailNextWrite(hf_uart_err_t::UART_ERR_TRANSMISSION_FAILED, 1);
    CHECK(!comm.write(cmd, sizeof(cmd)));
}

HF_TEST(BusErrors_Fdo2HandlerFailsWithSerialError) {
    MockUart uart;
    uart.SetOnWrite([](MockUart& u, const std::vector<uint8_t>& written) {
        if (std::string(written.begin(), written.end()) == "#VERS\r") {
            u.InjectRx("#VERS 8 1 357 15\r");
        }
    });
    Fdo2Handler fdo2(uart);
    REQUIRE(fdo2.EnsureInitialized());
    uart.FailNextWrite(hf_uart_err_t::UART_ERR_TRANSMISSION_FAILED, 1);
    CHECK(fdo2.ReadVersion().error == fdo2::DriverError::SerialError);
}

// ── GPIO hooks ───────────────────────────────────────────────────────────────

HF_TEST(BusErrors_Isf15acp4BeginCommandReportsAFailedPin) {
    MockSpi spi;
    MockGpio cs(1, kOut), dc(2, kOut), reset(3, kOut);
    REQUIRE(cs.EnsureInitialized() && dc.EnsureInitialized() && reset.EnsureInitialized());
    HalIsf15acp4SpiAdapter adapter(spi, cs, dc, reset);
    REQUIRE(adapter.EnsureInitialized());
    CHECK(adapter.BeginCommand());
    adapter.EndTransaction();
    dc.FailWrites(true);  // D/C stuck: a command would be taken as display data
    CHECK(!adapter.BeginCommand());
    CHECK(!adapter.BeginData());
}

HF_TEST(BusErrors_Isf15acp4GpioSetReportsAFailedPin) {
    MockSpi spi;
    MockGpio cs(1, kOut), dc(2, kOut), reset(3, kOut);
    REQUIRE(cs.EnsureInitialized() && dc.EnsureInitialized() && reset.EnsureInitialized());
    HalIsf15acp4SpiAdapter adapter(spi, cs, dc, reset);
    REQUIRE(adapter.EnsureInitialized());
    CHECK(adapter.GpioSet(isf15acp4::CtrlPin::Reset, isf15acp4::GpioSignal::Active));
    reset.FailWrites(true);
    CHECK(!adapter.GpioSet(isf15acp4::CtrlPin::Reset, isf15acp4::GpioSignal::Active));
}

HF_TEST(BusErrors_Isf15acp4InitializeReportsPowerSequence) {
    MockSpi spi;
    MockGpio cs(1, kOut), dc(2, kOut), reset(3, kOut);
    REQUIRE(cs.EnsureInitialized() && dc.EnsureInitialized() && reset.EnsureInitialized());
    HalIsf15acp4SpiAdapter adapter(spi, cs, dc, reset);
    isf15acp4::SmartDisplay<HalIsf15acp4SpiAdapter> display(&adapter);
    REQUIRE(adapter.EnsureInitialized());  // the adapter came up; RES# then fails in the reset pulse
    reset.FailWrites(true);
    CHECK(!display.Initialize());
    CHECK(display.HasError(isf15acp4::Error::GpioControl));
    CHECK(display.HasError(isf15acp4::Error::PowerSequence));
}

HF_TEST(BusErrors_Max22200GpioSetReportsAFailedPin) {
    MockSpi spi;
    MockGpio enable(1, kOut), cmd(2, kOut);
    REQUIRE(enable.EnsureInitialized() && cmd.EnsureInitialized());
    HalSpiMax22200Comm comm(spi, enable, cmd);
    CHECK(!comm.GpioSet(max22200::CtrlPin::CMD, max22200::GpioSignal::ACTIVE));  // not initialised
    REQUIRE(comm.Initialize());
    CHECK(comm.GpioSet(max22200::CtrlPin::CMD, max22200::GpioSignal::ACTIVE));
    cmd.FailWrites(true);
    CHECK(!comm.GpioSet(max22200::CtrlPin::CMD, max22200::GpioSignal::ACTIVE));
}

HF_TEST(BusErrors_Max22200DriverReportsAnEnablePinItCannotRaise) {
    MockSpi spi;
    MockGpio enable(1, kOut), cmd(2, kOut);
    REQUIRE(enable.EnsureInitialized() && cmd.EnsureInitialized());
    HalSpiMax22200Comm comm(spi, enable, cmd);
    max22200::MAX22200<HalSpiMax22200Comm> drv(comm);
    enable.FailWrites(true);
    CHECK(drv.Initialize() == max22200::DriverStatus::INITIALIZATION_ERROR);
    CHECK(spi.Transactions().empty());  // nothing sent to a chip still in shutdown
}

// ── AT25040 ──────────────────────────────────────────────────────────────────

HF_TEST(BusErrors_At25040ProbeReportsABusErrorNotAnAbsentChip) {
    MockSpi spi;
    At25040Handler eeprom(spi);
    spi.FailNext(hf_spi_err_t::SPI_ERR_TRANSFER_FAILED, 10);
    CHECK(eeprom.Probe() == At25040Handler::ProbeResult::BusError);
    CHECK(eeprom.LastStatus() == at25040::Status::BusError);
}

HF_TEST(BusErrors_At25040ReadRecordsTheBusError) {
    MockSpi spi;
    At25040Handler eeprom(spi);
    spi.FailNext(hf_spi_err_t::SPI_ERR_TRANSFER_FAILED, 10);
    uint8_t buf[4] = {};
    CHECK(!eeprom.Read(0, buf, sizeof(buf)));
    CHECK(eeprom.LastStatus() == at25040::Status::BusError);
}

HF_TEST(BusErrors_Isf15acp4AdapterIsNotReadyWithAnUndrivenIdlePin) {
    MockSpi spi;
    MockGpio cs(1, kOut), dc(2, kOut), reset(3, kOut);
    REQUIRE(cs.EnsureInitialized() && dc.EnsureInitialized() && reset.EnsureInitialized());
    HalIsf15acp4SpiAdapter adapter(spi, cs, dc, reset);
    cs.FailWrites(true);
    CHECK(!adapter.EnsureInitialized());
}
