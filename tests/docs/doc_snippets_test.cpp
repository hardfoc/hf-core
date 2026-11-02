// Runs the documentation snippets against the host mocks, so the pages show code that works, not only code
// that compiles.

#include <cstdint>

#include "HfCoreMocks.h"
#include "HostTest.h"

bool BlinkStatusLed(BaseI2c& i2c);
bool SetHeartbeat(BaseCan& can, uint8_t node_id);

HF_TEST(Docs_QuickstartBlinksOnAnExpander) {
  hf::host::MockI2c i2c(0x20);
  i2c.EnableRegisterModel();
  i2c.SetRegister(0x06, 0xFF);
  i2c.SetRegister(0x07, 0xFF);
  CHECK(BlinkStatusLed(i2c));
}

HF_TEST(Docs_QuickstartReportsADeadBus) {
  hf::host::MockI2c i2c(0x20);
  i2c.FailNext(hf_i2c_err_t::I2C_ERR_DEVICE_NACK, 100);
  CHECK(!BlinkStatusLed(i2c));
}

HF_TEST(Docs_SdoHeartbeatWriteIsAcknowledged) {
  hf::host::MockCan can;
  hf_can_message_t ack{};
  ack.id = 0x580 + 5;
  ack.dlc = 8;
  ack.data[0] = 0x60;  // download acknowledged
  ack.data[1] = 0x17;
  ack.data[2] = 0x10;
  CHECK(can.EnsureInitialized());
  CHECK(can.InjectRx(ack));
  CHECK(SetHeartbeat(can, 5));
  REQUIRE(!can.SentFrames().empty());
  CHECK_EQ(can.SentFrames()[0].id, 0x600u + 5u);
}
