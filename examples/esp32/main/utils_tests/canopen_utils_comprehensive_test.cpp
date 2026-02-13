/**
 * @file canopen_utils_comprehensive_test.cpp
 * @brief Comprehensive test suite for hf-utils-canopen utilities
 *
 * Tests: CanFrame construction, SDO protocol helpers, NMT state machine,
 * COB-ID calculation, and frame data encoding/decoding.
 *
 * @note This is a software-only test suite that validates CAN frame structures
 * and protocol utilities without requiring actual CAN hardware.
 *
 * @author HardFOC Team
 * @date 2025-2026
 * @copyright GPL-3.0-or-later
 */

#include "TestFramework.h"

#include "CanFrame.h"
#include "CanOpenUtils.h"

#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#ifdef __cplusplus
}
#endif

static const char* TAG = "CANopen_Utils_Test";
static TestResults g_test_results;

static constexpr bool ENABLE_CAN_FRAME_TESTS = true;
static constexpr bool ENABLE_SDO_TESTS       = true;
static constexpr bool ENABLE_NMT_TESTS       = true;


// ─────────────────────── CanFrame ───────────────────────

static bool test_can_frame_default() noexcept {
    CanOpen::CanFrame frame{};
    bool id_zero = (frame.id == 0);
    bool dlc_zero = (frame.dlc == 0);
    bool not_extended = !frame.extended;
    bool not_rtr = !frame.rtr;
    ESP_LOGI(TAG, "CanFrame default: id=%lu, dlc=%d, ext=%d, rtr=%d",
             frame.id, frame.dlc, frame.extended, frame.rtr);
    return id_zero && dlc_zero && not_extended && not_rtr;
}

static bool test_can_frame_data() noexcept {
    CanOpen::CanFrame frame{};
    frame.id = 0x601;
    frame.dlc = 8;
    frame.data[0] = 0x2F; // SDO download initiate (1 byte)
    frame.data[1] = 0x17; // Index low
    frame.data[2] = 0x10; // Index high
    frame.data[3] = 0x00; // Sub-index

    bool id_ok = (frame.id == 0x601);
    bool dlc_ok = (frame.dlc == 8);
    bool data_ok = (frame.data[0] == 0x2F);
    ESP_LOGI(TAG, "CanFrame data: id=0x%lX, dlc=%d, data[0]=0x%02X",
             frame.id, frame.dlc, frame.data[0]);
    return id_ok && dlc_ok && data_ok;
}

// ─────────────────────── SDO Protocol ───────────────────────

static bool test_sdo_download_frame() noexcept {
    // Build an SDO expedited download frame
    CanOpen::CanFrame frame = CanOpen::BuildSdoDownload(
        0x01,     // node_id
        0x1017,   // index (producer heartbeat time)
        0x00,     // sub-index
        1000,     // value (1000ms)
        2         // data_size (2 bytes)
    );

    bool id_ok = (frame.id == 0x601);
    bool dlc_ok = (frame.dlc == 8);
    bool cmd_ok = ((frame.data[0] & 0xE0) == 0x20); // Download initiate
    ESP_LOGI(TAG, "SDO download: id=0x%lX, cmd=0x%02X", frame.id, frame.data[0]);
    return id_ok && dlc_ok && cmd_ok;
}

// ─────────────────────── NMT Protocol ───────────────────────

static bool test_nmt_commands() noexcept {
    // Build NMT start remote node
    CanOpen::CanFrame frame = CanOpen::BuildNmt(
        0x01, // node_id
        CanOpen::NmtCommand::StartNode
    );

    bool id_zero = (frame.id == 0x000); // NMT always COB-ID 0
    bool dlc_two = (frame.dlc == 2);
    bool cmd_ok = (frame.data[0] == static_cast<uint8_t>(CanOpen::NmtCommand::StartNode));
    bool node_ok = (frame.data[1] == 0x01);

    ESP_LOGI(TAG, "NMT start: id=0x%lX, dlc=%d, cmd=0x%02X, node=%d",
             frame.id, frame.dlc, frame.data[0], frame.data[1]);
    return id_zero && dlc_two && cmd_ok && node_ok;
}

// ═══════════════════════ ENTRY POINT ═══════════════════════

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "\n");
    ESP_LOGI(TAG, "╔══════════════════════════════════════════════════════════════╗");
    ESP_LOGI(TAG, "║     CANopen UTILITIES COMPREHENSIVE TEST SUITE              ║");
    ESP_LOGI(TAG, "╚══════════════════════════════════════════════════════════════╝");

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_CAN_FRAME_TESTS, "CAN FRAME",
        RUN_TEST("default", test_can_frame_default); flip_test_progress_indicator();
        RUN_TEST("data", test_can_frame_data); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_SDO_TESTS, "SDO PROTOCOL",
        RUN_TEST("download", test_sdo_download_frame); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_NMT_TESTS, "NMT PROTOCOL",
        RUN_TEST("commands", test_nmt_commands); flip_test_progress_indicator();
    );

    print_test_summary(g_test_results, "CANopen UTILITIES COMPREHENSIVE", TAG);
    while (true) { vTaskDelay(pdMS_TO_TICKS(10000)); }
}
