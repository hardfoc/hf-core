/**
 * @file test_ntc.cpp
 * @brief NtcTemperatureHandler voltage -> resistance -> temperature on MockAdc.
 *
 * Divider: NTC on the low side, 10 kΩ series resistor to 3.3 V, so
 * V = 3.3 * R / (10k + R). The NTCG163JFT103FT1S is 10 kΩ at 25 °C with
 * β = 3435 K; expected temperatures below come from the β equation and the
 * tolerance covers the difference to the driver's lookup table.
 */

#include "HfCoreMocks.h"
#include "HostTest.h"
#include "NtcTemperatureHandler.h"

#include <cmath>

using hf::host::MockAdc;

namespace {

constexpr hf_channel_id_t kChannel = 2;
constexpr float kVref = 3.3f;
constexpr float kSeries = 10000.0f;

float VoltageForResistance(float r) { return kVref * r / (kSeries + r); }

double BetaCelsius(double r) {
    const double inv_t = 1.0 / 298.15 + std::log(r / 10000.0) / 3435.0;
    return 1.0 / inv_t - 273.15;
}

ntc_temp_handler_config_t Config() {
    ntc_temp_handler_config_t cfg = NTC_TEMP_HANDLER_CONFIG_DEFAULT();
    cfg.adc_channel = kChannel;
    cfg.voltage_divider_series_resistance = kSeries;
    cfg.reference_voltage = kVref;
    return cfg;
}

}  // namespace

HF_TEST(Ntc_ReadsTwentyFiveDegreesAtMidScale) {
    MockAdc adc(4, kVref, 12);
    adc.SetVoltage(kChannel, VoltageForResistance(10000.0f));
    NtcTemperatureHandler ntc(&adc, Config());
    REQUIRE(ntc.EnsureInitialized());

    float c = -100.0f;
    CHECK(ntc.ReadTemperatureCelsius(&c) == hf_temp_err_t::TEMP_SUCCESS);
    CHECK_NEAR(c, 25.0, 0.5);

    float r = 0.0f;
    CHECK(ntc.GetResistance(&r) == NtcError::Success);
    CHECK_NEAR(r, 10000.0, 50.0);
    CHECK(adc.ReadCount(kChannel) >= 1u);
}

HF_TEST(Ntc_FollowsBetaCurveHotAndCold) {
    MockAdc adc(4, kVref, 12);
    NtcTemperatureHandler ntc(&adc, Config());
    REQUIRE(ntc.EnsureInitialized());

    const float resistances[] = {3000.0f, 5000.0f, 20000.0f, 30000.0f};
    float previous = 1000.0f;
    for (float r : resistances) {
        adc.SetVoltage(kChannel, VoltageForResistance(r));
        float c = 0.0f;
        CHECK(ntc.ReadTemperatureCelsius(&c) == hf_temp_err_t::TEMP_SUCCESS);
        CHECK_NEAR(c, BetaCelsius(r), 1.5);
        CHECK(c < previous);  // higher resistance is colder
        previous = c;
    }
}

HF_TEST(Ntc_CalibrationOffsetShiftsReading) {
    MockAdc adc(4, kVref, 12);
    adc.SetVoltage(kChannel, VoltageForResistance(10000.0f));
    NtcTemperatureHandler ntc(&adc, Config());
    REQUIRE(ntc.EnsureInitialized());
    float base = 0.0f;
    REQUIRE(ntc.ReadTemperatureCelsius(&base) == hf_temp_err_t::TEMP_SUCCESS);
    CHECK(ntc.SetCalibrationOffset(2.0f) == hf_temp_err_t::TEMP_SUCCESS);
    float shifted = 0.0f;
    CHECK(ntc.ReadTemperatureCelsius(&shifted) == hf_temp_err_t::TEMP_SUCCESS);
    CHECK_NEAR(shifted - base, 2.0, 0.05);
}

HF_TEST(Ntc_AdcFailureIsReportedNotConverted) {
    MockAdc adc(4, kVref, 12);
    adc.SetVoltage(kChannel, VoltageForResistance(10000.0f));
    NtcTemperatureHandler ntc(&adc, Config());
    REQUIRE(ntc.EnsureInitialized());
    adc.FailChannel(kChannel);
    float c = 123.0f;
    CHECK(ntc.ReadTemperatureCelsius(&c) != hf_temp_err_t::TEMP_SUCCESS);
    adc.ClearFailure(kChannel);
    CHECK(ntc.ReadTemperatureCelsius(&c) == hf_temp_err_t::TEMP_SUCCESS);
}

HF_TEST(Ntc_RailVoltagesAreRejected) {
    // An open thermistor reads the reference voltage; a short reads 0 V.
    MockAdc adc(4, kVref, 12);
    NtcTemperatureHandler ntc(&adc, Config());
    REQUIRE(ntc.EnsureInitialized());
    float c = 0.0f;
    adc.SetVoltage(kChannel, kVref);
    CHECK(ntc.ReadTemperatureCelsius(&c) != hf_temp_err_t::TEMP_SUCCESS);
    adc.SetVoltage(kChannel, 0.0f);
    CHECK(ntc.ReadTemperatureCelsius(&c) != hf_temp_err_t::TEMP_SUCCESS);
}

HF_TEST(Ntc_NullAdcCannotInitialize) {
    NtcTemperatureHandler ntc(nullptr, Config());
    CHECK(!ntc.EnsureInitialized());
}

HF_TEST(Ntc_ConfiguredChannelMustExist) {
    MockAdc adc(4, kVref, 12);
    ntc_temp_handler_config_t cfg = Config();
    cfg.adc_channel = 9;
    NtcTemperatureHandler ntc(&adc, cfg);
    CHECK(!ntc.EnsureInitialized());
}
