/**
 * @file test_bno08x.cpp
 * @brief Bno08xHandler over MockI2c, with a minimal SH-2 hub model.
 *
 * The model speaks just enough SHTP for the handler's bring-up path:
 *  - after the host opens the link it reports "reset complete" on the
 *    executable channel (1);
 *  - it answers a Command Request INITIALIZE (0xF2 / cmd 4) on the control
 *    channel (2) with a Command Response carrying the request's sequence;
 *  - it records Set Feature (0xFD) requests, which enable sensors;
 *  - a test can push sensor reports on the input channel (3).
 * Everything else reads back as an empty bus (zero-length packets).
 */

#include "Bno08xHandler.h"
#include "HfCoreMocks.h"
#include "HostTest.h"

#include <algorithm>
#include <cstring>
#include <deque>
#include <map>

using hf::host::FakeClock;
using hf::host::I2cTransaction;
using hf::host::MockGpio;
using hf::host::MockI2c;

namespace {

constexpr uint8_t kChanExecutable = 1;
constexpr uint8_t kChanControl = 2;
constexpr uint8_t kChanInput = 3;

class Sh2HubModel {
public:
    explicit Sh2HubModel(MockI2c& i2c) {
        i2c.SetResponder([this](const I2cTransaction& txn, uint8_t* rx) { return OnI2c(txn, rx); });
    }

    bool answer_initialize{true};
    uint8_t initialize_status{0};  ///< r[0] of the INITIALIZE response; non-zero is a hub error.
    bool send_reset_complete{true};

    std::map<uint8_t, uint32_t> enabled_intervals_us;  ///< sensor id -> report interval
    int initialize_requests{0};

    void PushPacket(uint8_t chan, const std::vector<uint8_t>& payload) {
        std::vector<uint8_t> pkt;
        const auto len = static_cast<uint16_t>(payload.size() + 4);
        pkt.push_back(static_cast<uint8_t>(len & 0xFF));
        pkt.push_back(static_cast<uint8_t>(len >> 8));
        pkt.push_back(chan);
        pkt.push_back(out_seq_[chan]++);
        pkt.insert(pkt.end(), payload.begin(), payload.end());
        out_.push_back(std::move(pkt));
    }

    /** Accelerometer report (Q8, m/s^2) preceded by a base-timestamp reference. */
    void PushAccelerometer(int16_t x, int16_t y, int16_t z) {
        std::vector<uint8_t> p = {0xFB, 0, 0, 0, 0};  // base timestamp reference, delta 0
        const auto ux = static_cast<uint16_t>(x);
        const auto uy = static_cast<uint16_t>(y);
        const auto uz = static_cast<uint16_t>(z);
        const uint8_t report[10] = {0x01, report_seq_++, 0x03 /*accuracy high*/, 0,
                                    static_cast<uint8_t>(ux), static_cast<uint8_t>(ux >> 8),
                                    static_cast<uint8_t>(uy), static_cast<uint8_t>(uy >> 8),
                                    static_cast<uint8_t>(uz), static_cast<uint8_t>(uz >> 8)};
        p.insert(p.end(), report, report + sizeof(report));
        PushPacket(kChanInput, p);
    }

private:
    hf_i2c_err_t OnI2c(const I2cTransaction& txn, uint8_t* rx) {
        if (txn.kind == I2cTransaction::Kind::Write) {
            OnHostPacket(txn.tx);
            return hf_i2c_err_t::I2C_SUCCESS;
        }
        if (!reset_sent_ && send_reset_complete) {
            reset_sent_ = true;
            PushPacket(kChanExecutable, {0x01});  // EXECUTABLE_DEVICE_RESP_RESET_COMPLETE
        }
        std::memset(rx, 0, txn.rx_length);
        if (!out_.empty()) {
            const auto& pkt = out_.front();
            std::memcpy(rx, pkt.data(), std::min<size_t>(pkt.size(), txn.rx_length));
            out_.pop_front();
        }
        // Otherwise an all-zero header: nothing to deliver.
        return hf_i2c_err_t::I2C_SUCCESS;
    }

    void OnHostPacket(const std::vector<uint8_t>& pkt) {
        if (pkt.size() < 5 || pkt[2] != kChanControl) {
            return;
        }
        const uint8_t* p = pkt.data() + 4;
        if (p[0] == 0xF2 && pkt.size() >= 7 && p[2] == 4) {  // Command Request: INITIALIZE
            ++initialize_requests;
            if (answer_initialize) {
                std::vector<uint8_t> resp(16, 0);
                resp[0] = 0xF1;  // Command Response
                resp[2] = 4;     // command
                resp[3] = p[1];  // sequence number of the request
                resp[5] = initialize_status;
                PushPacket(kChanControl, resp);
            }
        } else if (p[0] == 0xFD && pkt.size() >= 4 + 9) {  // Set Feature
            const uint32_t interval = static_cast<uint32_t>(p[5]) | (static_cast<uint32_t>(p[6]) << 8) |
                                      (static_cast<uint32_t>(p[7]) << 16) |
                                      (static_cast<uint32_t>(p[8]) << 24);
            enabled_intervals_us[p[1]] = interval;
        }
    }

    bool reset_sent_{false};
    std::deque<std::vector<uint8_t>> out_;
    uint8_t out_seq_[8] = {};
    uint8_t report_seq_{0};
};

}  // namespace

HF_TEST(Bno08x_ConstructionDoesNotTouchTheBus) {
    MockI2c i2c(0x4A);
    Bno08xHandler imu(i2c);
    CHECK(!imu.IsInitialized());
    CHECK(imu.GetInterfaceType() == BNO085Interface::I2C);
    CHECK(std::strstr(imu.GetDescription(), "0x4A") != nullptr);
    CHECK(i2c.Transactions().empty());
}

HF_TEST(Bno08x_InitializeBringsUpHubAndEnablesConfiguredSensors) {
    MockI2c i2c(0x4A);
    Sh2HubModel hub(i2c);
    MockGpio rstn(10, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT);
    Bno08xHandler imu(i2c, Bno08xConfig{}, &rstn);

    CHECK(imu.Initialize() == Bno08xError::SUCCESS);
    CHECK(imu.IsInitialized());
    CHECK_EQ(hub.initialize_requests, 1);

    // Defaults: accelerometer, gyroscope, magnetometer, rotation vector.
    CHECK_EQ(hub.enabled_intervals_us.size(), 4u);
    CHECK_EQ(hub.enabled_intervals_us[0x01], 50000u);   // accelerometer, 50 ms
    CHECK_EQ(hub.enabled_intervals_us[0x02], 50000u);   // gyroscope
    CHECK_EQ(hub.enabled_intervals_us[0x03], 100000u);  // magnetometer, 100 ms
    CHECK_EQ(hub.enabled_intervals_us[0x05], 50000u);   // rotation vector

    // RSTN was pulsed: asserted (low) at some point, released (high) at the end.
    const auto& levels = rstn.LevelHistory();
    REQUIRE(levels.size() >= 2u);
    CHECK(levels.back() == hf_gpio_level_t::HF_GPIO_LEVEL_HIGH);
    CHECK(std::find(levels.begin(), levels.end(), hf_gpio_level_t::HF_GPIO_LEVEL_LOW) != levels.end());

    for (const auto& t : i2c.Transactions()) {
        CHECK_EQ(t.address, 0x4Au);
    }
}

HF_TEST(Bno08x_UpdateDeliversSensorReports) {
    MockI2c i2c(0x4A);
    Sh2HubModel hub(i2c);
    Bno08xHandler imu(i2c);
    REQUIRE(imu.Initialize() == Bno08xError::SUCCESS);

    int events = 0;
    SensorEvent last{};
    imu.SetSensorCallback([&](const SensorEvent& e) {
        ++events;
        last = e;
    });
    hub.PushAccelerometer(256, -512, 2509);  // 1.0, -2.0, 9.80 m/s^2 in Q8
    CHECK(imu.Update() == Bno08xError::SUCCESS);
    CHECK_EQ(events, 1);
    CHECK(last.sensor == BNO085Sensor::Accelerometer);
    CHECK_NEAR(last.vector.x, 1.0, 1e-3);
    CHECK_NEAR(last.vector.y, -2.0, 1e-3);
    CHECK_NEAR(last.vector.z, 9.80, 0.01);
}

HF_TEST(Bno08x_InitializeFailsWhenBusIsDead) {
    // Every transfer NACKs. The advertisement wait (200 ms) runs out on the
    // fake clock, then sending INITIALIZE fails and Initialize() returns.
    // (A hub that ACKs but never answers INITIALIZE is not tested: the
    // vendor SH-2 reinitialize operation has no timeout and would wait forever.)
    MockI2c i2c(0x4A);
    i2c.FailNext(hf_i2c_err_t::I2C_ERR_DEVICE_NACK, 1000000);
    Bno08xHandler imu(i2c);

    const uint64_t t0 = FakeClock::NowUs();
    CHECK(imu.Initialize() == Bno08xError::SENSOR_NOT_RESPONDING);
    CHECK(!imu.IsInitialized());
    CHECK(FakeClock::NowUs() - t0 >= 200000u);
    CHECK(i2c.CountOf(I2cTransaction::Kind::Write) >= 1u);
}

HF_TEST(Bno08x_ResetPulseHappensOnFirstInitialize) {
    // RSTN starts unconfigured; the handler must still assert and release it.
    MockI2c i2c(0x4A);
    Sh2HubModel hub(i2c);
    MockGpio rstn(10);  // input, active-high, not initialized
    Bno08xHandler imu(i2c, Bno08xConfig{}, &rstn);
    REQUIRE(imu.Initialize() == Bno08xError::SUCCESS);
    const auto& levels = rstn.LevelHistory();
    REQUIRE(levels.size() >= 2u);
    CHECK(levels[0] == hf_gpio_level_t::HF_GPIO_LEVEL_LOW);   // asserted (active-low)
    CHECK(levels[1] == hf_gpio_level_t::HF_GPIO_LEVEL_HIGH);  // released
}

HF_TEST(Bno08x_InitializeReportsHubError) {
    MockI2c i2c(0x4A);
    Sh2HubModel hub(i2c);
    hub.initialize_status = 1;
    Bno08xHandler imu(i2c);
    CHECK(imu.Initialize() == Bno08xError::SENSOR_NOT_RESPONDING);
    CHECK(!imu.IsInitialized());
    CHECK(imu.GetLastDriverError() != 0);
}
