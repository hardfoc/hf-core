/**
 * @file CO_driver_target.h
 * @brief CANopenNode v4 target definitions for the generic HardFOC driver
 *        (CO_driver_hardfoc.c).
 *
 * @details This directory is on the include path instead of CANopenNode's
 *          `example/`, so the stack picks this file up. It provides:
 *          - `CO_CAN_MAX_LEN`: payload size of the RX/TX buffers, 8 on classic
 *            builds and 64 with `HF_CO_CAN_FD=1` (nothing above the port
 *            binding assumes 8 bytes);
 *          - the RX/TX buffer and CANmodule types;
 *          - the CO locks: one recursive mutex per CANmodule (FreeRTOS on
 *            target, pthreads on a POSIX host, nothing on a bare build);
 *          - the application's stack configuration: `hf_co_app_config.h`
 *            is included first when the application puts one on the
 *            include path (`HF_CORE_CANOPENNODE_CONFIG_DIR`), so its
 *            `CO_CONFIG_*` values win over the module defaults.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#ifndef CO_DRIVER_TARGET_H
#define CO_DRIVER_TARGET_H

#if defined(__has_include)
#if __has_include("hf_co_app_config.h")
#include "hf_co_app_config.h"
#endif
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "hf_co_driver.h"

#if defined(HF_CO_DRIVER_FREERTOS) && HF_CO_DRIVER_FREERTOS
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#define HF_CO_LOCK_FREERTOS 1
#elif defined(__unix__) || defined(__APPLE__)
#include <pthread.h>
#define HF_CO_LOCK_PTHREAD 1
#endif

#ifdef CO_DRIVER_CUSTOM
#include "CO_driver_custom.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define CO_LITTLE_ENDIAN
#define CO_SWAP_16(x) (x)
#define CO_SWAP_32(x) (x)
#define CO_SWAP_64(x) (x)

typedef uint_fast8_t bool_t;
typedef float float32_t;
typedef double float64_t;

/** Payload bytes of every CANopenNode RX/TX buffer. */
#define CO_CAN_MAX_LEN HF_CO_CAN_MAX_LEN

/** Received message as CANopenNode RX callbacks see it (read through the macros below). */
typedef struct {
  uint16_t ident;               /**< 11-bit identifier, bit 11 = RTR */
  uint8_t dlc;                  /**< payload length in bytes */
  uint8_t data[CO_CAN_MAX_LEN];
} CO_CANrxMsg_t;

#define CO_CANrxMsg_readIdent(msg) ((uint16_t)(((const CO_CANrxMsg_t*)(msg))->ident & 0x07FFU))
#define CO_CANrxMsg_readDLC(msg) (((const CO_CANrxMsg_t*)(msg))->dlc)
#define CO_CANrxMsg_readData(msg) (((const CO_CANrxMsg_t*)(msg))->data)

/** RX buffer: one CANopenNode consumer. */
typedef struct {
  uint16_t ident; /**< 11-bit identifier, bit 11 = RTR */
  uint16_t mask;  /**< bits that must match */
  void* object;   /**< consumer object (handed to the RX policy) */
  void (*CANrx_callback)(void* object, void* message);
} CO_CANrx_t;

/** TX buffer: one CANopenNode producer. */
typedef struct {
  uint32_t ident;                /**< 11-bit identifier, bit 11 = RTR */
  uint8_t DLC;                   /**< payload length in bytes */
  uint8_t data[CO_CAN_MAX_LEN];
  volatile bool_t bufferFull;    /**< pending on at least one port */
  volatile bool_t syncFlag;      /**< synchronous TPDO */
  uint8_t pendingPorts;          /**< ports this frame still has to go out on */
} CO_CANtx_t;

/** CAN module: the CANopenNode side of one hf_co_driver_t. */
typedef struct CO_CANmodule_s {
  void* CANptr;                  /**< hf_co_driver_t* */
  CO_CANrx_t* rxArray;
  uint16_t rxSize;
  CO_CANtx_t* txArray;
  uint16_t txSize;
  uint16_t CANerrorStatus;
  volatile bool_t CANnormal;
  volatile bool_t useCANrxFilters;
  volatile bool_t bufferInhibitFlag;
  volatile bool_t firstCANtxMessage;
  volatile uint16_t CANtxCount;
  uint32_t errOld;
  bool_t lockReady;              /**< lock created (once, on the first CO_CANmodule_init) */
#if defined(HF_CO_LOCK_FREERTOS)
  SemaphoreHandle_t lock;
  StaticSemaphore_t lockStorage;
#elif defined(HF_CO_LOCK_PTHREAD)
  pthread_mutex_t lock;
#endif
} CO_CANmodule_t;

/** Storage entry (CO_storage.h). Filled by the application's storage backend. */
typedef struct {
  void* addr;
  size_t len;
  uint8_t subIndexOD;
  uint8_t attr;
  void* addrNV;
} CO_storage_entry_t;

/** Take / release the CANmodule lock (recursive; never held across a blocking call). */
void hf_co_lock(CO_CANmodule_t* CANmodule);
void hf_co_unlock(CO_CANmodule_t* CANmodule);

#define CO_LOCK_CAN_SEND(CAN_MODULE) hf_co_lock(CAN_MODULE)
#define CO_UNLOCK_CAN_SEND(CAN_MODULE) hf_co_unlock(CAN_MODULE)
#define CO_LOCK_EMCY(CAN_MODULE) hf_co_lock(CAN_MODULE)
#define CO_UNLOCK_EMCY(CAN_MODULE) hf_co_unlock(CAN_MODULE)
#define CO_LOCK_OD(CAN_MODULE) hf_co_lock(CAN_MODULE)
#define CO_UNLOCK_OD(CAN_MODULE) hf_co_unlock(CAN_MODULE)

/* RX callbacks run in thread context (hf_co_driver_rx_drain); the flag is
 * read by the thread that processes the object, possibly another one. A full
 * barrier keeps the data written before the flag. */
#define CO_MemoryBarrier() __sync_synchronize()
#define CO_FLAG_READ(rxNew) ((rxNew) != NULL)
#define CO_FLAG_SET(rxNew)                                                                                             \
  do {                                                                                                                 \
    CO_MemoryBarrier();                                                                                                \
    (rxNew) = (void*)1L;                                                                                               \
  } while (0)
#define CO_FLAG_CLEAR(rxNew)                                                                                           \
  do {                                                                                                                 \
    CO_MemoryBarrier();                                                                                                \
    (rxNew) = NULL;                                                                                                    \
  } while (0)

#ifdef __cplusplus
}
#endif

#endif /* CO_DRIVER_TARGET_H */
