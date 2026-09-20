---
layout: default
title: Dynamixel Handler
parent: Handlers
nav_order: 18
---

# DynamixelHandler

HAL integration for ROBOTIS Protocol 2.0 Dynamixel servos via
`hf-dynamixel-driver` (official SDK wrap, not a reimplemented protocol).

Enable with `-DHF_CORE_ENABLE_DYNAMIXEL=ON` (default **OFF**; ESP32 core
examples turn it **ON**). UART is auto-enabled. Compile define:
`HARDFOC_DYNAMIXEL_SUPPORT=1`.

## Usage

```cpp
DynamixelHandlerConfig cfg{};
cfg.servo_id = 1;
cfg.transport.baud_rate = 57600;
cfg.bind_xseries_fallback = true;   // later larger X-series after ping

DynamixelHandler dxl(uart /* already configured BaseUart */, cfg);

if (!dxl.EnsureInitialized()) {
    return;                         // ping failed
}

const dynamixel::PingInfo& id = dxl.Identity();
(void)id.model_number;              // 1070 = XC430-W150

auto tel = dxl.ReadTelemetry();     // torque stays off
if (tel.ok() && dxl.Model() && dxl.Model()->effort_is_current) {
    // address 126 is current on XM/XH, load on XC/XL
}

// Motion is explicit. Identify never enables torque.
(void)dxl.SetOperatingMode(dynamixel::OperatingMode::Position);
(void)dxl.SetMotionProfile(10, 50);
(void)dxl.SetGoalPosition(tel.value.position + 64);
(void)dxl.SetTorqueEnabled(true);
```

## Notes

- The handler does not own DIR GPIO or half-duplex wiring. Configure that on
  the `BaseUart` (or use the driver's ESP32 example adapter when you are
  not going through hf-core).
- `EnsureInitialized()` opens the bus and calls `Device::Identify()`. If the
  model number is unknown and `bind_xseries_fallback` is true, it binds the
  shared X-series table so present/goal position still work.
- `Bus` is not thread-safe. Every public method takes `RtosMutex`. Pass a
  shared mutex only when another protocol uses the same UART.
- Do not also link a second Dynamixel SDK into the same image.
- First validated servo: XC430-W150. XM430/XH430 numbers are in the catalog;
  any other larger X-series uses the fallback until you add a descriptor.

Driver repo: [`hf-dynamixel-driver`](https://github.com/N3b3x/hf-dynamixel-driver)
· docs: [model support](https://n3b3x.github.io/hf-dynamixel-driver/docs/model_support/)
· [ESP32 examples](https://n3b3x.github.io/hf-dynamixel-driver/docs/esp32_examples/).
