/**
 * @file test_tmc9660.cpp
 * @brief Tmc9660Handler and its SPI adapter on a TMCL slave model.
 *
 * TMCL over SPI is pipelined: the 8-byte MISO of a frame is the reply to the
 * previous frame. The driver therefore sends each command followed by a
 * NO_OP (opcode 0xFF) and parses the second MISO. The model keeps an axis
 * parameter store and answers SAP (5) / GAP (6).
 *
 * The handler's Initialize() runs the full bootloader sequence, which needs
 * a BootloaderConfig and a bootloader-mode device; that path is not modelled.
 * The parameter path is exercised through HalSpiTmc9660Comm, the hf-core
 * adapter that Initialize() hands to the driver.
 */

#include "HfCoreMocks.h"
#include "HostTest.h"
#include "Tmc9660Handler.h"

#include <map>
#include <utility>

using hf::host::MockGpio;
using hf::host::MockSpi;
using hf::host::SpiTransaction;
using Param = tmc9660::tmcl::Parameters;

namespace {

constexpr uint8_t kSap = 5;
constexpr uint8_t kGap = 6;
constexpr uint8_t kNoOp = 0xFF;
constexpr uint8_t kSpiOk = 0xFF;
constexpr uint8_t kSpiNotReady = 0xF0;
constexpr uint8_t kReplyOk = 100;
constexpr uint8_t kReplyWrongType = 3;

uint8_t Checksum(const uint8_t* b, size_t n) {
    uint8_t s = 0;
    for (size_t i = 0; i < n; ++i) {
        s = static_cast<uint8_t>(s + b[i]);
    }
    return s;
}

class TmclSlaveModel {
public:
    explicit TmclSlaveModel(MockSpi& spi) {
        spi.SetResponder([this](const uint8_t* tx, uint8_t* rx, uint16_t len) { OnFrame(tx, rx, len); });
    }

    std::map<std::pair<uint16_t, uint8_t>, uint32_t> params;  ///< (type, motor) -> value
    std::map<uint16_t, uint8_t> reject_type;                   ///< type -> TMCL status to answer
    int not_ready_replies{0};  ///< answer this many reply frames with NOT_READY first
    int commands{0};

private:
    void OnFrame(const uint8_t* tx, uint8_t* rx, uint16_t len) {
        if (len != 8) {
            return;
        }
        if (not_ready_replies > 0 && tx[0] == kNoOp) {
            --not_ready_replies;
            Fill(rx, kSpiNotReady, 0, 0, 0);
            return;
        }
        std::copy(pending_, pending_ + 8, rx);
        if (tx[0] == kNoOp) {
            return;  // a reply request: the pending reply stays valid for a retry
        }
        ++commands;
        const uint8_t op = tx[0];
        const auto type = static_cast<uint16_t>(tx[1] | ((tx[2] & 0xF0u) << 4));
        const uint8_t motor = tx[2] & 0x0Fu;
        const uint32_t value = (static_cast<uint32_t>(tx[3]) << 24) | (static_cast<uint32_t>(tx[4]) << 16) |
                               (static_cast<uint32_t>(tx[5]) << 8) | tx[6];
        if (Checksum(tx, 7) != tx[7]) {
            Fill(pending_, kSpiOk, 1 /*REPLY_CHKERR*/, op, 0);
            return;
        }
        if (auto it = reject_type.find(type); it != reject_type.end()) {
            Fill(pending_, kSpiOk, it->second, op, 0);
            return;
        }
        if (op == kSap) {
            params[{type, motor}] = value;
            Fill(pending_, kSpiOk, kReplyOk, op, value);
        } else if (op == kGap) {
            Fill(pending_, kSpiOk, kReplyOk, op, params[{type, motor}]);
        } else {
            Fill(pending_, kSpiOk, 2 /*REPLY_INVALID_CMD*/, op, 0);
        }
    }

    static void Fill(uint8_t* out, uint8_t spi_status, uint8_t status, uint8_t op, uint32_t value) {
        out[0] = spi_status;
        out[1] = status;
        out[2] = op;
        out[3] = static_cast<uint8_t>(value >> 24);
        out[4] = static_cast<uint8_t>(value >> 16);
        out[5] = static_cast<uint8_t>(value >> 8);
        out[6] = static_cast<uint8_t>(value);
        out[7] = Checksum(out, 7);
    }

    uint8_t pending_[8] = {kSpiOk, kReplyOk, 0, 0, 0, 0, 0, 0};
};

struct Pins {
    MockGpio rst{1, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT};
    MockGpio drv_en{2, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT};
    MockGpio faultn{3, hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT, hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW};
    MockGpio wake{4, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT, hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW};
};

size_t CommandFrames(const MockSpi& spi, uint8_t op) {
    size_t n = 0;
    for (const SpiTransaction& t : spi.Transactions()) {
        n += (t.tx.size() == 8 && t.tx[0] == op) ? 1 : 0;
    }
    return n;
}

}  // namespace

HF_TEST(Tmc9660_ConstructionIsLazyAndReportsSpi) {
    MockSpi spi;
    Pins pins;
    Tmc9660Handler h(spi, pins.rst, pins.drv_en, pins.faultn, pins.wake, 0x00);
    CHECK(h.GetCommMode() == tmc9660::CommMode::SPI);
    CHECK(!h.IsDriverReady());
    CHECK(spi.Transactions().empty());
    CHECK(pins.rst.LevelHistory().empty());
}

HF_TEST(Tmc9660_InitializeWithoutBootloaderConfigTouchesNothing) {
    MockSpi spi;
    Pins pins;
    Tmc9660Handler h(spi, pins.rst, pins.drv_en, pins.faultn, pins.wake, 0x00, /*bootCfg=*/nullptr);
    CHECK(!h.Initialize());
    CHECK(!h.IsDriverReady());
    CHECK(h.driverViaSpi() == nullptr);  // retries Initialize, fails again
    CHECK(spi.Transactions().empty());
    CHECK(pins.rst.LevelHistory().empty());
}

HF_TEST(Tmc9660_WriteThenReadAxisParameter) {
    MockSpi spi;
    TmclSlaveModel chip(spi);
    Pins pins;
    HalSpiTmc9660Comm comm(spi, pins.rst, pins.drv_en, pins.faultn, pins.wake);
    Tmc9660Handler::SpiDriver drv(comm, 0x00);

    CHECK(drv.writeParameter(Param::MOTOR_POLE_PAIRS, 7));
    CHECK_EQ((chip.params[{static_cast<uint16_t>(Param::MOTOR_POLE_PAIRS), 0}]), 7u);

    uint32_t value = 0;
    CHECK(drv.readParameter(Param::MOTOR_POLE_PAIRS, value));
    CHECK_EQ(value, 7u);

    // Each command is one 8-byte command frame plus one NO_OP reply frame.
    CHECK_EQ(CommandFrames(spi, kSap), 1u);
    CHECK_EQ(CommandFrames(spi, kGap), 1u);
    CHECK_EQ(CommandFrames(spi, kNoOp), 2u);
    for (const SpiTransaction& t : spi.Transactions()) {
        CHECK_EQ(t.tx.size(), 8u);
        CHECK_EQ(t.tx[7], Checksum(t.tx.data(), 7));
    }
}

HF_TEST(Tmc9660_ParameterNumbersAbove255KeepTheirHighBits) {
    // TYPE is 12 bits: type[11:8] travels in the high nibble of byte 2.
    MockSpi spi;
    TmclSlaveModel chip(spi);
    Pins pins;
    HalSpiTmc9660Comm comm(spi, pins.rst, pins.drv_en, pins.faultn, pins.wake);
    Tmc9660Handler::SpiDriver drv(comm, 0x00);
    const auto big = static_cast<Param>(0x123);
    CHECK(drv.writeParameter(big, 0xCAFEBABE, 1));
    CHECK_EQ((chip.params[{0x123, 1}]), 0xCAFEBABEu);
    REQUIRE(!spi.Transactions().empty());
    const auto& cmd = spi.Transactions()[0].tx;
    CHECK_EQ(cmd[1], 0x23u);
    CHECK_EQ(cmd[2], 0x11u);  // type[11:8]=1 in the high nibble, motor 1 in the low nibble
}

HF_TEST(Tmc9660_RejectedParameterFails) {
    MockSpi spi;
    TmclSlaveModel chip(spi);
    chip.reject_type[static_cast<uint16_t>(Param::MAX_TORQUE)] = kReplyWrongType;
    Pins pins;
    HalSpiTmc9660Comm comm(spi, pins.rst, pins.drv_en, pins.faultn, pins.wake);
    Tmc9660Handler::SpiDriver drv(comm, 0x00);
    uint32_t value = 0;
    CHECK(!drv.readParameter(Param::MAX_TORQUE, value));
    CHECK(!drv.writeParameter(Param::MAX_TORQUE, 1000));
}

HF_TEST(Tmc9660_NotReadyIsRetriedWithoutResendingTheCommand) {
    MockSpi spi;
    TmclSlaveModel chip(spi);
    chip.not_ready_replies = 3;
    Pins pins;
    HalSpiTmc9660Comm comm(spi, pins.rst, pins.drv_en, pins.faultn, pins.wake);
    Tmc9660Handler::SpiDriver drv(comm, 0x00);
    CHECK(drv.writeParameter(Param::TARGET_VELOCITY, 1500));
    CHECK_EQ(chip.commands, 1);               // the SAP itself went out once
    CHECK_EQ(CommandFrames(spi, kSap), 1u);
    CHECK_EQ(CommandFrames(spi, kNoOp), 4u);  // three NOT_READY, then the reply
}

HF_TEST(Tmc9660_SpiTransportErrorFailsTheCommand) {
    MockSpi spi;
    TmclSlaveModel chip(spi);
    Pins pins;
    HalSpiTmc9660Comm comm(spi, pins.rst, pins.drv_en, pins.faultn, pins.wake);
    Tmc9660Handler::SpiDriver drv(comm, 0x00);
    spi.FailNext(hf_spi_err_t::SPI_ERR_TRANSFER_TIMEOUT);
    CHECK(!drv.writeParameter(Param::MOTOR_TYPE, 3));
    CHECK_EQ(comm.transportDiag(), static_cast<uint32_t>(hf_spi_err_t::SPI_ERR_TRANSFER_TIMEOUT));
    CHECK(drv.writeParameter(Param::MOTOR_TYPE, 3));  // bus recovered
}
