/**
 * @file test_mocks.cpp
 * @brief Behaviour of the hf_core_host_mocks fakes themselves.
 *
 * Handler tests rely on these semantics; a HAL that reuses the mocks can
 * read this file as their specification.
 */

#include "HfCoreMocks.h"
#include "HostTest.h"

#include <cstring>

using namespace hf::host;

HF_TEST(MockI2c_RegisterModelAutoIncrements) {
    MockI2c i2c(0x21);
    i2c.EnableRegisterModel();
    const uint8_t write[] = {0x06, 0xAA, 0x55};
    CHECK(i2c.Write(write, sizeof(write)) == hf_i2c_err_t::I2C_SUCCESS);
    CHECK_EQ(i2c.GetRegister(0x06), 0xAAu);
    CHECK_EQ(i2c.GetRegister(0x07), 0x55u);

    uint8_t reg = 0x06;
    uint8_t rx[2] = {};
    CHECK(i2c.WriteRead(&reg, 1, rx, 2) == hf_i2c_err_t::I2C_SUCCESS);
    CHECK_EQ(rx[0], 0xAAu);
    CHECK_EQ(rx[1], 0x55u);
    CHECK_EQ(i2c.Transactions().size(), 2u);
    CHECK_EQ(i2c.Transactions()[1].address, 0x21u);
    CHECK_EQ(i2c.WritesTo(0x06), 1u);
}

HF_TEST(MockI2c_ReadOnlyRegisterIgnoresWrites) {
    MockI2c i2c;
    i2c.EnableRegisterModel();
    i2c.SetRegister(0x00, 0x3C);
    i2c.SetReadOnly(0x00);
    const uint8_t write[] = {0x00, 0xFF};
    CHECK(i2c.Write(write, sizeof(write)) == hf_i2c_err_t::I2C_SUCCESS);
    CHECK_EQ(i2c.GetRegister(0x00), 0x3Cu);
}

HF_TEST(MockI2c_QueuedResponsesWinAndFailuresAreLogged) {
    MockI2c i2c;
    i2c.EnableRegisterModel();
    i2c.SetRegister(0x10, 0x11);
    i2c.QueueRead({0xDE, 0xAD});
    uint8_t reg = 0x10;
    uint8_t rx[2] = {};
    CHECK(i2c.WriteRead(&reg, 1, rx, 2) == hf_i2c_err_t::I2C_SUCCESS);
    CHECK_EQ(rx[0], 0xDEu);
    CHECK_EQ(rx[1], 0xADu);

    i2c.FailNext(hf_i2c_err_t::I2C_ERR_DEVICE_NACK, 2);
    CHECK(i2c.WriteRead(&reg, 1, rx, 1) == hf_i2c_err_t::I2C_ERR_DEVICE_NACK);
    CHECK(i2c.Read(rx, 1) == hf_i2c_err_t::I2C_ERR_DEVICE_NACK);
    CHECK(i2c.WriteRead(&reg, 1, rx, 1) == hf_i2c_err_t::I2C_SUCCESS);
    CHECK_EQ(rx[0], 0x11u);
    CHECK_EQ(i2c.Transactions().size(), 4u);
}

HF_TEST(MockSpi_ResponderSeesEveryFrame) {
    MockSpi spi;
    int frames = 0;
    spi.SetResponder([&frames](const uint8_t* tx, uint8_t* rx, uint16_t len) {
        ++frames;
        for (uint16_t i = 0; i < len; ++i) {
            rx[i] = static_cast<uint8_t>(~tx[i]);
        }
    });
    const uint8_t tx[3] = {0x01, 0x02, 0x03};
    uint8_t rx[3] = {};
    CHECK(spi.Transfer(tx, rx, 3) == hf_spi_err_t::SPI_SUCCESS);
    CHECK_EQ(rx[2], 0xFCu);
    spi.QueueResponse({0x42});
    CHECK(spi.Transfer(tx, rx, 3) == hf_spi_err_t::SPI_SUCCESS);
    CHECK_EQ(rx[0], 0x42u);
    CHECK_EQ(rx[1], 0x00u);  // fill byte past the queued bytes
    CHECK_EQ(frames, 1);
    spi.FailNext(hf_spi_err_t::SPI_ERR_TRANSFER_TIMEOUT);
    CHECK(spi.Transfer(tx, rx, 3) == hf_spi_err_t::SPI_ERR_TRANSFER_TIMEOUT);
    CHECK_EQ(spi.Transactions().size(), 3u);
}

HF_TEST(MockGpio_ActiveLowMappingAndHistory) {
    MockGpio rst(4, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT, hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
    CHECK(rst.EnsureInitialized());
    CHECK(rst.SetActive() == hf_gpio_err_t::GPIO_SUCCESS);
    CHECK(rst.DrivenLevel() == hf_gpio_level_t::HF_GPIO_LEVEL_LOW);
    CHECK(rst.SetInactive() == hf_gpio_err_t::GPIO_SUCCESS);
    CHECK(rst.DrivenLevel() == hf_gpio_level_t::HF_GPIO_LEVEL_HIGH);
    CHECK_EQ(rst.LevelHistory().size(), 2u);

    MockGpio input(5);
    CHECK(input.EnsureInitialized());
    input.SetInputLevel(hf_gpio_level_t::HF_GPIO_LEVEL_HIGH);
    bool active = false;
    CHECK(input.IsActive(active) == hf_gpio_err_t::GPIO_SUCCESS);
    CHECK(active);
}

namespace {
int g_irq_hits = 0;
void CountIrq(BaseGpio*, hf_gpio_interrupt_trigger_t, void* user) {
    ++g_irq_hits;
    *static_cast<int*>(user) += 10;
}
}  // namespace

HF_TEST(MockGpio_InterruptOnlyWhenEnabled) {
    MockGpio irq(7);
    int user = 0;
    CHECK(irq.ConfigureInterrupt(hf_gpio_interrupt_trigger_t::HF_GPIO_INTERRUPT_TRIGGER_FALLING_EDGE, CountIrq,
                                 &user) == hf_gpio_err_t::GPIO_SUCCESS);
    CHECK(!irq.FireInterrupt());
    CHECK(irq.EnableInterrupt() == hf_gpio_err_t::GPIO_SUCCESS);
    CHECK(irq.FireInterrupt());
    CHECK_EQ(g_irq_hits, 1);
    CHECK_EQ(user, 10);
}

HF_TEST(MockAdc_VoltageCountAndQueue) {
    MockAdc adc(4, 3.3f, 12);
    adc.SetVoltage(1, 1.65f);
    hf_u32_t count = 0;
    float volts = 0.0f;
    CHECK(adc.ReadChannel(1, count, volts) == hf_adc_err_t::ADC_SUCCESS);
    CHECK_NEAR(volts, 1.65, 1e-6);
    CHECK_EQ(count, 2048u);  // round(0.5 * 4095)

    adc.QueueVoltages(1, {0.1f, 0.2f});
    CHECK(adc.ReadChannelV(1, volts) == hf_adc_err_t::ADC_SUCCESS);
    CHECK_NEAR(volts, 0.1, 1e-6);
    CHECK(adc.ReadChannelV(1, volts) == hf_adc_err_t::ADC_SUCCESS);
    CHECK_NEAR(volts, 0.2, 1e-6);
    CHECK(adc.ReadChannelV(1, volts) == hf_adc_err_t::ADC_SUCCESS);
    CHECK_NEAR(volts, 1.65, 1e-6);

    CHECK(adc.ReadChannelV(4, volts) == hf_adc_err_t::ADC_ERR_INVALID_CHANNEL);
    adc.FailChannel(1);
    CHECK(adc.ReadChannelV(1, volts) == hf_adc_err_t::ADC_ERR_CHANNEL_READ_ERR);
    CHECK_EQ(adc.ReadCount(1), 5u);
}

namespace {
int g_can_rx = 0;
hf_u32_t g_can_last_id = 0;
void OnCan(const hf_can_message_t& msg) {
    ++g_can_rx;
    g_can_last_id = msg.id;
}
}  // namespace

HF_TEST(MockCan_SendFilterAndCallback) {
    MockCan can;
    CHECK(can.EnsureInitialized());
    CHECK(can.SendMessage(MockCan::Frame(0x123, {1, 2, 3})) == hf_can_err_t::CAN_SUCCESS);
    REQUIRE(can.SentFrames().size() == 1u);
    CHECK_EQ(can.SentFrames()[0].dlc, 3u);
    CHECK_EQ(can.SentFrames()[0].data[2], 3u);

    CHECK(can.SetAcceptanceFilter(0x100, 0x700) == hf_can_err_t::CAN_SUCCESS);
    CHECK(can.InjectRx(MockCan::Frame(0x181, {0xAA})));
    CHECK(!can.InjectRx(MockCan::Frame(0x281, {0xAA})));
    hf_can_message_t msg;
    CHECK(can.ReceiveMessage(msg) == hf_can_err_t::CAN_SUCCESS);
    CHECK_EQ(msg.id, 0x181u);
    CHECK(can.ReceiveMessage(msg) == hf_can_err_t::CAN_ERR_QUEUE_EMPTY);

    CHECK(can.SetReceiveCallback(OnCan) == hf_can_err_t::CAN_SUCCESS);
    CHECK(can.InjectRx(MockCan::Frame(0x1FF, {})));
    CHECK_EQ(g_can_rx, 1);
    CHECK_EQ(g_can_last_id, 0x1FFu);

    can.SetBusOff(true);
    CHECK(can.SendMessage(MockCan::Frame(0x1, {})) == hf_can_err_t::CAN_ERR_BUS_OFF);
    hf_can_status_t status;
    CHECK(can.GetStatus(status) == hf_can_err_t::CAN_SUCCESS);
    CHECK(status.bus_off);
    CHECK_EQ(status.tx_failed_count, 1u);
}

HF_TEST(MockNvs_TypedStorageAndFaults) {
    MockNvs nvs("cal");
    CHECK(nvs.EnsureInitialized());
    CHECK(nvs.SetU32("gain", 1234) == hf_nvs_err_t::NVS_SUCCESS);
    hf_u32_t v = 0;
    CHECK(nvs.GetU32("gain", v) == hf_nvs_err_t::NVS_SUCCESS);
    CHECK_EQ(v, 1234u);

    char text[8] = {};
    CHECK(nvs.GetString("gain", text, sizeof(text)) == hf_nvs_err_t::NVS_ERR_KEY_NOT_FOUND);
    CHECK(nvs.SetString("name", "axis0") == hf_nvs_err_t::NVS_SUCCESS);
    size_t len = 0;
    CHECK(nvs.GetString("name", text, sizeof(text), &len) == hf_nvs_err_t::NVS_SUCCESS);
    CHECK(std::strcmp(text, "axis0") == 0);
    CHECK_EQ(len, 6u);

    const uint8_t blob[3] = {9, 8, 7};
    CHECK(nvs.SetBlob("b", blob, sizeof(blob)) == hf_nvs_err_t::NVS_SUCCESS);
    uint8_t out[2];
    CHECK(nvs.GetBlob("b", out, sizeof(out)) == hf_nvs_err_t::NVS_ERR_VALUE_TOO_LARGE);

    CHECK(nvs.SetU32("this_key_is_too_long", 1) == hf_nvs_err_t::NVS_ERR_KEY_TOO_LONG);
    nvs.FailNext(hf_nvs_err_t::NVS_ERR_STORAGE_FULL);
    CHECK(nvs.SetU32("x", 1) == hf_nvs_err_t::NVS_ERR_STORAGE_FULL);
    CHECK(!nvs.KeyExists("x"));
    CHECK(nvs.Commit() == hf_nvs_err_t::NVS_SUCCESS);
    CHECK_EQ(nvs.CommitCalls(), 1);
    CHECK(nvs.EraseKey("gain") == hf_nvs_err_t::NVS_SUCCESS);
    CHECK(!nvs.KeyExists("gain"));
}

HF_TEST(MockUart_RequestResponse) {
    MockUart uart(2);
    uart.SetOnWrite([](MockUart& u, const std::vector<uint8_t>& written) {
        if (!written.empty() && written[0] == '?') {
            u.InjectRx("OK\r");
        }
    });
    CHECK(uart.EnsureInitialized());
    const uint8_t q = '?';
    CHECK(uart.Write(&q, 1) == hf_uart_err_t::UART_SUCCESS);
    CHECK_EQ(uart.BytesAvailable(), 3u);
    uint8_t rx[4] = {};
    CHECK(uart.Read(rx, 4) == hf_uart_err_t::UART_ERR_TIMEOUT);  // only 3 buffered
    CHECK(uart.Read(rx, 3) == hf_uart_err_t::UART_SUCCESS);
    CHECK(std::memcmp(rx, "OK\r", 3) == 0);
    CHECK(uart.Printf("v=%d", 5) == 3);
    CHECK_EQ(uart.TxWrites().size(), 2u);
    CHECK_EQ(uart.TxBytes().size(), 4u);
}

HF_TEST(MockTemperature_ReadsThroughBasePath) {
    MockTemperature t;
    CHECK(t.EnsureInitialized());
    t.SetCelsius(41.5f);
    float c = 0.0f;
    CHECK(t.ReadTemperatureCelsius(&c) == hf_temp_err_t::TEMP_SUCCESS);
    CHECK_NEAR(c, 41.5, 1e-6);
    t.Fail();
    CHECK(t.ReadTemperatureCelsius(&c) == hf_temp_err_t::TEMP_ERR_READ_FAILED);
}
