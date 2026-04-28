/**
 * @file hf_co_driver_iface.h
 * @brief C linkage between CANopenNode CO_driver_vortex.c and the Vortex app (BaseCan).
 */
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Register TX hook used by CO_CANsend (BaseCan::SendMessage in C++). Returns 0 on success. */
void hf_co_driver_register_tx(int (*tx_fn)(uint32_t std_id, uint8_t dlc, const uint8_t* data, void* ctx), void* ctx);

/**
 * Feed one received standard (11-bit) data frame into CANopenNode RX dispatch.
 * @param CANmodule CO_CANmodule_t* from CO_t (opaque here so C++ does not depend on a wrong `struct` forward decl).
 */
void hf_co_driver_process_rx(void* CANmodule, uint16_t std_id, uint8_t dlc, const uint8_t* data);

#ifdef __cplusplus
}
#endif
