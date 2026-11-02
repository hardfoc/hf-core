/**
 * @file test_as5047u.cpp
 * @brief As5047uHandler over MockSpi with an AS5047U register model.
 *
 * The model follows the datasheet framing the driver implements: MISO of a
 * frame carries the answer to the previous command; 16-bit frames are reads
 * without CRC; 24-bit frames carry a CRC-8 (poly 0x1D, init 0xC4, xor 0xFF);
 * a write is a command frame followed by a data frame. ERRFL clears on read;
 * "persistent" error bits model a condition (such as a weak magnet) that
 * re-asserts the flag on every read.
 */

#include "As5047uHandler.h"
#include "HfCoreMocks.h"
#include "HostTest.h"

#include <map>
#include <optional>

using hf::host::MockSpi;

namespace {

constexpr uint16_t kErrfl = 0x0001;
constexpr uint16_t kZposm = 0x0016;
constexpr uint16_t kZposl = 0x0017;
constexpr uint16_t kAnglecom = 0x3FFF;

constexpr uint16_t kAgcWarning = 1u << 0;
constexpr uint16_t kMagHalf = 1u << 1;
constexpr uint16_t kCrcError = 1u << 6;
constexpr uint16_t kCordicOverflow = 1u << 10;

uint8_t Crc8(uint16_t data16) {
    uint8_t crc = 0xC4;
    for (int i = 0; i < 16; ++i) {
        const bool bit = (((data16 >> (15 - i)) & 1u) ^ ((crc >> 7) & 1u)) != 0;
        crc = static_cast<uint8_t>((crc << 1) ^ (bit ? 0x1D : 0x00));
    }
    return static_cast<uint8_t>(crc ^ 0xFF);
}

class As5047uModel {
public:
    explicit As5047uModel(MockSpi& spi) {
        spi.SetResponder([this](const uint8_t* tx, uint8_t* rx, uint16_t len) { OnFrame(tx, rx, len); });
    }

    std::map<uint16_t, uint16_t> regs;
    uint16_t errfl_latched{0};     ///< cleared by the first ERRFL read
    uint16_t errfl_persistent{0};  ///< re-asserted on every ERRFL read
    bool corrupt_miso_crc{false};  ///< answer 24-bit frames with a bad CRC
    int frames{0};

private:
    uint16_t ReadValue(uint16_t addr) {
        if (addr == kErrfl) {
            const uint16_t v = static_cast<uint16_t>(errfl_latched | errfl_persistent);
            errfl_latched = 0;
            return v;
        }
        return static_cast<uint16_t>(regs[addr] & 0x3FFF);
    }

    void OnFrame(const uint8_t* tx, uint8_t* rx, uint16_t len) {
        ++frames;
        const uint16_t word = static_cast<uint16_t>((tx[0] << 8) | tx[1]);
        if (len == 2) {
            rx[0] = static_cast<uint8_t>((pending_ >> 8) & 0x3F);
            rx[1] = static_cast<uint8_t>(pending_ & 0xFF);
            if ((word & 0x4000u) != 0) {
                pending_ = ReadValue(word & 0x3FFF);
            }
            return;
        }
        if (len != 3) {
            errfl_latched |= 1u << 4;  // framing error
            return;
        }
        if (tx[2] != Crc8(word)) {
            errfl_latched |= kCrcError;
        }
        if (write_addr_) {
            const uint16_t old = static_cast<uint16_t>(regs[*write_addr_] & 0x3FFF);
            Answer(old, rx);
            regs[*write_addr_] = word & 0x3FFF;
            pending_ = regs[*write_addr_];  // the next frame returns the new content
            write_addr_.reset();
            return;
        }
        Answer(pending_, rx);
        const uint16_t addr = word & 0x3FFF;
        if ((tx[0] & 0x40u) != 0) {
            pending_ = ReadValue(addr);
        } else {
            write_addr_ = addr;
        }
    }

    void Answer(uint16_t value, uint8_t* rx) const {
        rx[0] = static_cast<uint8_t>((value >> 8) & 0x3F);
        rx[1] = static_cast<uint8_t>(value & 0xFF);
        const uint8_t crc = Crc8(static_cast<uint16_t>((rx[0] << 8) | rx[1]));
        rx[2] = corrupt_miso_crc ? static_cast<uint8_t>(~crc) : crc;
    }

    uint16_t pending_{0};
    std::optional<uint16_t> write_addr_;
};

}  // namespace

HF_TEST(As5047u_InitializeAppliesConfigurationOverTheWire) {
    MockSpi spi;
    As5047uModel enc(spi);
    enc.regs[kAnglecom] = 0x1234;

    As5047uConfig cfg = As5047uHandler::GetDefaultConfig();
    cfg.zero_position = 0x0ABC;
    As5047uHandler handler(spi, cfg);
    CHECK(!handler.IsInitialized());
    CHECK_EQ(enc.frames, 0);

    CHECK(handler.Initialize());
    CHECK(handler.IsInitialized());
    CHECK(handler.GetLastError() == AS5047U_Error::None);
    // Zero position split across ZPOSM (bits 13..6) and ZPOSL (bits 5..0).
    CHECK_EQ(enc.regs[kZposm], static_cast<uint16_t>((0x0ABC >> 6) & 0xFF));
    CHECK_EQ(enc.regs[kZposl] & 0x3Fu, static_cast<uint16_t>(0x0ABC & 0x3F));

    const uint16_t angle = handler.visitDriver([](auto& drv) { return drv.GetAngle(); });
    CHECK_EQ(angle, 0x1234u);
}

HF_TEST(As5047u_DiagnosticsHealthyWhenNoErrorFlags) {
    MockSpi spi;
    As5047uModel enc(spi);
    As5047uHandler handler(spi);
    REQUIRE(handler.Initialize());
    const As5047uDiagnostics d = handler.GetDiagnostics();
    CHECK(d.magnetic_field_ok);
    CHECK(d.communication_ok);
    CHECK(d.offset_compensation_ok);
    CHECK(!d.agc_warning);
    CHECK(!d.cordic_overflow);
    CHECK_EQ(d.last_error_flags, 0u);
}

HF_TEST(As5047u_DiagnosticsReflectPersistentFieldErrors) {
    MockSpi spi;
    As5047uModel enc(spi);
    As5047uHandler handler(spi);
    REQUIRE(handler.Initialize());

    enc.errfl_persistent = kMagHalf | kAgcWarning | kCordicOverflow;
    CHECK(handler.RefreshDiagnostics());
    const As5047uDiagnostics d = handler.GetDiagnostics();
    CHECK(!d.magnetic_field_ok);
    CHECK(d.agc_warning);
    CHECK(d.cordic_overflow);
    CHECK(d.communication_ok);  // field problems are not bus problems
    CHECK_EQ(d.last_error_flags, static_cast<uint16_t>(kMagHalf | kAgcWarning | kCordicOverflow));

    enc.errfl_persistent = 0;  // magnet back in place
    CHECK(handler.RefreshDiagnostics());
    CHECK(handler.GetDiagnostics().magnetic_field_ok);
}

HF_TEST(As5047u_DiagnosticsFlagCommunicationErrors) {
    MockSpi spi;
    As5047uModel enc(spi);
    As5047uHandler handler(spi);
    REQUIRE(handler.Initialize());
    enc.errfl_persistent = kCrcError;
    CHECK(handler.RefreshDiagnostics());
    CHECK(!handler.GetDiagnostics().communication_ok);
}

HF_TEST(As5047u_InitializeFailsOnFloatingMiso) {
    // No sensor: MISO floats high, so every write read-back is 0x3FFF and
    // never matches. Configuration fails and the handler stays uninitialized.
    MockSpi spi;
    spi.SetFillByte(0xFF);
    As5047uConfig cfg = As5047uHandler::GetDefaultConfig();
    cfg.crc_retries = 0;
    As5047uHandler handler(spi, cfg);
    CHECK(!handler.Initialize());
    CHECK(!handler.IsInitialized());
    CHECK(!handler.RefreshDiagnostics());
    CHECK(!spi.Transactions().empty());
}
