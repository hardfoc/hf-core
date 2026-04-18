/**
 * @file PlatformMutexBackend.h
 * @ingroup core
 * @brief RTOS mutex backend -- bridges PlatformMutex (interface-wrap) with
 *        OsAbstraction (rtos-wrap) at the core/ layer.
 *
 * This file is the compile-time injection point for PlatformMutex.h.
 * It lives in handlers/common/ which is already on the core/ include path,
 * so PlatformMutex.h discovers it via __has_include("PlatformMutexBackend.h").
 *
 * === Architecture ===
 *
 *   hf-internal-interface-wrap/inc/utils/PlatformMutex.h
 *     ^ self-contained, defines NullMutexBackend (no-op default)
 *     ^ uses __has_include("PlatformMutexBackend.h") to find this file
 *
 *   hf-utils-rtos-wrap/include/OsAbstraction.h
 *     ^ self-contained RTOS abstraction (FreeRTOS / NONE)
 *
 *   handlers/common/PlatformMutexBackend.h  <- THIS FILE
 *     -> Bridges the two independent modules at the core/ layer
 *     -> Defines FreeRtosMutexBackend when HF_RTOS_FREERTOS is active
 *     -> Falls through to NullMutexBackend when RTOS=NONE
 *
 * @author Nebiyu Tadesse
 * @date 2025
 * @copyright HardFOC
 */

#pragma once

#include "OsAbstraction.h"

//==============================================================================
// FreeRTOS Mutex Backend -- only active when a real RTOS is configured
//==============================================================================

#if defined(HF_RTOS_FREERTOS)

/**
 * @brief Concrete mutex backend that delegates to the OsAbstraction RTOS layer.
 *
 * All methods are static inline -- the compiler inlines them into PlatformMutex
 * operations with zero call overhead. Handle types (OS_Mutex = SemaphoreHandle_t)
 * are pointer-sized.
 *
 * The backend API uses pointer parameters (Handle*) so the OsAbstraction
 * functions can read/write handles in-place without intermediate copies.
 */
struct FreeRtosMutexBackend {
    // -- Handle Types ------------------------------------------------------
    using RecursiveMutexHandle = OS_Mutex;
    using MutexHandle          = OS_Mutex;

    // -- Constants ---------------------------------------------------------
    static constexpr uint32_t MAX_DELAY    = static_cast<uint32_t>(OS_WAIT_FOREVER);
    static constexpr uint32_t TICK_RATE_HZ = configTICK_RATE_HZ;

    // -- Recursive Mutex Operations ----------------------------------------

    static inline void createRecursive(RecursiveMutexHandle* h) noexcept {
        os_recursive_mutex_create(h, "PlatMtx");
    }

    static inline void destroyRecursive(RecursiveMutexHandle* h) noexcept {
        if (h && *h) {
            os_recursive_mutex_delete(h);
            *h = nullptr;
        }
    }

    static inline bool lockRecursive(RecursiveMutexHandle* h, uint32_t timeout_ticks) noexcept {
        return os_recursive_mutex_get(h, static_cast<OS_Ulong>(timeout_ticks)) == OS_SUCCESS;
    }

    static inline bool tryLockRecursive(RecursiveMutexHandle* h) noexcept {
        return os_recursive_mutex_get(h, OS_NO_WAIT) == OS_SUCCESS;
    }

    static inline void unlockRecursive(RecursiveMutexHandle* h) noexcept {
        os_recursive_mutex_put(h);
    }

    // -- Regular Mutex Operations ------------------------------------------

    static inline void createMutex(MutexHandle* h) noexcept {
        os_mutex_create(h, "PlatShrd", OS_INHERIT);
    }

    static inline void destroyMutex(MutexHandle* h) noexcept {
        if (h && *h) {
            os_mutex_delete(h);
            *h = nullptr;
        }
    }

    static inline bool lockMutex(MutexHandle* h, uint32_t timeout_ticks) noexcept {
        return os_mutex_get(h, static_cast<OS_Ulong>(timeout_ticks)) == OS_SUCCESS;
    }

    static inline bool tryLockMutex(MutexHandle* h) noexcept {
        return os_mutex_get(h, OS_NO_WAIT) == OS_SUCCESS;
    }

    static inline void unlockMutex(MutexHandle* h) noexcept {
        os_mutex_put(h);
    }

    // -- Time / Scheduling -------------------------------------------------

    static inline uint32_t getTickCount() noexcept {
        return static_cast<uint32_t>(os_time_get());
    }

    static inline uint32_t msToTicks(uint32_t ms) noexcept {
        return static_cast<uint32_t>(pdMS_TO_TICKS(ms));
    }

    static inline void yield() noexcept {
        taskYIELD();
    }
};

/// Select FreeRtosMutexBackend as the active backend for PlatformMutex
using PlatformMutexActiveBackend = FreeRtosMutexBackend;

/// Signal to PlatformMutex.h that a real backend is configured
#define PLATFORM_MUTEX_BACKEND_CONFIGURED

#endif // HF_RTOS_FREERTOS

//==============================================================================
// HF_RTOS_NONE path
//==============================================================================
// When RTOS=NONE, this header is included (it's on the include path) but
// nothing is defined -- no PlatformMutexActiveBackend, no PLATFORM_MUTEX_BACKEND_CONFIGURED.
// PlatformMutex.h falls back to its built-in NullMutexBackend with zero overhead.
// This is the correct behavior: users who build without an RTOS get mutex no-ops.
//==============================================================================
