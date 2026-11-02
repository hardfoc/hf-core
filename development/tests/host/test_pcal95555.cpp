/**
 * @file test_pcal95555.cpp
 * @brief Pcal95555Handler against a register-model MockI2c.
 *
 * The mock holds the PCA9555 register file (INPUT 0/1, OUTPUT 2/3,
 * POLARITY 4/5, CONFIG 6/7) at power-on values, so reads and writes made by
 * the handler land where the chip would put them.
 */

#include "HfCoreMocks.h"
#include "HostTest.h"
#include "Pcal95555Handler.h"

using hf::host::I2cTransaction;
using hf::host::MockI2c;

namespace {

constexpr uint8_t kAddr = 0x21;
constexpr uint8_t kIn0 = 0x00, kIn1 = 0x01, kOut0 = 0x02, kOut1 = 0x03, kCfg0 = 0x06, kCfg1 = 0x07;

void PowerOn(MockI2c& i2c) {
    i2c.EnableRegisterModel();
    i2c.SetRegister(kOut0, 0xFF);
    i2c.SetRegister(kOut1, 0xFF);
    i2c.SetRegister(kCfg0, 0xFF);  // all inputs
    i2c.SetRegister(kCfg1, 0xFF);
    i2c.SetReadOnly(kIn0);
    i2c.SetReadOnly(kIn1);
}

/** Registers read with WriteRead([reg]) since the last ClearTransactions(). */
std::vector<uint8_t> RegistersRead(const MockI2c& i2c) {
    std::vector<uint8_t> regs;
    for (const auto& t : i2c.Transactions()) {
        if (t.kind == I2cTransaction::Kind::WriteRead && !t.tx.empty()) {
            regs.push_back(t.tx[0]);
        }
    }
    return regs;
}

}  // namespace

HF_TEST(Pcal95555_InitializeProbesInputPortAtStrapAddress) {
    MockI2c i2c(kAddr);
    PowerOn(i2c);
    Pcal95555Handler pcal(i2c);
    CHECK(pcal.EnsureInitialized());
    CHECK_EQ(pcal.GetI2cAddress(), kAddr);
    CHECK(!pcal.HasAgileIO());  // handler forces the PCA9555 register set
    REQUIRE(!i2c.Transactions().empty());
    CHECK(i2c.Transactions()[0].kind == I2cTransaction::Kind::WriteRead);
    CHECK_EQ(i2c.Transactions()[0].tx[0], kIn0);
    CHECK_EQ(i2c.Transactions()[0].address, kAddr);
}

HF_TEST(Pcal95555_InitializeFailsWhenDeviceNacks) {
    MockI2c i2c(kAddr);
    PowerOn(i2c);
    i2c.FailNext(hf_i2c_err_t::I2C_ERR_DEVICE_NACK, 1);
    Pcal95555Handler pcal(i2c);
    CHECK(!pcal.EnsureInitialized());
    uint16_t levels = 0xBEEF;
    // Nothing is cached from the failed attempt: the next call retries and succeeds.
    CHECK(pcal.ReadAllInputLevels(levels) == hf_gpio_err_t::GPIO_SUCCESS);
    CHECK(pcal.EnsureInitialized());
}

HF_TEST(Pcal95555_ReadAllInputLevelsAssemblesBothPorts) {
    MockI2c i2c(kAddr);
    PowerOn(i2c);
    i2c.SetRegister(kIn0, 0x5A);
    i2c.SetRegister(kIn1, 0xC3);
    Pcal95555Handler pcal(i2c);
    REQUIRE(pcal.EnsureInitialized());
    i2c.ClearTransactions();

    uint16_t levels = 0;
    CHECK(pcal.ReadAllInputLevels(levels) == hf_gpio_err_t::GPIO_SUCCESS);
    CHECK_EQ(levels, 0xC35Au);  // port 1 in the high byte
    CHECK(RegistersRead(i2c) == (std::vector<uint8_t>{kIn0, kIn1}));

    i2c.SetRegister(kIn1, 0x01);
    CHECK(pcal.ReadAllInputLevels(levels) == hf_gpio_err_t::GPIO_SUCCESS);
    CHECK_EQ(levels, 0x015Au);
}

HF_TEST(Pcal95555_ReadAllInputLevelsReportsBusError) {
    MockI2c i2c(kAddr);
    PowerOn(i2c);
    Pcal95555Handler pcal(i2c);
    REQUIRE(pcal.EnsureInitialized());
    pcal.ClearErrorFlags();
    // Fail every attempt the driver makes (it retries on a live bus).
    i2c.FailNext(hf_i2c_err_t::I2C_ERR_BUS_TIMEOUT, 16);
    uint16_t levels = 0x1234;
    CHECK(pcal.ReadAllInputLevels(levels) == hf_gpio_err_t::GPIO_ERR_READ_FAILURE);
}

HF_TEST(Pcal95555_CreateGpioPinConfiguresDirectionAndDrivesOutput) {
    MockI2c i2c(kAddr);
    PowerOn(i2c);
    Pcal95555Handler pcal(i2c);

    auto pin9 = pcal.CreateGpioPin(9, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT);
    REQUIRE(pin9 != nullptr);
    CHECK(pcal.IsPinCreated(9));
    CHECK_EQ(pcal.GetCreatedPinMask(), static_cast<uint16_t>(1u << 9));
    CHECK_EQ(i2c.GetRegister(kCfg1) & 0x02u, 0u);    // P1.1 is now an output
    CHECK_EQ(i2c.GetRegister(kCfg0), 0xFFu);         // port 0 untouched

    CHECK(pin9->SetInactive() == hf_gpio_err_t::GPIO_SUCCESS);
    CHECK_EQ(i2c.GetRegister(kOut1) & 0x02u, 0u);
    CHECK(pin9->SetActive() == hf_gpio_err_t::GPIO_SUCCESS);
    CHECK_EQ(i2c.GetRegister(kOut1) & 0x02u, 0x02u);

    // Asking again returns the same pin unless the caller wants exclusivity.
    auto again = pcal.CreateGpioPin(9, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT);
    CHECK(again.get() == pin9.get());
    CHECK(pcal.CreateGpioPin(9, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT,
                             hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH,
                             hf_gpio_output_mode_t::HF_GPIO_OUTPUT_MODE_PUSH_PULL,
                             hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_FLOATING,
                             /*allow_existing=*/false) == nullptr);
    CHECK(pcal.GetGpioPin(9).get() == pin9.get());
}

HF_TEST(Pcal95555_CreateGpioPinInputReadsLevel) {
    MockI2c i2c(kAddr);
    PowerOn(i2c);
    Pcal95555Handler pcal(i2c);
    auto pin2 = pcal.CreateGpioPin(2, hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT,
                                   hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
    REQUIRE(pin2 != nullptr);
    CHECK_EQ(i2c.GetRegister(kCfg0) & 0x04u, 0x04u);  // still an input

    bool active = false;
    i2c.SetRegister(kIn0, 0x00);  // P0.2 low -> active (active-low)
    CHECK(pin2->IsActive(active) == hf_gpio_err_t::GPIO_SUCCESS);
    CHECK(active);
    i2c.SetRegister(kIn0, 0x04);
    CHECK(pin2->IsActive(active) == hf_gpio_err_t::GPIO_SUCCESS);
    CHECK(!active);
}

HF_TEST(Pcal95555_CreateGpioPinRejectsBadPinAndDeadBus) {
    MockI2c i2c(kAddr);
    PowerOn(i2c);
    {
        Pcal95555Handler pcal(i2c);
        CHECK(pcal.CreateGpioPin(16) == nullptr);
        CHECK(pcal.GetGpioPin(3) == nullptr);
    }
    MockI2c dead(kAddr);
    dead.FailNext(hf_i2c_err_t::I2C_ERR_DEVICE_NOT_FOUND, 1000);
    Pcal95555Handler pcal(dead);
    CHECK(pcal.CreateGpioPin(0, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT) == nullptr);
    CHECK_EQ(pcal.GetCreatedPinMask(), 0u);
}
