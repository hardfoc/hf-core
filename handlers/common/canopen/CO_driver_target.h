/*
 * HardFOC Vortex — CANopenNode CO_driver_target.h (shadows CANopenNode example).
 * Provides real CO_CANrxMsg_* macros and FreeRTOS-backed critical sections.
 *
 * Include path: this directory is listed before CANopenNode/example so CANopen
 * picks this file instead of the blank template.
 */
#ifndef CO_DRIVER_TARGET_H
#define CO_DRIVER_TARGET_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"

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

/** Wire-format RX message passed to CANopenNode rx callbacks (stack reads via macros). */
typedef struct {
  uint16_t ident; /**< 11-bit ID, no RTR bit in ident here */
  uint8_t dlc;
  uint8_t data[8];
} CO_CANrxMsg_t;

#define CO_CANrxMsg_readIdent(msg) (((const CO_CANrxMsg_t*)(msg))->ident)
#define CO_CANrxMsg_readDLC(msg) (((const CO_CANrxMsg_t*)(msg))->dlc)
#define CO_CANrxMsg_readData(msg) (((const CO_CANrxMsg_t*)(msg))->data)

typedef struct {
  uint16_t ident;
  uint16_t mask;
  void* object;
  void (*CANrx_callback)(void* object, void* message);
} CO_CANrx_t;

typedef struct {
  uint32_t ident;
  uint8_t DLC;
  uint8_t data[8];
  volatile bool_t bufferFull;
  volatile bool_t syncFlag;
} CO_CANtx_t;

typedef struct {
  void* CANptr;
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
} CO_CANmodule_t;

typedef struct {
  void* addr;
  size_t len;
  uint8_t subIndexOD;
  uint8_t attr;
  void* addrNV;
} CO_storage_entry_t;

void hf_co_driver_lock(void);
void hf_co_driver_unlock(void);

#define CO_LOCK_CAN_SEND(CAN_MODULE) ((void)(CAN_MODULE), hf_co_driver_lock())
#define CO_UNLOCK_CAN_SEND(CAN_MODULE) ((void)(CAN_MODULE), hf_co_driver_unlock())
#define CO_LOCK_EMCY(CAN_MODULE) ((void)(CAN_MODULE), hf_co_driver_lock())
#define CO_UNLOCK_EMCY(CAN_MODULE) ((void)(CAN_MODULE), hf_co_driver_unlock())
#define CO_LOCK_OD(CAN_MODULE) ((void)(CAN_MODULE), hf_co_driver_lock())
#define CO_UNLOCK_OD(CAN_MODULE) ((void)(CAN_MODULE), hf_co_driver_unlock())

#define CO_MemoryBarrier() __asm__ __volatile__("" ::: "memory")
#define CO_FLAG_READ(rxNew) ((rxNew) != NULL)
#define CO_FLAG_SET(rxNew)                                                                                             \
  do {                                                                                                                 \
    CO_MemoryBarrier();                                                                                               \
    (rxNew) = (void*)1;                                                                                                \
  } while (0)
#define CO_FLAG_CLEAR(rxNew)                                                                                           \
  do {                                                                                                                 \
    CO_MemoryBarrier();                                                                                               \
    (rxNew) = NULL;                                                                                                    \
  } while (0)

#ifdef __cplusplus
}
#endif

#endif /* CO_DRIVER_TARGET_H */
