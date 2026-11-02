// Code shown on the documentation site and in the README. Compiled by the host build (target
// hf_core_doc_snippets); the README copy is checked by `hardfoc-docs gen --check`.

// --8<-- [start:quickstart]
#include "HandlerCommon.h"
#include "Pcal95555Handler.h"

// `i2c` is the expander's BaseI2c device, created by the board HAL
// (an EspI2cDevice on ESP32, a hf::host::MockI2c in a host test).
bool BlinkStatusLed(BaseI2c& i2c) {
  Pcal95555Handler expander(i2c);              // no bus traffic yet
  if (!expander.EnsureInitialized()) {         // probes the chip once
    return false;                              // wiring, address or power
  }

  constexpr uint8_t kLedPin = 3;
  if (expander.SetDirection(kLedPin, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT) !=
      hf_gpio_err_t::GPIO_SUCCESS) {
    return false;
  }
  for (int i = 0; i < 6; ++i) {
    if (expander.Toggle(kLedPin) != hf_gpio_err_t::GPIO_SUCCESS) {
      return false;
    }
    handler_utils::DelayMs(250);
  }
  return true;
}
// --8<-- [end:quickstart]
