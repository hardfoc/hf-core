/**
 * @file test_pca9685.cpp
 * @brief Pca9685Handler over MockI2c's auto-incrementing register model.
 *
 * Checks the datasheet behaviour the handler now relies on from the driver:
 * MODE1.AI set at reset (7.3.1), the full-ON / full-OFF bits for 100 % and
 * 0 % duty (7.3.3), and never ON == OFF ticks (7.3.4).
 */

#include "HfCoreMocks.h"
#include "HostTest.h"
#include "Pca9685Handler.h"

using hf::host::MockI2c;

namespace {

constexpr uint8_t kMode1 = 0x00;
constexpr uint8_t kMode1Ai = 0x20;
constexpr uint8_t kMode1Sleep = 0x10;
constexpr uint8_t kLed0OnL = 0x06;
constexpr uint8_t kFullBit = 0x10;  // bit 4 of LEDn_ON_H / LEDn_OFF_H

uint8_t OnH(const MockI2c& i2c, uint8_t ch) { return i2c.GetRegister(kLed0OnL + 4 * ch + 1); }
uint8_t OffH(const MockI2c& i2c, uint8_t ch) { return i2c.GetRegister(kLed0OnL + 4 * ch + 3); }
uint16_t On(const MockI2c& i2c, uint8_t ch) {
    return static_cast<uint16_t>(i2c.GetRegister(kLed0OnL + 4 * ch) | ((OnH(i2c, ch) & 0x0F) << 8));
}
uint16_t Off(const MockI2c& i2c, uint8_t ch) {
    return static_cast<uint16_t>(i2c.GetRegister(kLed0OnL + 4 * ch + 2) |
                                 ((OffH(i2c, ch) & 0x0F) << 8));
}

}  // namespace

HF_TEST(Pca9685_InitializeSetsAutoIncrementAndWakes) {
    MockI2c i2c(0x40);
    i2c.EnableRegisterModel();
    Pca9685Handler pca(i2c);
    REQUIRE(pca.EnsureInitialized());
    const uint8_t mode1 = i2c.GetRegister(kMode1);
    CHECK((mode1 & kMode1Ai) != 0);
    CHECK((mode1 & kMode1Sleep) == 0);
}

HF_TEST(Pca9685_DutyEndsUseTheFullOnAndFullOffBits) {
    MockI2c i2c(0x40);
    i2c.EnableRegisterModel();
    Pca9685Handler pca(i2c);
    REQUIRE(pca.EnsureInitialized());
    auto pwm = pca.GetPwmAdapter();
    REQUIRE(pwm != nullptr);

    REQUIRE(pwm->SetDutyCycle(3, 1.0f) == hf_pwm_err_t::PWM_SUCCESS);
    CHECK((OnH(i2c, 3) & kFullBit) != 0);
    CHECK((OffH(i2c, 3) & kFullBit) == 0);

    REQUIRE(pwm->SetDutyCycle(3, 0.0f) == hf_pwm_err_t::PWM_SUCCESS);
    CHECK((OffH(i2c, 3) & kFullBit) != 0);

    REQUIRE(pwm->SetDutyCycle(3, 0.25f) == hf_pwm_err_t::PWM_SUCCESS);
    CHECK((OnH(i2c, 3) & kFullBit) == 0);
    CHECK((OffH(i2c, 3) & kFullBit) == 0);
    CHECK_EQ(On(i2c, 3), 0u);
    CHECK_NEAR(Off(i2c, 3), 1024.0, 1.0);
}

HF_TEST(Pca9685_EqualOnAndOffTicksBecomeFullOff) {
    MockI2c i2c(0x40);
    i2c.EnableRegisterModel();
    Pca9685Handler pca(i2c);
    REQUIRE(pca.EnsureInitialized());
    auto pwm = pca.GetPwmAdapter();
    REQUIRE(pwm != nullptr);
    // Raw 0 at phase 0 is ON == OFF == 0: the driver writes full-OFF instead.
    REQUIRE(pwm->SetDutyCycleRaw(5, 0) == hf_pwm_err_t::PWM_SUCCESS);
    CHECK((OffH(i2c, 5) & kFullBit) != 0);
    REQUIRE(pwm->SetDutyCycleRaw(5, 2048) == hf_pwm_err_t::PWM_SUCCESS);
    CHECK((OffH(i2c, 5) & kFullBit) == 0);
    CHECK_EQ(Off(i2c, 5), 2048u);
}
