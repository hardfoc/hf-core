# I/O, PWM, LEDs and displays

Handlers for the PCA9555 / PCAL9555A I/O expander, the PCA9685 PWM controller, WS2812 LED strips and the ISF15ACP4
OLED pushbutton.

## Pcal95555Handler

<!-- generated: handler-pcal95555 -->
Unified handler for the PCA9555 / PCAL9555A 16-bit I2C GPIO expander.

- Header: [`handlers/pcal95555/Pcal95555Handler.h`](../../handlers/pcal95555/Pcal95555Handler.h)
- Switch: `HF_CORE_ENABLE_PCAL95555` (default OFF)
- Driver: [hf-pcal95555-driver](https://github.com/hardfoc/hf-pcal95555-driver)
- Host test: [`tests/host/test_pcal95555.cpp`](../../tests/host/test_pcal95555.cpp)

```cpp
Pcal95555Handler(BaseI2c& i2c_device, BaseGpio* interrupt_pin = nullptr);
```

| Group | Public methods |
| --- | --- |
| Initialization and Lifecycle | `EnsureInitialized()`, `EnsureDeinitialized()`, `IsInitialized()` |
| Basic GPIO Operations | `SetDirection()`, `SetOutput()`, `ReadInput()`, `Toggle()`, `SetPullMode()`, `GetPullMode()` |
| Batch GPIO Operations | `SetDirections()`, `SetOutputs()`, `SetOutputsVerified()`, `SetPullModes()` |
| Interrupt Management | `HasInterruptSupport()`, `IsInterruptConfigured()`, `DrainPendingInterrupts()`, `GetAllInterruptMasks()`, `GetAllInterruptStatus()` |
| Pin Factory | `PinCount()`, `GetI2cAddress()`, `AttachStaticPin()`, `CreateGpioPin()`, `GetGpioPin()`, `GetGpioPinRaw()`, `ReadAllInputLevels()`, `ProgramPortsAbsolute()`, `ReadConfigAndOutputPorts()`, `ReadOutputLatchFromWire()`, `ReadStandardRegisterBank()`, `VerifyOutputPinEffect()`, `IsPinCreated()`, `GetCreatedPinMask()` |
| PCAL9555A Advanced Features (Agile I/O) | `HasAgileIO()`, `GetChipVariant()`, `SetPolarityInversion()`, `SetMultiplePolarityInversion()`, `SetInterruptMask()`, `GetInterruptStatus()`, `SetDriveStrength()`, `EnableInputLatch()`, `SetOutputMode()`, `ResetToDefault()` |
| Error Management | `GetErrorFlags()`, `ClearErrorFlags()` |
| Diagnostics | `DumpDiagnostics()` |
<!-- /generated -->

## Pca9685Handler

<!-- generated: handler-pca9685 -->
Unified handler for the PCA9685 16-channel 12-bit PWM controller.

- Header: [`handlers/pca9685/Pca9685Handler.h`](../../handlers/pca9685/Pca9685Handler.h)
- Switch: `HF_CORE_ENABLE_PCA9685` (default OFF)
- Driver: [hf-pca9685-driver](https://github.com/hardfoc/hf-pca9685-driver)

```cpp
Pca9685Handler(BaseI2c& i2c_device);
```

| Group | Public methods |
| --- | --- |
| Initialization and Lifecycle | `EnsureInitialized()`, `EnsureDeinitialized()`, `IsInitialized()` |
| Wrapper Factories | `ChannelCount()`, `GetI2cAddress()`, `GetPwmAdapter()`, `CreateGpioPin()`, `GetGpioPin()`, `IsPinCreated()` |
| Error Management | `GetErrorFlags()`, `ClearErrorFlags()` |
| Diagnostics | `DumpDiagnostics()` |
| Direct Driver Access | `GetDriver()` |
<!-- /generated -->

## Ws2812Handler

<!-- generated: handler-ws2812 -->
Unified handler for WS2812 addressable LED strips using RMT peripheral.

- Header: [`handlers/ws2812/Ws2812Handler.h`](../../handlers/ws2812/Ws2812Handler.h)
- Switch: `HF_CORE_ENABLE_WS2812` (default OFF)
- Driver: [hf-ws2812-rmt-driver](https://github.com/hardfoc/hf-ws2812-rmt-driver)

```cpp
Ws2812Handler(const Config& config);
```

| Group | Public methods |
| --- | --- |
| All | `Initialize()`, `EnsureInitialized()`, `Deinitialize()`, `IsInitialized()`, `GetNumLeds()`, `GetStrip()`, `GetDriver()`, `GetAnimator()`, `visitDriver()`, `visitAnimator()`, `DumpDiagnostics()`, `GetDescription()`, `GetDefaultConfig()` |
<!-- /generated -->

## Isf15acp4Handler

<!-- generated: handler-isf15acp4 -->
HAL handler for NKK ISF15ACP4 SmartDisplay OLED pushbutton (SSD1331 SPI).

- Header: [`handlers/isf15acp4/Isf15acp4Handler.h`](../../handlers/isf15acp4/Isf15acp4Handler.h)
- Switch: `HF_CORE_ENABLE_ISF15ACP4` (default OFF)
- Driver: [hf-isf15acp4-driver](https://github.com/hardfoc/hf-isf15acp4-driver)

```cpp
Isf15acp4Handler(
    BaseSpi& spi,
    BaseGpio& cs,
    BaseGpio& dc,
    BaseGpio& reset,
    const Isf15acp4HandlerConfig& config = {},
    BaseGpio* vcc_enable = nullptr,
    BaseGpio* switch_in = nullptr,
    bool switch_active_low = true);
```

| Group | Public methods |
| --- | --- |
| All | `EnsureInitialized()`, `EnsureDeinitialized()`, `Deinitialize()`, `IsInitialized()`, `DisplayOn()`, `DisplayOff()`, `FillScreen()`, `Present()`, `CreateGraphicsContext()`, `IsPressed()`, `visitDriver()`, `GetDriver()`, `GetDisplay()`, `GetErrorFlags()`, `ClearErrorFlags()`, `DumpDiagnostics()` |
<!-- /generated -->

