/**
 * @file test_bmp581_example.cpp
 * @brief The BMP581 cases of the reference handler, over MockI2c's register model.
 *
 * Written by hand next to the generated test_bmp581.cpp, the way a new handler adds its chip's own
 * behaviour: the happy path through VisitDriver(), a bus error mapped once in the adapter, and the
 * diagnostics header that counts both.
 */

#include "Bmp581Handler.h"
#include "HfCoreMocks.h"
#include "HostTest.h"

using hf::Status;
using hf::host::MockI2c;

namespace {

constexpr uint8_t kAddress = 0x46;

void PowerUp(MockI2c& i2c) {
  i2c.EnableRegisterModel();
  i2c.SetRegister(bmp581::reg::kChipId, bmp581::chip_id::kPrimary);
  i2c.SetRegister(bmp581::reg::kStatus, bmp581::status::kNvmRdy);
}

} // namespace

HF_TEST(test_bmp581_example_reads_through_visit_driver) {
  MockI2c i2c(kAddress);
  PowerUp(i2c);
  hf::Bmp581Handler handler(i2c);
  REQUIRE(handler.EnsureInitialized().ok());
  CHECK(handler.IsInitialized());

  // 25.0 degC (25 * 65536 = 0x190000) and 100000 Pa (100000 * 64 = 0x61A800).
  const uint8_t data[6] = {0x00, 0x00, 0x19, 0x00, 0xA8, 0x61};
  for (uint8_t i = 0; i < 6; ++i) {
    i2c.SetRegister(static_cast<uint8_t>(bmp581::reg::kTempDataXlsb + i), data[i]);
  }
  // --8<-- [start:visit]
  bmp581::Sample sample{};
  const hf::Status s = handler.VisitDriver([&](auto& driver) {
    const auto r = driver.ReadSample();
    sample = r.value;
    return hf::ToStatus(r.error);
  });
  // --8<-- [end:visit]
  REQUIRE(s == Status::Ok);
  CHECK_NEAR(sample.temperature_c, 25.0, 1e-3);
  CHECK_NEAR(sample.pressure_pa, 100000.0, 1e-2);

  const auto d = handler.GetDiagnostics();
  CHECK(d.header.initialized);
  CHECK_EQ(d.header.ok_count, 2U); // initialisation + the visit
  CHECK_EQ(d.header.error_count, 0U);
}

HF_TEST(test_bmp581_example_wrong_chip_is_not_found) {
  MockI2c i2c(kAddress);
  PowerUp(i2c);
  i2c.SetRegister(bmp581::reg::kChipId, 0x42);
  hf::Bmp581Handler handler(i2c);
  const auto r = handler.EnsureInitialized();
  CHECK_EQ(r.status, Status::NotFound);
  CHECK_EQ(r.detail, static_cast<uint16_t>(bmp581::DriverError::InvalidChipId));
  CHECK(!handler.IsInitialized());
}

HF_TEST(test_bmp581_example_bus_error_is_mapped_and_counted) {
  MockI2c i2c(kAddress);
  PowerUp(i2c);
  hf::Bmp581Handler handler(i2c);
  REQUIRE(handler.EnsureInitialized().ok());
  i2c.FailNext(hf_i2c_err_t::I2C_ERR_DEVICE_NACK);
  // --8<-- [start:errors]
  const Status s =
      handler.VisitDriver([](auto& driver) { return hf::ToStatus(driver.ReadSample().error); });
  CHECK_EQ(s, Status::Bus);
  const hf::Bmp581Diagnostics d = handler.GetDiagnostics();
  CHECK_EQ(d.header.error_count, 1U);
  CHECK_EQ(d.header.last_error, Status::Bus); // VisitDriver() counts what fn returns
  // --8<-- [end:errors]
}
