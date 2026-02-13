# Handlers vs External Drivers Compatibility Review

Date: 2026-02-12  
Branch: `cursor/handlers-drivers-compatibility-2013`

## Scope

This review evaluates:

1. How each handler maps to its underlying external driver contract.
2. How well handlers expose functionality for wider manager-level systems using `Base*` interfaces.
3. Build-time, runtime, and integration risks that impact portability and manager compatibility.

Handlers reviewed:

- `handlers/as5047u/As5047uHandler.*`
- `handlers/bno08x/Bno08xHandler.*`
- `handlers/pca9685/Pca9685Handler.*`
- `handlers/pcal95555/Pcal95555Handler.*`
- `handlers/ntc/NtcTemperatureHandler.*`
- `handlers/tmc9660/Tmc9660Handler.*`
- `handlers/tmc9660/Tmc9660AdcWrapper.*`

Base contracts reviewed:

- `BaseI2c`, `BaseSpi`, `BaseUart`, `BaseGpio`, `BasePwm`, `BaseAdc`, `BaseTemperature`, `BasePeriodicTimer`.

Driver contracts reviewed:

- `hf-as5047u-driver`
- `hf-bno08x-driver`
- `hf-pca9685-driver`
- `hf-pcal95555-driver`
- `hf-tmc9660-driver`

---

## Executive Summary

Overall design quality is strong in architecture patterns (bridge/CRTP/type erasure/wrappers), but compatibility is currently blocked by multiple critical issues:

- **AS5047U handler is not compatible with the current external driver API and include layout** (build-breaking).
- **NTC handler header/source are out of sync** (build-breaking if compiled).
- **PCAL95555 interrupt path performs heavy I2C + callback work in ISR context** (runtime hazard).
- **TMC9660 wrapper access can dereference null internals before `Initialize()`** (runtime crash hazard).
- **BNO08x freshness semantics are weakened** (stale data can be treated as valid by manager code).

Submodule initialization status is healthy (`git submodule status --recursive` reports populated tree).

---

## Findings (ordered by severity)

## CRITICAL

### 1) AS5047U handler is API-incompatible with current external driver

**Impact:** Build failure + unusable integration.

Evidence:

- Invalid include path:
  - `handlers/as5047u/As5047uHandler.h:39` includes `.../hf-as5047u-driver/src/AS5047U.hpp` (file does not exist in current submodule).
  - Current driver header is `hf-core-drivers/external/hf-as5047u-driver/inc/as5047u.hpp`.
- Handler expects old class model:
  - `handlers/as5047u/As5047uHandler.h:100` uses `AS5047U::spiBus`.
  - Current driver uses CRTP `as5047u::SpiInterface<Derived>` and `template <typename SpiType> class AS5047U`:
    - `hf-core-drivers/external/hf-as5047u-driver/inc/as5047u_spi_interface.hpp:39`
    - `hf-core-drivers/external/hf-as5047u-driver/inc/as5047u.hpp:78-79`
- Method naming mismatch (handler uses old lowercase API):
  - e.g. `getAngle/getVelocity/setZeroPosition/...` in `handlers/as5047u/As5047uHandler.cpp`
  - current driver exports `GetAngle/GetVelocity/SetZeroPosition/...`:
    - `hf-core-drivers/external/hf-as5047u-driver/inc/as5047u.hpp:121,138,204,...`
- Enum/string mismatch in diagnostics:
  - `handlers/as5047u/As5047uHandler.cpp:766` checks `As5047uError::COMMUNICATION_ERROR` (not defined in enum).
  - Frame-format names in diagnostics use `FRAME_16BIT` style:
    - `handlers/as5047u/As5047uHandler.cpp:783-785`
    - current driver uses `FrameFormat::SPI_16/SPI_24/SPI_32`:
      - `hf-core-drivers/external/hf-as5047u-driver/inc/as5047u_types.hpp:15-20`

Recommendation:

- Rebase handler on current driver API:
  - Use `as5047u::AS5047U<As5047uSpiAdapter>` and CRTP adapter.
  - Update all API calls to current names/casing.
  - Normalize frame-format enum usage.
  - Replace ad-hoc error string ternary with `As5047uErrorToString`.

---

### 2) NTC handler header/source divergence (ABI and symbol mismatch)

**Impact:** Build failure if this handler is part of target build.

Evidence:

- Constructor signature mismatch:
  - Header declares:
    - `NtcTemperatureHandler(ntc_type_t, BaseAdc*, const char*)` and
    - `NtcTemperatureHandler(const ntc_temp_handler_config_t&, BaseAdc*)`
    - `handlers/ntc/NtcTemperatureHandler.h:108,115`
  - Source defines:
    - `NtcTemperatureHandler(BaseAdc*, const ntc_temp_handler_config_t&)`
    - `handlers/ntc/NtcTemperatureHandler.cpp:26-27`
- Destructor definition conflict:
  - Header default-defines destructor inline:
    - `handlers/ntc/NtcTemperatureHandler.h:140`
  - Source defines destructor body again:
    - `handlers/ntc/NtcTemperatureHandler.cpp:69`
- Source references members not declared in header:
  - `continuous_callback_`, `continuous_user_data_`, `monitoring_timer_`, `monitoring_active_`, etc.
    - `handlers/ntc/NtcTemperatureHandler.cpp:34-37`
  - Header has different members (`monitoring_callback_`, `continuous_monitoring_active_`, etc.):
    - `handlers/ntc/NtcTemperatureHandler.h:337-340`
- Source defines methods not declared in header (and vice versa), e.g.:
  - `GetNtcReading`, `SetLastError`, `ContinuousMonitoringCallback`
    - `handlers/ntc/NtcTemperatureHandler.cpp:535,563,674`

Recommendation:

- Treat this as a merge-drift emergency:
  - Regenerate a single coherent API surface (header and source).
  - Reconcile member names/types and callback model.
  - Add compile gate in CI that explicitly builds this handler.

---

### 3) PCAL95555 interrupt processing does heavy work in ISR path

**Impact:** Potential hard faults, lockups, latency spikes, reentrancy issues.

Evidence:

- ISR callback states ISR context:
  - `handlers/pcal95555/Pcal95555Handler.cpp:498-502`
- ISR callback directly calls `ProcessInterrupts()`:
  - `handlers/pcal95555/Pcal95555Handler.cpp:501`
- `ProcessInterrupts()` performs:
  - I2C operations (`GetInterruptStatus`, `ReadAllInputs`):
    - `handlers/pcal95555/Pcal95555Handler.cpp:509,513`
  - User callback invocation:
    - `handlers/pcal95555/Pcal95555Handler.cpp:556-557`
- No handler-level lock in this path while mutating shared state (`prev_input_state_`, registry access).

Recommendation:

- Move ISR to deferred work model:
  - ISR only sets an atomic/event flag.
  - Worker task/thread drains hardware status via I2C and dispatches callbacks.
  - Protect shared state with `handler_mutex_` in worker context.

---

### 4) TMC9660 wrapper access can dereference null internals before initialization

**Impact:** Runtime crash (null dereference) in manager integration flows.

Evidence:

- Wrappers are only created in `Tmc9660Handler::Initialize()`:
  - `handlers/tmc9660/Tmc9660Handler.cpp:368-373`
- Accessors unconditionally dereference pointers:
  - `handlers/tmc9660/Tmc9660Handler.cpp:613-619`
- Delegation wrapper immediately uses `handler_.adc()`:
  - `handlers/tmc9660/Tmc9660AdcWrapper.cpp:31-37`

If manager constructs `Tmc9660AdcWrapper` before handler `Initialize()`, `handler_.adc()` dereferences `nullptr`.

Recommendation:

- Make accessor contract safe:
  - Option A: `adc()/temperature()/gpio()` return pointer/optional and guard null.
  - Option B: force-create wrappers in constructor.
  - Option C: lazy-create wrappers inside accessors before dereference.

---

### 5) PCA9685/PCAL95555 I2C adapters do not actually ensure BaseI2c init

**Impact:** Lazy-init contract becomes non-deterministic for manager code.

Evidence:

- PCA adapter returns true without calling BaseI2c init:
  - `handlers/pca9685/Pca9685Handler.cpp:68-71`
- PCAL adapter same behavior:
  - `handlers/pcal95555/Pcal95555Handler.cpp:67-70`
- External drivers explicitly rely on adapter `EnsureInitialized()` to init bus:
  - PCA driver:
    - `hf-core-drivers/external/hf-pca9685-driver/src/pca9685.ipp:45`
  - PCAL driver:
    - `hf-core-drivers/external/hf-pcal95555-driver/src/pcal95555.ipp:82`

Recommendation:

- Adapter `EnsureInitialized()` should call `i2c_device_.EnsureInitialized()`.
- Keep docs and behavior aligned (current docs claim delegation, implementation does not).

---

## HIGH

### 6) BNO08x handler treats cached/stale data as valid without freshness gating

**Impact:** Managers can consume stale snapshots as fresh sensor data.

Evidence:

- `readVectorSensor()` always marks output valid:
  - `handlers/bno08x/Bno08xHandler.cpp:405-415`
- `ReadImuData()` unconditionally sets `imu_data.valid = true`:
  - `handlers/bno08x/Bno08xHandler.cpp:482-510`
- External driver semantics:
  - New-data flag is cleared by `GetLatest()` and should be checked via `HasNewData()`:
    - `hf-core-drivers/external/hf-bno08x-driver/inc/bno08x.hpp:499-508,517-523`

Recommendation:

- Add freshness policy:
  - Check `HasNewData(sensor)` before promoting data to valid.
  - Optionally compare timestamps and reject unchanged snapshots.
  - Return `Bno08xError::DATA_NOT_AVAILABLE` when no fresh data.

---

### 7) BNO08x `ApplyConfiguration` does not disable sensors set false

**Impact:** Config drift and unwanted sensor traffic.

Evidence:

- `applyConfigLocked` helper only enables sensors when flag is true; no disable branch:
  - `handlers/bno08x/Bno08xHandler.cpp:640-647`

Recommendation:

- If flag is false, call `DisableSensor(sensor)` to enforce configuration as source of truth.

---

### 8) PCAL95555 interrupt capability/signaling is misleading on PCA9555

**Impact:** Manager may enable interrupt workflows that cannot work on detected chip.

Evidence:

- Pin wrapper reports interrupt support unconditionally:
  - `handlers/pcal95555/Pcal95555Handler.h:265-267`
- `RegisterPinInterrupt()` ignores driver enable result:
  - `handlers/pcal95555/Pcal95555Handler.cpp:449-451`
- Driver requires Agile I/O for interrupts:
  - `hf-core-drivers/external/hf-pcal95555-driver/src/pcal95555.ipp:690-696`

Recommendation:

- `SupportsInterrupts()` should reflect both hardware INT availability and chip variant support.
- Check and propagate `ConfigureInterrupt()` return status.

---

### 9) PCA9685 raw duty write ignores phase offset semantics

**Impact:** Wrong effective pulse width when phase shift is used.

Evidence:

- `SetDutyCycleRaw` uses `off_time = raw_value` directly:
  - `handlers/pca9685/Pca9685Handler.cpp:476-480`
- `SetDutyCycle` correctly uses `off_time = on_time + width`:
  - `handlers/pca9685/Pca9685Handler.cpp:457-460`

Recommendation:

- Align `SetDutyCycleRaw` with phase-aware math:
  - `off_time = (on_time_cache_[channel] + raw_value) & kMaxRawValue`.

---

## MEDIUM

### 10) TMC9660 `BaseAdc` channel model is sparse/non-uniform for generic managers

**Impact:** Generic manager assumptions about contiguous channel IDs and voltage semantics can break.

Evidence:

- Sparse ID scheme documented (`0-3`, `10-13`, `20-21`, `30-31`, `40-42`):
  - `handlers/tmc9660/Tmc9660Handler.h:1026-1033`
- Some channels return non-voltage physical values through `ReadChannelV`.
- Signed motor values cast to unsigned raw:
  - `handlers/tmc9660/Tmc9660Handler.cpp:970,976,982`

Recommendation:

- Provide either:
  - contiguous virtual channel mapping for manager-facing API, or
  - channel metadata query API so manager can interpret units/encoding safely.

---

### 11) TMC9660 handler explicitly non-thread-safe

**Impact:** Multi-threaded manager stacks must add external synchronization.

Evidence:

- Design note in docs:
  - `handlers/tmc9660/Tmc9660Handler.h:66-68`

Recommendation:

- Add optional internal lock mode or publish strict thread-safety contract in manager docs.

---

### 12) Platform coupling reduces portability of handler layer

**Impact:** Wider-system reuse is constrained.

Evidence:

- BNO08x comm adapters use FreeRTOS delay directly:
  - `handlers/bno08x/Bno08xHandler.cpp:104-106,216-218`
- NTC uses ESP timer APIs directly:
  - `handlers/ntc/NtcTemperatureHandler.cpp:350-371,394-395`
- TMC microsecond delay fallback assumes fixed 240 MHz:
  - `handlers/tmc9660/Tmc9660Handler.cpp:157`

Recommendation:

- Route timing/periodic work through abstract timer/thread services (`BasePeriodicTimer` or internal OS abstraction layer) for portability.

---

## Positive Patterns Worth Keeping

- `Pca9685Handler` and `Pcal95555Handler` expose `BasePwm`/`BaseGpio` wrappers that are manager-friendly.
- `Bno08xHandler` and `Tmc9660Handler` use type erasure/visitor patterns effectively to hide template complexity.
- `Tmc9660AdcWrapper` is a strong ownership adapter concept for manager integration (needs init-safety hardening).

---

## Prioritized Remediation Plan

1. **Build blockers first**
   - Rework `As5047uHandler` to current driver API.
   - Reconcile `NtcTemperatureHandler` header/source drift.
2. **Runtime safety second**
   - Defer PCAL interrupt processing out of ISR.
   - Harden TMC wrapper access (`adc()/temperature()/gpio()` null-safe contract).
3. **Manager compatibility third**
   - Enforce BNO freshness semantics and proper disable path in `ApplyConfiguration`.
   - Fix PCA raw duty phase handling.
   - Correct adapter lazy-init behavior for PCA/PCAL.
4. **Portability and consistency**
   - Remove direct platform timer/RTOS coupling from handlers where practical.
   - Add unit/integration tests for:
     - freshness semantics
     - interrupt registration and dispatch behavior
     - lazy initialization path
     - wrapper pre-init safety

---

## Suggested Acceptance Criteria for "Manager-Ready" Handlers

A handler should be considered manager-ready only if:

1. It compiles against the current external driver API.
2. It exposes a stable `Base*` surface (or a wrapper) for manager ownership.
3. Fresh/stale data semantics are explicit and test-covered.
4. Interrupt callbacks are safe with respect to ISR/task context.
5. Lazy initialization truly initializes underlying transport interfaces.
6. Thread-safety expectations are explicit and test-covered.

---

## Reanalysis Update (2026-02-12, sync check after requested "huge edits")

### Git sync/delta result

- Executed:
  - `git fetch origin cursor/handlers-drivers-compatibility-2013`
  - `git pull origin cursor/handlers-drivers-compatibility-2013`
- Branch status after sync: clean and up to date with remote.
- Diff against `origin/main...HEAD`: only this review document exists as branch delta.
  - No new handler code modifications were present on this branch at reanalysis time.

### Revalidated status of previously critical items

- **AS5047U API/include drift**: still present.
  - `handlers/as5047u/As5047uHandler.h:39,100`
  - `handlers/as5047u/As5047uHandler.cpp:165,766,783`
- **NTC header/source divergence**: still present.
  - `handlers/ntc/NtcTemperatureHandler.h:140`
  - `handlers/ntc/NtcTemperatureHandler.cpp:26,69,34,36,535,674`
- **PCAL95555 ISR-heavy interrupt path**: still present.
  - `handlers/pcal95555/Pcal95555Handler.cpp:494-513,556`
- **TMC wrapper pre-init null dereference risk**: still present.
  - `handlers/tmc9660/Tmc9660Handler.cpp:371-372,613-619`
  - `handlers/tmc9660/Tmc9660AdcWrapper.cpp:31-37`
- **BNO freshness/config semantics gaps**: still present.
  - `handlers/bno08x/Bno08xHandler.cpp:405-415,488-510,636-647`
- **PCA/PCAL adapter `EnsureInitialized()` no-op**: still present.
  - `handlers/pca9685/Pca9685Handler.cpp:68-71`
  - `handlers/pcal95555/Pcal95555Handler.cpp:67-70`

### Practical interpretation

No new code edits were available to re-score in this branch, so this reanalysis confirms that the original findings remain valid and unresolved.

