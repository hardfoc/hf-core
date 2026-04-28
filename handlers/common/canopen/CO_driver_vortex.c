/**
 * @file CO_driver_vortex.c
 * @brief CANopenNode hardware driver for HardFOC Vortex: TWAI via registered TX/RX hooks.
 */
#include "301/CO_driver.h"

#include "hf_co_driver_iface.h"

#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"

static portMUX_TYPE s_co_mux = portMUX_INITIALIZER_UNLOCKED;

void hf_co_driver_lock(void) { portENTER_CRITICAL(&s_co_mux); }

void hf_co_driver_unlock(void) { portEXIT_CRITICAL(&s_co_mux); }

/** Returns 0 on success, non-zero on failure (propagated as CO_ERROR_TX_BUSY / overflow semantics). */
static int (*s_tx_fn)(uint32_t std_id, uint8_t dlc, const uint8_t* data, void* ctx) = NULL;
static void* s_tx_ctx = NULL;

void hf_co_driver_register_tx(int (*tx_fn)(uint32_t std_id, uint8_t dlc, const uint8_t* data, void* ctx),
                               void* ctx) {
  s_tx_fn = tx_fn;
  s_tx_ctx = ctx;
}

void CO_CANsetConfigurationMode(void* CANptr) { (void)CANptr; }

void CO_CANsetNormalMode(CO_CANmodule_t* CANmodule) {
  if (CANmodule != NULL) {
    CANmodule->CANnormal = true;
  }
}

CO_ReturnError_t CO_CANmodule_init(CO_CANmodule_t* CANmodule, void* CANptr, CO_CANrx_t rxArray[], uint16_t rxSize,
                                   CO_CANtx_t txArray[], uint16_t txSize, uint16_t CANbitRate) {
  (void)CANbitRate;
  if (CANmodule == NULL || rxArray == NULL || txArray == NULL) {
    return CO_ERROR_ILLEGAL_ARGUMENT;
  }

  CANmodule->CANptr = CANptr;
  CANmodule->rxArray = rxArray;
  CANmodule->rxSize = rxSize;
  CANmodule->txArray = txArray;
  CANmodule->txSize = txSize;
  CANmodule->CANerrorStatus = 0;
  CANmodule->CANnormal = false;
  /* Software dispatch only: scan rxArray (TWAI has no per-slot HW filter API wired here). */
  CANmodule->useCANrxFilters = false;
  CANmodule->bufferInhibitFlag = false;
  CANmodule->firstCANtxMessage = true;
  CANmodule->CANtxCount = 0U;
  CANmodule->errOld = 0U;

  for (uint16_t i = 0U; i < rxSize; i++) {
    rxArray[i].ident = 0U;
    rxArray[i].mask = 0xFFFFU;
    rxArray[i].object = NULL;
    rxArray[i].CANrx_callback = NULL;
  }
  for (uint16_t i = 0U; i < txSize; i++) {
    txArray[i].bufferFull = false;
  }
  return CO_ERROR_NO;
}

void CO_CANmodule_disable(CO_CANmodule_t* CANmodule) {
  if (CANmodule != NULL) {
    CANmodule->CANnormal = false;
  }
}

CO_ReturnError_t CO_CANrxBufferInit(CO_CANmodule_t* CANmodule, uint16_t index, uint16_t ident, uint16_t mask, bool_t rtr,
                                    void* object, void (*CANrx_callback)(void* object, void* message)) {
  CO_ReturnError_t ret = CO_ERROR_NO;

  if ((CANmodule != NULL) && (object != NULL) && (CANrx_callback != NULL) && (index < CANmodule->rxSize)) {
    CO_CANrx_t* buffer = &CANmodule->rxArray[index];
    buffer->object = object;
    buffer->CANrx_callback = CANrx_callback;
    buffer->ident = ident & 0x07FFU;
    if (rtr) {
      buffer->ident |= 0x0800U;
    }
    buffer->mask = (mask & 0x07FFU) | 0x0800U;
  } else {
    ret = CO_ERROR_ILLEGAL_ARGUMENT;
  }
  return ret;
}

CO_CANtx_t* CO_CANtxBufferInit(CO_CANmodule_t* CANmodule, uint16_t index, uint16_t ident, bool_t rtr, uint8_t noOfBytes,
                               bool_t syncFlag) {
  CO_CANtx_t* buffer = NULL;

  if ((CANmodule != NULL) && (index < CANmodule->txSize)) {
    buffer = &CANmodule->txArray[index];
    buffer->ident = ((uint32_t)ident & 0x07FFU) | ((uint32_t)(((uint32_t)noOfBytes & 0xFU) << 11U))
                    | ((uint32_t)(rtr ? 0x8000U : 0U));
    buffer->DLC = noOfBytes;
    buffer->bufferFull = false;
    buffer->syncFlag = syncFlag;
  }
  return buffer;
}

static void decode_tx_ident(const CO_CANtx_t* buffer, uint16_t* out_id, uint8_t* out_dlc, bool_t* out_rtr) {
  *out_id = (uint16_t)(buffer->ident & 0x07FFU);
  *out_dlc = (uint8_t)((buffer->ident >> 11) & 0x0FU);
  if (*out_dlc > 8U) {
    *out_dlc = 8U;
  }
  *out_rtr = ((buffer->ident & 0x8000U) != 0U);
  (void)out_rtr; /* TWAI path sends data frames only from stack */
}

CO_ReturnError_t CO_CANsend(CO_CANmodule_t* CANmodule, CO_CANtx_t* buffer) {
  if (CANmodule == NULL || buffer == NULL) {
    return CO_ERROR_ILLEGAL_ARGUMENT;
  }

  if (buffer->bufferFull) {
    if (!CANmodule->firstCANtxMessage) {
      CANmodule->CANerrorStatus |= CO_CAN_ERRTX_OVERFLOW;
    }
    return CO_ERROR_TX_OVERFLOW;
  }

  uint16_t std_id = 0;
  uint8_t dlc = 0;
  bool_t rtr = false;
  decode_tx_ident(buffer, &std_id, &dlc, &rtr);
  (void)rtr;

  CO_ReturnError_t err = CO_ERROR_NO;
  CO_LOCK_CAN_SEND(CANmodule);
  if (s_tx_fn == NULL) {
    err = CO_ERROR_INVALID_STATE;
  } else {
    const int rc = s_tx_fn((uint32_t)std_id, dlc, buffer->data, s_tx_ctx);
    if (rc != 0) {
      buffer->bufferFull = true;
      CANmodule->CANtxCount++;
      err = CO_ERROR_TX_BUSY;
    } else {
      CANmodule->firstCANtxMessage = false;
      CANmodule->bufferInhibitFlag = buffer->syncFlag;
    }
  }
  CO_UNLOCK_CAN_SEND(CANmodule);
  return err;
}

void CO_CANclearPendingSyncPDOs(CO_CANmodule_t* CANmodule) {
  uint32_t tpdoDeleted = 0U;

  CO_LOCK_CAN_SEND(CANmodule);
  if (CANmodule->bufferInhibitFlag) {
    CANmodule->bufferInhibitFlag = false;
    tpdoDeleted = 1U;
  }
  if (CANmodule->CANtxCount != 0U) {
    uint16_t i;
    CO_CANtx_t* buffer = &CANmodule->txArray[0];
    for (i = CANmodule->txSize; i > 0U; i--) {
      if (buffer->bufferFull) {
        if (buffer->syncFlag) {
          buffer->bufferFull = false;
          CANmodule->CANtxCount--;
          tpdoDeleted = 2U;
        }
      }
      buffer++;
    }
  }
  CO_UNLOCK_CAN_SEND(CANmodule);

  if (tpdoDeleted != 0U) {
    CANmodule->CANerrorStatus |= CO_CAN_ERRTX_PDO_LATE;
  }
}

void CO_CANmodule_process(CO_CANmodule_t* CANmodule) {
  (void)CANmodule;
}

static void dispatch_rx(CO_CANmodule_t* CANmodule, CO_CANrxMsg_t* rcvMsg) {
  const uint16_t rcvMsgIdent = rcvMsg->ident;

  for (uint16_t i = 0U; i < CANmodule->rxSize; i++) {
    CO_CANrx_t* buffer = &CANmodule->rxArray[i];
    if (((rcvMsgIdent ^ buffer->ident) & buffer->mask) == 0U) {
      if (buffer->CANrx_callback != NULL) {
        buffer->CANrx_callback(buffer->object, (void*)rcvMsg);
      }
      break;
    }
  }
}

void hf_co_driver_process_rx(void* CANmodule_void, uint16_t std_id, uint8_t dlc, const uint8_t* data) {
  CO_CANmodule_t* CANmodule = (CO_CANmodule_t*)CANmodule_void;
  if (CANmodule == NULL || !CANmodule->CANnormal) {
    return;
  }
  CO_CANrxMsg_t rx;
  rx.ident = std_id & 0x7FFU;
  rx.dlc = (dlc > 8U) ? 8U : dlc;
  if (data != NULL) {
    (void)memcpy(rx.data, data, rx.dlc);
  } else {
    (void)memset(rx.data, 0, sizeof(rx.data));
  }
  dispatch_rx(CANmodule, &rx);
}

void CO_CANinterrupt(CO_CANmodule_t* CANmodule) {
  (void)CANmodule;
}
