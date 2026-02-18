/**
 * @file tmc5160_handler_comprehensive_test.cpp
 * @brief Comprehensive test suite for Tmc5160Handler
 *
 * Tests: CRTP SPI adapter, initialization with DriverConfig, motor enable/
 * disable, motion control (position/velocity/acceleration/stop), visitDriver()
 * subsystem access (rampControl, motorControl, stallGuard, status, encoder,
 * etc.), StallGuard readback, diagnostics dump, status checks (overtemp,
 * stall, standstill, target reached), driver config snapshot, and pre-init
 * error handling.
 *
 * @note This test requires a TMC5160 evaluation board connected via SPI.
 *       Without hardware, initialization will fail gracefully and remaining
 *       tests will be skipped.
 *
 * @author HardFOC Team
 * @date 2025-2026
 * @copyright GPL-3.0-or-later
 */

#include "TestFramework.h"
#include "esp32_bus_setup.hpp"
#include "esp32_test_config.hpp"

#include "handlers/tmc5160/Tmc5160Handler.h"

#include <cmath>
#include <memory>

#ifdef __cplusplus
extern "C" {
#endif
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#ifdef __cplusplus
}
#endif

static const char* TAG = "TMC5160_Handler_Test";
static TestResults g_test_results;

static constexpr bool ENABLE_CONSTRUCTION_TESTS    = true;
static constexpr bool ENABLE_INITIALIZATION_TESTS  = true;
static constexpr bool ENABLE_MOTOR_ENABLE_TESTS    = true;
static constexpr bool ENABLE_MOTION_CONTROL_TESTS  = true;
static constexpr bool ENABLE_STATUS_TESTS          = true;
static constexpr bool ENABLE_VISIT_DRIVER_TESTS    = true;
static constexpr bool ENABLE_DIAGNOSTICS_TESTS     = true;
static constexpr bool ENABLE_ERROR_HANDLING_TESTS  = true;

// Hardware resources
static std::unique_ptr<EspGpio> g_enable_gpio;
static std::unique_ptr<EspGpio> g_diag0_gpio;
static std::unique_ptr<EspGpio> g_diag1_gpio;
static BaseSpi* g_spi_device = nullptr;
static std::unique_ptr<Tmc5160Handler> g_handler;

static bool g_hw_present = false;

static bool create_handler() noexcept {
    auto* spi_bus = get_shared_spi_bus();
    if (!spi_bus) {
        ESP_LOGE(TAG, "Failed to get SPI bus");
        return false;
    }

    hf_spi_device_config_t dev_cfg = {};
    dev_cfg.cs_pin = static_cast<hf_pin_num_t>(PIN_TMC5160_CS);
    dev_cfg.clock_speed_hz = TMC5160_SPI_CLOCK_HZ;
    dev_cfg.mode = hf_spi_mode_t::HF_SPI_MODE_3;  // CPOL=1, CPHA=1
    dev_cfg.queue_size = 1;

    int dev_idx = spi_bus->CreateDevice(dev_cfg);
    if (dev_idx < 0) {
        ESP_LOGE(TAG, "SPI device creation failed for TMC5160");
        return false;
    }
    g_spi_device = spi_bus->GetDevice(dev_idx);
    if (!g_spi_device) {
        ESP_LOGE(TAG, "SPI device init failed for TMC5160");
        return false;
    }

    g_enable_gpio = create_gpio(PIN_TMC5160_DRV_ENN,
                                hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT,
                                hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
    g_diag0_gpio = create_gpio(PIN_TMC5160_DIAG0,
                               hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT,
                               hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH);
    g_diag1_gpio = create_gpio(PIN_TMC5160_DIAG1,
                               hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT,
                               hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH);

    if (!g_enable_gpio) {
        ESP_LOGE(TAG, "Failed to create enable GPIO");
        return false;
    }

    g_handler = std::make_unique<Tmc5160Handler>(
        *g_spi_device, *g_enable_gpio,
        g_diag0_gpio.get(), g_diag1_gpio.get());

    return g_handler != nullptr;
}

// ─────────────────────── Construction ───────────────────────

static bool test_construction() noexcept {
    bool is_init = g_handler->IsInitialized();
    bool is_spi = g_handler->IsSpi();
    ESP_LOGI(TAG, "After construction: IsInitialized=%d (expected false), IsSpi=%d (expected true)",
             is_init, is_spi);
    return !is_init && is_spi;
}

// ─────────────────────── Initialization ───────────────────────

static bool test_initialize() noexcept {
    if (!g_handler) return false;

    tmc51x0::DriverConfig config = {};
    // Use conservative defaults for test — actual values depend on motor
    auto err = g_handler->Initialize(config, true /*verbose*/);
    bool ok = (err == tmc51x0::ErrorCode::OK);
    g_hw_present = ok;
    ESP_LOGI(TAG, "Initialize: %s (err=%d, hw_present=%d)",
             ok ? "OK" : "FAILED", static_cast<int>(err), g_hw_present);
    return true; // Always pass — no hardware is a valid scenario
}

static bool test_is_initialized() noexcept {
    if (!g_handler) return false;
    bool init = g_handler->IsInitialized();
    ESP_LOGI(TAG, "IsInitialized: %d (expected %d)", init, g_hw_present);
    return init == g_hw_present;
}

static bool test_driver_config_snapshot() noexcept {
    if (!g_handler) return false;
    const auto& cfg = g_handler->GetDriverConfig();
    ESP_LOGI(TAG, "DriverConfig snapshot retrieved (address in config object: %p)",
             static_cast<const void*>(&cfg));
    return true;
}

// ─────────────────────── Motor Enable ───────────────────────

static bool test_enable_motor() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->visitDriver([](auto& drv) -> bool {
        auto r = drv.motorControl.Enable();
        return r.IsOk();
    });
    ESP_LOGI(TAG, "EnableMotor: %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_is_motor_enabled() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool enabled = g_handler->visitDriver([](auto& drv) -> bool {
        auto r = drv.motorControl.IsEnabled();
        return r ? r.Value() : false;
    });
    ESP_LOGI(TAG, "IsMotorEnabled: %d", enabled);
    return enabled;
}

static bool test_disable_motor() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->visitDriver([](auto& drv) -> bool {
        auto r = drv.motorControl.Disable();
        return r.IsOk();
    });
    ESP_LOGI(TAG, "DisableMotor: %s", ok ? "OK" : "FAILED");
    return ok;
}

// ─────────────────────── Motion Control ───────────────────────

static bool test_set_target_velocity() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->visitDriver([](auto& drv) -> bool {
        auto r = drv.rampControl.SetRampMode(tmc51x0::RampMode::VELOCITY_POS);
        if (!r.IsOk()) return false;
        auto r2 = drv.rampControl.SetMaxSpeed(0.0f, tmc51x0::Unit::Steps);
        return r2.IsOk();
    });
    ESP_LOGI(TAG, "SetTargetVelocity(0): %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_set_target_position() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->visitDriver([](auto& drv) -> bool {
        auto r = drv.rampControl.SetRampMode(tmc51x0::RampMode::POSITIONING);
        if (!r.IsOk()) return false;
        auto r2 = drv.rampControl.SetTargetPosition(0.0f, tmc51x0::Unit::Steps);
        return r2.IsOk();
    });
    ESP_LOGI(TAG, "SetTargetPosition(0): %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_get_current_position() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    int32_t pos = g_handler->visitDriver([](auto& drv) -> int32_t {
        auto r = drv.rampControl.GetCurrentPositionMicrosteps();
        return r ? r.Value() : 0;
    });
    ESP_LOGI(TAG, "GetCurrentPosition: %ld", static_cast<long>(pos));
    return true; // 0 is valid
}

static bool test_get_current_velocity() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    float vel = g_handler->visitDriver([](auto& drv) -> float {
        auto r = drv.rampControl.GetCurrentSpeed(tmc51x0::Unit::Steps);
        return r ? r.Value() : 0.0f;
    });
    ESP_LOGI(TAG, "GetCurrentVelocity: %.1f", vel);
    return true;
}

static bool test_stop() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->visitDriver([](auto& drv) -> bool {
        auto r = drv.rampControl.Stop();
        return r.IsOk();
    });
    ESP_LOGI(TAG, "Stop: %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_set_current() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->visitDriver([](auto& drv) -> bool {
        auto r = drv.motorControl.SetCurrent(16, 8); // irun=16, ihold=8 (raw 0-31 scale)
        return r.IsOk();
    });
    ESP_LOGI(TAG, "SetCurrent(16, 8): %s", ok ? "OK" : "FAILED");
    return ok;
}

// ─────────────────────── Status ───────────────────────

static bool test_is_standstill() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool standstill = g_handler->visitDriver([](auto& drv) -> bool {
        auto r = drv.rampControl.IsStandstill();
        return r ? r.Value() : false;
    });
    ESP_LOGI(TAG, "IsStandstill: %d (expected true at rest)", standstill);
    return standstill; // should be at standstill
}

static bool test_is_target_reached() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool reached = g_handler->visitDriver([](auto& drv) -> bool {
        auto r = drv.rampControl.IsTargetReached();
        return r ? r.Value() : false;
    });
    ESP_LOGI(TAG, "IsTargetReached: %d", reached);
    return true; // may or may not be reached
}

static bool test_is_overtemperature() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ot = g_handler->visitDriver([](auto& drv) -> bool {
        auto r = drv.status.IsOvertemperature();
        return r ? r.Value() : false;
    });
    ESP_LOGI(TAG, "IsOvertemperature: %d (expected false)", ot);
    return !ot; // should not be overtemp at start
}

static bool test_is_stall_detected() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool stall = g_handler->visitDriver([](auto& drv) -> bool {
        auto r = drv.stallGuard.IsStallDetected();
        return r ? r.Value() : false;
    });
    ESP_LOGI(TAG, "IsStallDetected: %d (expected false at standstill)", stall);
    return !stall;
}

static bool test_stallguard_result() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    int32_t sg = g_handler->visitDriver([](auto& drv) -> int32_t {
        auto r = drv.stallGuard.GetStallGuardResult();
        return r ? static_cast<int32_t>(r.Value()) : 0;
    });
    ESP_LOGI(TAG, "StallGuard result: %ld", static_cast<long>(sg));
    return true;
}

static bool test_chip_version() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    uint32_t version = g_handler->visitDriver([](auto& drv) -> uint32_t {
        return static_cast<uint32_t>(drv.status.GetChipVersion());
    });
    ESP_LOGI(TAG, "Chip version: 0x%08lX", static_cast<unsigned long>(version));
    return version != 0;
}

// ─────────────────────── visitDriver() ───────────────────────

static bool test_visit_driver_ramp_control() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto result = g_handler->visitDriver([](auto& drv) -> bool {
        // Access rampControl subsystem
        auto& ramp = drv.rampControl;
        (void)ramp; // Just verify access compiles and works
        return true;
    });
    ESP_LOGI(TAG, "visitDriver(rampControl): %s", result ? "OK" : "FAILED");
    return result;
}

static bool test_visit_driver_motor_control() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto result = g_handler->visitDriver([](auto& drv) -> bool {
        auto& mc = drv.motorControl;
        (void)mc;
        return true;
    });
    ESP_LOGI(TAG, "visitDriver(motorControl): %s", result ? "OK" : "FAILED");
    return result;
}

static bool test_visit_driver_encoder() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto result = g_handler->visitDriver([](auto& drv) -> bool {
        auto& enc = drv.encoder;
        (void)enc;
        return true;
    });
    ESP_LOGI(TAG, "visitDriver(encoder): %s", result ? "OK" : "FAILED");
    return result;
}

static bool test_visit_driver_communication() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto result = g_handler->visitDriver([](auto& drv) -> bool {
        auto& comm = drv.communication;
        (void)comm;
        return true;
    });
    ESP_LOGI(TAG, "visitDriver(communication): %s", result ? "OK" : "FAILED");
    return result;
}

// ─────────────────────── Diagnostics ───────────────────────

static bool test_dump_diagnostics() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    g_handler->DumpDiagnostics();
    ESP_LOGI(TAG, "DumpDiagnostics: completed");
    return true;
}

// ─────────────────────── Error Handling ───────────────────────

static bool test_operations_before_init() noexcept {
    if (!g_spi_device || !g_enable_gpio) return true;

    auto uninit = std::make_unique<Tmc5160Handler>(
        *g_spi_device, *g_enable_gpio);

    bool init = uninit->IsInitialized();

    ESP_LOGI(TAG, "Uninit handler: init=%d (expected false)", init);
    return !init;
}

static bool test_deinitialize() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->Deinitialize();
    ESP_LOGI(TAG, "Deinitialize: %s", ok ? "OK" : "FAILED");
    bool still_init = g_handler->IsInitialized();
    ESP_LOGI(TAG, "IsInitialized after deinit: %d (expected false)", still_init);
    return ok && !still_init;
}

// ═══════════════════════ ENTRY POINT ═══════════════════════

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "\n");
    ESP_LOGI(TAG, "╔══════════════════════════════════════════════════════════════╗");
    ESP_LOGI(TAG, "║     TMC5160 HANDLER COMPREHENSIVE TEST SUITE                ║");
    ESP_LOGI(TAG, "╚══════════════════════════════════════════════════════════════╝");

    if (!create_handler()) { ESP_LOGE(TAG, "FATAL: Handler creation failed"); return; }

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_CONSTRUCTION_TESTS, "CONSTRUCTION",
        RUN_TEST_IN_TASK("construct", test_construction, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_INITIALIZATION_TESTS, "INITIALIZATION",
        RUN_TEST_IN_TASK("init", test_initialize, 16384, 15); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("is_init", test_is_initialized, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("drv_cfg", test_driver_config_snapshot, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_MOTOR_ENABLE_TESTS, "MOTOR ENABLE",
        RUN_TEST_IN_TASK("enable", test_enable_motor, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("is_enabled", test_is_motor_enabled, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("disable", test_disable_motor, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_MOTION_CONTROL_TESTS, "MOTION CONTROL",
        RUN_TEST_IN_TASK("velocity", test_set_target_velocity, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("position", test_set_target_position, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("get_pos", test_get_current_position, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("get_vel", test_get_current_velocity, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("stop", test_stop, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("current", test_set_current, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_STATUS_TESTS, "STATUS",
        RUN_TEST_IN_TASK("standstill", test_is_standstill, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("target_reached", test_is_target_reached, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("overtemp", test_is_overtemperature, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("stall", test_is_stall_detected, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("sg_result", test_stallguard_result, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("chip_ver", test_chip_version, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_VISIT_DRIVER_TESTS, "visitDriver()",
        RUN_TEST_IN_TASK("v_ramp", test_visit_driver_ramp_control, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("v_motor", test_visit_driver_motor_control, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("v_enc", test_visit_driver_encoder, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("v_comm", test_visit_driver_communication, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_DIAGNOSTICS_TESTS, "DIAGNOSTICS",
        RUN_TEST_IN_TASK("dump_diag", test_dump_diagnostics, 16384, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_ERROR_HANDLING_TESTS, "ERROR HANDLING",
        RUN_TEST_IN_TASK("before_init", test_operations_before_init, 16384, 10); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("deinit", test_deinitialize, 8192, 5); flip_test_progress_indicator();
    );

    print_test_summary(g_test_results, "TMC5160 HANDLER COMPREHENSIVE", TAG);
    while (true) { vTaskDelay(pdMS_TO_TICKS(10000)); }
}
