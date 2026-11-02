// The host-test example of docs/host-build.md. Compiled by the host build (target hf_core_doc_snippets);
// tests/host/test_pcal95555.cpp is the full suite.

// --8<-- [start:host-test]
#include "HfCoreMocks.h"
#include "HostTest.h"
#include "Pcal95555Handler.h"

HF_TEST(Expander_DrivesPinThreeAsOutput) {
  hf::host::MockI2c i2c(0x21);    // BaseI2c fake at the expander's address
  i2c.EnableRegisterModel();      // reads return what was written
  i2c.SetRegister(0x06, 0xFF);    // CONFIG0: all pins inputs at power-on
  i2c.SetRegister(0x07, 0xFF);

  Pcal95555Handler expander(i2c);
  REQUIRE(expander.EnsureInitialized());
  CHECK(expander.SetDirection(3, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT) ==
        hf_gpio_err_t::GPIO_SUCCESS);
  CHECK((i2c.GetRegister(0x06) & (1u << 3)) == 0);  // bit 3 cleared: output
}
// --8<-- [end:host-test]
