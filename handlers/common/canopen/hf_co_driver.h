/**
 * @file hf_co_driver.h
 * @brief C interface of the generic CANopenNode driver (CO_driver_hardfoc.c).
 *
 * @details One CANopenNode `CO_CANmodule_t` fans out to up to
 *          `HF_CO_MAX_PORTS` CAN ports. A port is any transport that can send
 *          a frame without blocking and report received frames: the TWAI
 *          controller (EspTwaiPort), SocketCAN (LinuxSocketCanPort), the host
 *          pipe bus (PipePort), later FDCAN and the wireless tunnels. C++ ports
 *          implement `hf::can::ICanPort` (HfCanPort.h) and are attached with
 *          `hf::can::CoPortBinding` (HfCoPortBinding.h); this header is the C
 *          seam underneath.
 *
 *          Threading contract:
 *          - `hf_co_driver_rx_push()` and `hf_co_driver_tx_done()` are
 *            ISR-safe and lock-free. Each port has its own single-producer
 *            ring, so one port must push from one context (its ISR or its
 *            reader thread).
 *          - Received frames are dispatched to CANopenNode from
 *            `CO_CANmodule_process()` (called by `CO_process()`) or from
 *            `hf_co_driver_rx_drain()`, in thread context. CANopenNode's RX
 *            callbacks therefore never run in an ISR.
 *          - `CO_CANsend()` hands the frame to every port inside the stack's
 *            CAN-send lock. Port `send` callbacks must not block. A port that
 *            answers "queue full" keeps the frame pending in its CANopenNode
 *            buffer; it is re-sent in buffer (priority) order from
 *            `CO_CANmodule_process()`, which the port can request at once
 *            with `hf_co_driver_tx_done()`.
 *          - The CO locks (CAN send, EMCY, OD) are one recursive mutex per
 *            CANmodule (FreeRTOS or pthread). Nothing that can block is ever
 *            called while it is held.
 *
 *          Error status: `CO_CANmodule_process()` reads every port attached
 *          with `HF_CO_PORT_ERROR_SOURCE` and maps TEC/REC/bus-off and RX
 *          overruns to the `CO_CAN_ERR*` bits that CO_Emergency turns into
 *          EMCY messages. A bus-off port gets its `recover` callback (for
 *          EspCan: `RequestBusRecovery()`), repeated every
 *          `HF_CO_BUSOFF_RETRY_CALLS` process calls while it stays bus-off.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#ifndef HF_CO_DRIVER_H
#define HF_CO_DRIVER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── Build configuration (override with -D) ─────────────────────────────── */

/** Largest payload CANopenNode buffers carry: 8 on classic builds, 64 with HF_CO_CAN_FD. */
#ifndef HF_CO_CAN_MAX_LEN
#if defined(HF_CO_CAN_FD) && HF_CO_CAN_FD
#define HF_CO_CAN_MAX_LEN 64U
#else
#define HF_CO_CAN_MAX_LEN 8U
#endif
#endif

/** Ports one CANmodule can fan out to. */
#ifndef HF_CO_MAX_PORTS
#define HF_CO_MAX_PORTS 4U
#endif

/** Frames each port's RX ring holds (power of two). */
#ifndef HF_CO_RX_RING_LEN
#define HF_CO_RX_RING_LEN 64U
#endif

/** Driver instances available from hf_co_driver_acquire() (static pool, no heap). */
#ifndef HF_CO_MAX_DRIVERS
#define HF_CO_MAX_DRIVERS 2U
#endif

/** CO_CANmodule_process() calls between bus-off recovery requests while a port stays bus-off. */
#ifndef HF_CO_BUSOFF_RETRY_CALLS
#define HF_CO_BUSOFF_RETRY_CALLS 100U
#endif

#if (HF_CO_RX_RING_LEN & (HF_CO_RX_RING_LEN - 1U)) != 0U
#error "HF_CO_RX_RING_LEN must be a power of two"
#endif
#if HF_CO_MAX_PORTS > 8U
#error "HF_CO_MAX_PORTS is limited to 8 (one pending bit per port in a uint8_t)"
#endif

/* ── Frame ──────────────────────────────────────────────────────────────── */

/** Largest payload of any frame (CAN FD). */
#define HF_CO_FRAME_MAX_LEN 64U

/** `hf_co_frame_t::flags` bits. Same values as hf::can::frame_flag and vortex::contracts::frame_flag. */
#define HF_CO_FRAME_EXT 0x01U /**< 29-bit identifier */
#define HF_CO_FRAME_RTR 0x02U /**< remote request */
#define HF_CO_FRAME_FDF 0x04U /**< FD frame format */
#define HF_CO_FRAME_BRS 0x08U /**< bit-rate switch */
#define HF_CO_FRAME_ESI 0x10U /**< error state indicator */

/** Transport frame. Same layout as hf::can::CanFrame (checked in HfCoPortBinding.h). */
typedef struct hf_co_frame {
  uint32_t id;                        /**< 11- or 29-bit identifier */
  uint8_t len;                        /**< payload length in bytes (not a DLC code) */
  uint8_t flags;                      /**< HF_CO_FRAME_* */
  uint8_t data[HF_CO_FRAME_MAX_LEN];  /**< payload */
  uint32_t ts_us;                     /**< RX timestamp (port clock); ignored on TX */
} hf_co_frame_t;

/* ── Port ───────────────────────────────────────────────────────────────── */

/** Result of a non-blocking port send. Same order as hf::can::TxResult. */
typedef enum hf_co_tx_result {
  HF_CO_TX_OK = 0,          /**< queued for transmission */
  HF_CO_TX_QUEUE_FULL = 1,  /**< no room now; the frame stays pending and is retried */
  HF_CO_TX_BUS_OFF = 2,     /**< controller bus-off; the frame is dropped for this port */
  HF_CO_TX_NOT_ALLOWED = 3, /**< the port cannot carry this frame; dropped for this port */
} hf_co_tx_result_t;

/** Controller error state. Same order as hf::can::BusState. */
typedef enum hf_co_bus_state {
  HF_CO_BUS_STOPPED = 0,
  HF_CO_BUS_ERROR_ACTIVE = 1,
  HF_CO_BUS_ERROR_WARNING = 2,
  HF_CO_BUS_ERROR_PASSIVE = 3,
  HF_CO_BUS_OFF = 4,
} hf_co_bus_state_t;

/** Port error counters. Same fields as hf::can::PortStatus. */
typedef struct hf_co_port_status {
  uint8_t state;           /**< hf_co_bus_state_t */
  uint16_t tec;            /**< transmit error counter */
  uint16_t rec;            /**< receive error counter */
  uint32_t rx_overruns;    /**< frames the port itself lost */
  uint32_t tx_queue_full;  /**< sends answered "queue full" */
  uint32_t bus_off_count;  /**< bus-off events */
} hf_co_port_status_t;

/** Port callbacks. `send` is required; the others may be NULL. */
typedef struct hf_co_port_ops {
  /** Queue @p frame without blocking. Called with the CO CAN-send lock held. */
  hf_co_tx_result_t (*send)(void* ctx, const hf_co_frame_t* frame);
  /** Fill @p out; return false if unavailable. Called from CO_CANmodule_process(). */
  bool (*status)(void* ctx, hf_co_port_status_t* out);
  /** Request bus-off recovery; return false if refused. Called without the CO lock. */
  bool (*recover)(void* ctx);
} hf_co_port_ops_t;

/** The port is a physical bus: its counters drive CANerrorStatus (warning/passive/bus-off). */
#define HF_CO_PORT_ERROR_SOURCE 0x01U

/** Per-port driver statistics. */
typedef struct hf_co_port_stats {
  uint32_t rx_frames;         /**< frames accepted into the RX ring */
  uint32_t rx_ring_overruns;  /**< frames lost because the RX ring was full */
  uint32_t rx_rejected;       /**< frames dropped at push: too long for the build, malformed */
  uint32_t rx_dispatched;     /**< frames delivered to a CANopenNode consumer */
  uint32_t rx_unhandled;      /**< frames no consumer matched */
  uint32_t rx_denied;         /**< frames the RX policy refused */
  uint32_t tx_frames;         /**< frames the port accepted */
  uint32_t tx_queue_full;     /**< sends answered "queue full" (frame left pending) */
  uint32_t tx_retried;        /**< pending frames sent later */
  uint32_t tx_dropped;        /**< frames the port refused (bus-off, not allowed) */
  uint32_t tx_denied;         /**< frames the TX policy or the length check refused */
  uint32_t recover_requests;  /**< bus-off recovery requests */
} hf_co_port_stats_t;

/* ── Policy and wake hooks ──────────────────────────────────────────────── */

/**
 * RX policy: may @p frame from @p port reach the consumer in CANopenNode RX
 * buffer @p rx_index (whose object is @p consumer)? Thread context.
 */
typedef bool (*hf_co_rx_policy_fn)(void* ctx, uint8_t port, uint16_t rx_index, const void* consumer,
                                   const hf_co_frame_t* frame);
/**
 * TX policy: may @p frame from CANopenNode TX buffer @p tx_index leave on
 * @p port? Called with the CO CAN-send lock held: decide, do not block.
 */
typedef bool (*hf_co_tx_policy_fn)(void* ctx, uint8_t port, uint16_t tx_index, const hf_co_frame_t* frame);
/** Wake the thread that calls CO_process(). Must be ISR-safe. */
typedef void (*hf_co_wake_fn)(void* ctx);

/* ── Driver instance ────────────────────────────────────────────────────── */

/** Opaque driver instance; pass it as `CANptr` to CO_CANinit(). */
typedef struct hf_co_driver hf_co_driver_t;

/** Take a driver instance from the static pool (NULL when all are in use). */
hf_co_driver_t* hf_co_driver_acquire(void);
/** Return @p drv to the pool. Detach the ports and stop calling CO first. */
void hf_co_driver_release(hf_co_driver_t* drv);

/**
 * Attach a port at @p index.
 * @param max_len largest payload the port carries (8 classic, 64 FD)
 * @param flags HF_CO_PORT_* bits
 * @return false on a bad argument or an occupied index
 */
bool hf_co_driver_attach_port(hf_co_driver_t* drv, uint8_t index, const hf_co_port_ops_t* ops, void* ctx,
                              uint8_t max_len, uint8_t flags);
/** Detach the port at @p index. Its pending TX frames are dropped. */
void hf_co_driver_detach_port(hf_co_driver_t* drv, uint8_t index);

/** Install the router policy (either function may be NULL = allow). */
void hf_co_driver_set_policy(hf_co_driver_t* drv, hf_co_rx_policy_fn rx, hf_co_tx_policy_fn tx, void* ctx);
/** Install the wake hook called after every RX push and TX-done. */
void hf_co_driver_set_wake(hf_co_driver_t* drv, hf_co_wake_fn wake, void* ctx);

/**
 * Store a received frame for dispatch. ISR-safe, lock-free, single producer
 * per port. Frames longer than HF_CO_CAN_MAX_LEN are rejected (counted).
 * @return false if the frame was dropped
 */
bool hf_co_driver_rx_push(hf_co_driver_t* drv, uint8_t port, const hf_co_frame_t* frame);

/**
 * Like hf_co_driver_rx_push(), for ports that apply back-pressure: a full
 * ring returns false without counting an overrun, and the port offers the
 * frame again later.
 */
bool hf_co_driver_rx_offer(hf_co_driver_t* drv, uint8_t port, const hf_co_frame_t* frame);

/** The port has room again (TX-done). ISR-safe: marks the retry and wakes. */
void hf_co_driver_tx_done(hf_co_driver_t* drv, uint8_t port);

/**
 * Dispatch up to @p max queued RX frames to CANopenNode (0 = all queued).
 * Thread context. CO_CANmodule_process() calls this; call it directly to
 * drain earlier (for example from an RT thread before CO_process_SYNC).
 * @return frames taken from the rings
 */
uint32_t hf_co_driver_rx_drain(hf_co_driver_t* drv, uint32_t max);

/** Frames waiting in the RX rings. */
uint32_t hf_co_driver_rx_pending(const hf_co_driver_t* drv);
/** CANopenNode TX buffers still pending on at least one port. */
uint32_t hf_co_driver_tx_pending(const hf_co_driver_t* drv);

/** Copy the statistics of @p port. */
void hf_co_driver_get_stats(const hf_co_driver_t* drv, uint8_t port, hf_co_port_stats_t* out);

#ifdef __cplusplus
}
#endif

#endif /* HF_CO_DRIVER_H */
