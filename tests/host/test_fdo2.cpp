/**
 * @file test_fdo2.cpp
 * @brief Fdo2Handler and its BaseUart adapter over MockUart.
 *
 * The FDO2 driver parses what one read() returns as the whole reply, and a
 * reply without its CR is a ProtocolError (data sheet §4.1). The adapter must
 * therefore return exactly one complete line, keep a partial line across
 * calls, and drop it on flush_rx().
 */

#include "Fdo2Handler.h"
#include "HfCoreMocks.h"
#include "HostTest.h"

#include <cstring>
#include <string>

using hf::host::MockUart;

namespace {

std::string ReadLine(HalUartFdo2Comm& comm, std::uint32_t timeout_ms) {
    std::uint8_t buf[160] = {};
    const std::size_t n = comm.read(buf, sizeof(buf), timeout_ms);
    return std::string(reinterpret_cast<const char*>(buf), n);
}

}  // namespace

HF_TEST(Fdo2_ReadReturnsOneCompleteLineAtATime) {
    MockUart uart;
    HalUartFdo2Comm comm(uart);
    uart.InjectRx("#VERS 8 1 357 15\r#IDNR 42\r");
    CHECK(ReadLine(comm, 0) == "#VERS 8 1 357 15\r");
    CHECK(ReadLine(comm, 0) == "#IDNR 42\r");
    CHECK(ReadLine(comm, 0).empty());
}

HF_TEST(Fdo2_ReadKeepsAPartialLineUntilItsCrArrives) {
    MockUart uart;
    HalUartFdo2Comm comm(uart);
    uart.InjectRx("#MOXY 0 210");
    CHECK(ReadLine(comm, 0).empty());   // poll: nothing complete yet
    CHECK(ReadLine(comm, 20).empty());  // timeout: still incomplete, kept
    uart.InjectRx("00 2500\r");
    CHECK(ReadLine(comm, 0) == "#MOXY 0 21000 2500\r");
}

HF_TEST(Fdo2_FlushDropsAPartialLine) {
    MockUart uart;
    HalUartFdo2Comm comm(uart);
    uart.InjectRx("#MOX");
    CHECK(ReadLine(comm, 0).empty());
    comm.flush_rx();
    uart.InjectRx("#IDNR 7\r");
    CHECK(ReadLine(comm, 0) == "#IDNR 7\r");
}

HF_TEST(Fdo2_HandlerReadsVersionThroughTheAdapter) {
    MockUart uart;
    uart.SetOnWrite([](MockUart& u, const std::vector<uint8_t>& written) {
        const std::string cmd(written.begin(), written.end());
        if (cmd == "#VERS\r") {
            u.InjectRx("#VERS 8 1 357 15\r");
        }
    });
    Fdo2Handler fdo2(uart);
    CHECK(fdo2.EnsureInitialized());
    const auto v = fdo2.ReadVersion();
    REQUIRE(v.ok());
    CHECK_EQ(v.value.device_id, 8);
    CHECK_EQ(v.value.firmware_revision, 357);
}
