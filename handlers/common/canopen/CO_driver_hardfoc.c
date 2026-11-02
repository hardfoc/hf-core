/**
 * @file CO_driver_hardfoc.c
 * @brief Generic CANopenNode v4 driver: one CO_CANmodule over N ports.
 *
 * @details See hf_co_driver.h for the threading contract. In short:
 *          - RX: ports push frames into per-port lock-free rings (ISR-safe);
 *            CO_CANmodule_process() / hf_co_driver_rx_drain() dispatch them
 *            to CANopenNode in thread context, through the RX policy.
 *          - TX: CO_CANsend() offers the frame to every port without
 *            blocking. A port that is full leaves the frame pending in its
 *            TX buffer (bufferFull, one pending bit per port); pending frames
 *            are re-sent in buffer order from CO_CANmodule_process(), which a
 *            port triggers at once with hf_co_driver_tx_done().
 *          - Error status: port TEC/REC/bus-off and RX overruns map to the
 *            CO_CAN_ERR* bits; bus-off ports get a recovery request.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "301/CO_driver.h"

#include <stdatomic.h>
#include <string.h>

/* ── Instance ───────────────────────────────────────────────────────────── */

typedef struct {
  uint32_t id;
  uint8_t len;
  uint8_t flags;
  uint8_t data[CO_CAN_MAX_LEN];
  uint32_t ts_us;
} hf_co_rx_elem_t;

typedef struct {
  hf_co_rx_elem_t elem[HF_CO_RX_RING_LEN];
  atomic_uint head; /* written by the producer (port) */
  atomic_uint tail; /* written by the consumer (drain) */
} hf_co_rx_ring_t;

typedef struct {
  atomic_uint rx_frames;
  atomic_uint rx_ring_overruns;
  atomic_uint rx_rejected;
  atomic_uint rx_dispatched;
  atomic_uint rx_unhandled;
  atomic_uint rx_denied;
  atomic_uint tx_frames;
  atomic_uint tx_queue_full;
  atomic_uint tx_retried;
  atomic_uint tx_dropped;
  atomic_uint tx_denied;
  atomic_uint recover_requests;
} hf_co_stats_atomic_t;

typedef struct {
  atomic_bool attached;
  hf_co_port_ops_t ops;
  void* ctx;
  uint8_t max_len;
  uint8_t flags;
  /* Below: CO lock held, or the processing thread only. */
  uint16_t pending;          /* TX buffers pending on this port */
  bool was_bus_off;
  uint32_t recover_countdown;
  uint32_t last_overruns;    /* port overruns + ring overruns at the last check */
} hf_co_port_slot_t;

struct hf_co_driver {
  atomic_bool in_use;
  CO_CANmodule_t* module;
  hf_co_port_slot_t port[HF_CO_MAX_PORTS];
  hf_co_rx_ring_t ring[HF_CO_MAX_PORTS];
  hf_co_stats_atomic_t stats[HF_CO_MAX_PORTS];
  hf_co_rx_policy_fn rx_policy;
  hf_co_tx_policy_fn tx_policy;
  void* policy_ctx;
  _Atomic(hf_co_wake_fn) wake;
  void* _Atomic wake_ctx;
  atomic_uint tx_done_mask;  /* ports that reported room (cleared by process) */
  atomic_flag drain_busy;    /* single consumer guard */
  uint8_t drain_next;        /* round-robin start port */
  uint16_t transient_seen;   /* transient CANerrorStatus bits seen at the last process call */
};

static hf_co_driver_t s_drivers[HF_CO_MAX_DRIVERS];

#define HF_CO_ERR_TRANSIENT (CO_CAN_ERRTX_OVERFLOW | CO_CAN_ERRTX_PDO_LATE | CO_CAN_ERRRX_OVERFLOW)
#define HF_CO_ERR_LEVEL                                                                                                \
  (CO_CAN_ERRTX_WARNING | CO_CAN_ERRTX_PASSIVE | CO_CAN_ERRTX_BUS_OFF | CO_CAN_ERRRX_WARNING | CO_CAN_ERRRX_PASSIVE)

static inline void stat_inc(atomic_uint* s) { atomic_fetch_add_explicit(s, 1U, memory_order_relaxed); }

static inline hf_co_driver_t* driver_of(const CO_CANmodule_t* m) {
  return (m != NULL) ? (hf_co_driver_t*)m->CANptr : NULL;
}

static bool port_live(const hf_co_driver_t* d, uint8_t p) {
  return atomic_load_explicit(&d->port[p].attached, memory_order_acquire);
}

/* ── Locks ──────────────────────────────────────────────────────────────── */

static void lock_create(CO_CANmodule_t* m) {
  if (m->lockReady) {
    return;
  }
#if defined(HF_CO_LOCK_FREERTOS)
  m->lock = xSemaphoreCreateRecursiveMutexStatic(&m->lockStorage);
#elif defined(HF_CO_LOCK_PTHREAD)
  pthread_mutexattr_t attr;
  (void)pthread_mutexattr_init(&attr);
  (void)pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  (void)pthread_mutex_init(&m->lock, &attr);
  (void)pthread_mutexattr_destroy(&attr);
#endif
  m->lockReady = true;
}

void hf_co_lock(CO_CANmodule_t* CANmodule) {
  if (CANmodule == NULL || !CANmodule->lockReady) {
    return;
  }
#if defined(HF_CO_LOCK_FREERTOS)
  (void)xSemaphoreTakeRecursive(CANmodule->lock, portMAX_DELAY);
#elif defined(HF_CO_LOCK_PTHREAD)
  (void)pthread_mutex_lock(&CANmodule->lock);
#endif
}

void hf_co_unlock(CO_CANmodule_t* CANmodule) {
  if (CANmodule == NULL || !CANmodule->lockReady) {
    return;
  }
#if defined(HF_CO_LOCK_FREERTOS)
  (void)xSemaphoreGiveRecursive(CANmodule->lock);
#elif defined(HF_CO_LOCK_PTHREAD)
  (void)pthread_mutex_unlock(&CANmodule->lock);
#endif
}

/* ── Driver instance API ────────────────────────────────────────────────── */

static void stats_reset(hf_co_stats_atomic_t* s) {
  atomic_store(&s->rx_frames, 0U);
  atomic_store(&s->rx_ring_overruns, 0U);
  atomic_store(&s->rx_rejected, 0U);
  atomic_store(&s->rx_dispatched, 0U);
  atomic_store(&s->rx_unhandled, 0U);
  atomic_store(&s->rx_denied, 0U);
  atomic_store(&s->tx_frames, 0U);
  atomic_store(&s->tx_queue_full, 0U);
  atomic_store(&s->tx_retried, 0U);
  atomic_store(&s->tx_dropped, 0U);
  atomic_store(&s->tx_denied, 0U);
  atomic_store(&s->recover_requests, 0U);
}

hf_co_driver_t* hf_co_driver_acquire(void) {
  for (size_t i = 0; i < HF_CO_MAX_DRIVERS; i++) {
    bool expected = false;
    if (atomic_compare_exchange_strong(&s_drivers[i].in_use, &expected, true)) {
      hf_co_driver_t* d = &s_drivers[i];
      d->module = NULL;
      for (uint8_t p = 0; p < HF_CO_MAX_PORTS; p++) {
        atomic_store(&d->port[p].attached, false);
        memset(&d->port[p].ops, 0, sizeof(d->port[p].ops));
        d->port[p].ctx = NULL;
        d->port[p].max_len = 0;
        d->port[p].flags = 0;
        d->port[p].pending = 0;
        d->port[p].was_bus_off = false;
        d->port[p].recover_countdown = 0;
        d->port[p].last_overruns = 0;
        atomic_store(&d->ring[p].head, 0U);
        atomic_store(&d->ring[p].tail, 0U);
        stats_reset(&d->stats[p]);
      }
      d->rx_policy = NULL;
      d->tx_policy = NULL;
      d->policy_ctx = NULL;
      atomic_store(&d->wake, (hf_co_wake_fn)NULL);
      atomic_store(&d->wake_ctx, NULL);
      atomic_store(&d->tx_done_mask, 0U);
      atomic_flag_clear(&d->drain_busy);
      d->drain_next = 0;
      d->transient_seen = 0;
      return d;
    }
  }
  return NULL;
}

void hf_co_driver_release(hf_co_driver_t* drv) {
  if (drv == NULL) {
    return;
  }
  for (uint8_t p = 0; p < HF_CO_MAX_PORTS; p++) {
    atomic_store(&drv->port[p].attached, false);
  }
  drv->module = NULL;
  atomic_store(&drv->in_use, false);
}

bool hf_co_driver_attach_port(hf_co_driver_t* drv, uint8_t index, const hf_co_port_ops_t* ops, void* ctx,
                              uint8_t max_len, uint8_t flags) {
  if (drv == NULL || ops == NULL || ops->send == NULL || index >= HF_CO_MAX_PORTS || max_len == 0U) {
    return false;
  }
  hf_co_port_slot_t* s = &drv->port[index];
  if (port_live(drv, index)) {
    return false;
  }
  s->ops = *ops;
  s->ctx = ctx;
  s->max_len = max_len;
  s->flags = flags;
  s->pending = 0;
  s->was_bus_off = false;
  s->recover_countdown = 0;
  s->last_overruns = 0;
  atomic_store(&drv->ring[index].head, 0U);
  atomic_store(&drv->ring[index].tail, 0U);
  atomic_store_explicit(&s->attached, true, memory_order_release);
  return true;
}

void hf_co_driver_detach_port(hf_co_driver_t* drv, uint8_t index) {
  if (drv == NULL || index >= HF_CO_MAX_PORTS) {
    return;
  }
  CO_CANmodule_t* m = drv->module;
  hf_co_lock(m);
  atomic_store_explicit(&drv->port[index].attached, false, memory_order_release);
  if (m != NULL && m->txArray != NULL) {
    const uint8_t bit = (uint8_t)(1U << index);
    for (uint16_t i = 0; i < m->txSize; i++) {
      CO_CANtx_t* b = &m->txArray[i];
      if (b->bufferFull && (b->pendingPorts & bit) != 0U) {
        b->pendingPorts = (uint8_t)(b->pendingPorts & (uint8_t)~bit);
        if (b->pendingPorts == 0U) {
          b->bufferFull = false;
          m->CANtxCount--;
        }
      }
    }
  }
  drv->port[index].pending = 0;
  hf_co_unlock(m);
}

void hf_co_driver_set_policy(hf_co_driver_t* drv, hf_co_rx_policy_fn rx, hf_co_tx_policy_fn tx, void* ctx) {
  if (drv == NULL) {
    return;
  }
  CO_CANmodule_t* m = drv->module;
  hf_co_lock(m);
  drv->rx_policy = rx;
  drv->tx_policy = tx;
  drv->policy_ctx = ctx;
  hf_co_unlock(m);
}

void hf_co_driver_set_wake(hf_co_driver_t* drv, hf_co_wake_fn wake, void* ctx) {
  if (drv == NULL) {
    return;
  }
  atomic_store(&drv->wake, (hf_co_wake_fn)NULL);
  atomic_store(&drv->wake_ctx, ctx);
  atomic_store(&drv->wake, wake);
}

static void wake_up(hf_co_driver_t* d) {
  hf_co_wake_fn fn = atomic_load_explicit(&d->wake, memory_order_acquire);
  if (fn != NULL) {
    fn(atomic_load_explicit(&d->wake_ctx, memory_order_relaxed));
  }
}

static bool ring_push(hf_co_driver_t* drv, uint8_t port, const hf_co_frame_t* frame, bool count_overrun) {
  if (drv == NULL || frame == NULL || port >= HF_CO_MAX_PORTS || !port_live(drv, port)) {
    return false;
  }
  hf_co_stats_atomic_t* st = &drv->stats[port];
  if (frame->len > CO_CAN_MAX_LEN || frame->len > drv->port[port].max_len) {
    stat_inc(&st->rx_rejected);
    return false;
  }
  hf_co_rx_ring_t* r = &drv->ring[port];
  const unsigned head = atomic_load_explicit(&r->head, memory_order_relaxed);
  const unsigned tail = atomic_load_explicit(&r->tail, memory_order_acquire);
  if ((unsigned)(head - tail) >= HF_CO_RX_RING_LEN) {
    if (count_overrun) {
      stat_inc(&st->rx_ring_overruns);
    }
    wake_up(drv);
    return false;
  }
  hf_co_rx_elem_t* e = &r->elem[head & (HF_CO_RX_RING_LEN - 1U)];
  e->id = frame->id;
  e->len = frame->len;
  e->flags = frame->flags;
  e->ts_us = frame->ts_us;
  memcpy(e->data, frame->data, frame->len);
  atomic_store_explicit(&r->head, head + 1U, memory_order_release);
  stat_inc(&st->rx_frames);
  wake_up(drv);
  return true;
}

bool hf_co_driver_rx_push(hf_co_driver_t* drv, uint8_t port, const hf_co_frame_t* frame) {
  return ring_push(drv, port, frame, true);
}

bool hf_co_driver_rx_offer(hf_co_driver_t* drv, uint8_t port, const hf_co_frame_t* frame) {
  return ring_push(drv, port, frame, false);
}

void hf_co_driver_tx_done(hf_co_driver_t* drv, uint8_t port) {
  if (drv == NULL || port >= HF_CO_MAX_PORTS) {
    return;
  }
  atomic_fetch_or_explicit(&drv->tx_done_mask, 1U << port, memory_order_release);
  wake_up(drv);
}

uint32_t hf_co_driver_rx_pending(const hf_co_driver_t* drv) {
  if (drv == NULL) {
    return 0;
  }
  uint32_t n = 0;
  for (uint8_t p = 0; p < HF_CO_MAX_PORTS; p++) {
    const hf_co_rx_ring_t* r = &drv->ring[p];
    n += (uint32_t)(atomic_load_explicit(&r->head, memory_order_acquire)
                    - atomic_load_explicit(&r->tail, memory_order_acquire));
  }
  return n;
}

uint32_t hf_co_driver_tx_pending(const hf_co_driver_t* drv) {
  if (drv == NULL || drv->module == NULL) {
    return 0;
  }
  CO_CANmodule_t* m = drv->module;
  hf_co_lock(m);
  const uint32_t n = m->CANtxCount;
  hf_co_unlock(m);
  return n;
}

void hf_co_driver_get_stats(const hf_co_driver_t* drv, uint8_t port, hf_co_port_stats_t* out) {
  if (out == NULL) {
    return;
  }
  memset(out, 0, sizeof(*out));
  if (drv == NULL || port >= HF_CO_MAX_PORTS) {
    return;
  }
  const hf_co_stats_atomic_t* s = &drv->stats[port];
  out->rx_frames = atomic_load(&s->rx_frames);
  out->rx_ring_overruns = atomic_load(&s->rx_ring_overruns);
  out->rx_rejected = atomic_load(&s->rx_rejected);
  out->rx_dispatched = atomic_load(&s->rx_dispatched);
  out->rx_unhandled = atomic_load(&s->rx_unhandled);
  out->rx_denied = atomic_load(&s->rx_denied);
  out->tx_frames = atomic_load(&s->tx_frames);
  out->tx_queue_full = atomic_load(&s->tx_queue_full);
  out->tx_retried = atomic_load(&s->tx_retried);
  out->tx_dropped = atomic_load(&s->tx_dropped);
  out->tx_denied = atomic_load(&s->tx_denied);
  out->recover_requests = atomic_load(&s->recover_requests);
}

/* ── RX dispatch ────────────────────────────────────────────────────────── */

static void dispatch(hf_co_driver_t* d, CO_CANmodule_t* m, uint8_t port, const hf_co_rx_elem_t* e) {
  hf_co_stats_atomic_t* st = &d->stats[port];
  if ((e->flags & HF_CO_FRAME_EXT) != 0U) {
    stat_inc(&st->rx_unhandled); /* CANopen uses 11-bit identifiers only */
    return;
  }
  CO_CANrxMsg_t msg;
  msg.ident = (uint16_t)((e->id & 0x07FFU) | (((e->flags & HF_CO_FRAME_RTR) != 0U) ? 0x0800U : 0U));
  msg.dlc = e->len;
  memcpy(msg.data, e->data, e->len);
  if (e->len < CO_CAN_MAX_LEN) {
    memset(&msg.data[e->len], 0, CO_CAN_MAX_LEN - e->len);
  }
  for (uint16_t i = 0; i < m->rxSize; i++) {
    CO_CANrx_t* b = &m->rxArray[i];
    if (b->CANrx_callback == NULL || ((msg.ident ^ b->ident) & b->mask) != 0U) {
      continue;
    }
    if (d->rx_policy != NULL) {
      hf_co_frame_t f;
      f.id = e->id;
      f.len = e->len;
      f.flags = e->flags;
      f.ts_us = e->ts_us;
      memcpy(f.data, e->data, e->len);
      if (!d->rx_policy(d->policy_ctx, port, i, b->object, &f)) {
        stat_inc(&st->rx_denied);
        return;
      }
    }
    b->CANrx_callback(b->object, (void*)&msg);
    stat_inc(&st->rx_dispatched);
    return;
  }
  stat_inc(&st->rx_unhandled);
}

uint32_t hf_co_driver_rx_drain(hf_co_driver_t* drv, uint32_t max) {
  if (drv == NULL || atomic_flag_test_and_set_explicit(&drv->drain_busy, memory_order_acquire)) {
    return 0;
  }
  CO_CANmodule_t* m = drv->module;
  const bool deliver = (m != NULL) && m->CANnormal && (m->rxArray != NULL);
  uint32_t taken = 0;
  bool any = true;
  while (any && (max == 0U || taken < max)) {
    any = false;
    for (uint8_t k = 0; k < HF_CO_MAX_PORTS && (max == 0U || taken < max); k++) {
      const uint8_t p = (uint8_t)((drv->drain_next + k) % HF_CO_MAX_PORTS);
      hf_co_rx_ring_t* r = &drv->ring[p];
      const unsigned tail = atomic_load_explicit(&r->tail, memory_order_relaxed);
      const unsigned head = atomic_load_explicit(&r->head, memory_order_acquire);
      if (tail == head) {
        continue;
      }
      hf_co_rx_elem_t e = r->elem[tail & (HF_CO_RX_RING_LEN - 1U)];
      atomic_store_explicit(&r->tail, tail + 1U, memory_order_release);
      taken++;
      any = true;
      if (deliver) {
        dispatch(drv, m, p, &e);
      }
    }
    drv->drain_next = (uint8_t)((drv->drain_next + 1U) % HF_CO_MAX_PORTS);
  }
  atomic_flag_clear_explicit(&drv->drain_busy, memory_order_release);
  return taken;
}

/* ── TX ─────────────────────────────────────────────────────────────────── */

static void frame_of(const CO_CANtx_t* b, hf_co_frame_t* f) {
  f->id = b->ident & 0x07FFU;
  f->flags = ((b->ident & 0x0800U) != 0U) ? HF_CO_FRAME_RTR : 0U;
  f->len = (b->DLC > CO_CAN_MAX_LEN) ? CO_CAN_MAX_LEN : b->DLC;
  if (f->len > 8U) {
    f->flags |= HF_CO_FRAME_FDF;
  }
  f->ts_us = 0;
  memcpy(f->data, b->data, f->len);
}

/* May the frame of TX buffer @p idx go out on @p p at all? CO lock held. */
static bool tx_allowed(hf_co_driver_t* d, uint8_t p, uint16_t idx, const hf_co_frame_t* f) {
  if (f->len > d->port[p].max_len) {
    return false;
  }
  return d->tx_policy == NULL || d->tx_policy(d->policy_ctx, p, idx, f);
}

CO_ReturnError_t CO_CANsend(CO_CANmodule_t* CANmodule, CO_CANtx_t* buffer) {
  hf_co_driver_t* d = driver_of(CANmodule);
  if (d == NULL || buffer == NULL || buffer < CANmodule->txArray || buffer >= CANmodule->txArray + CANmodule->txSize) {
    return CO_ERROR_ILLEGAL_ARGUMENT;
  }
  const uint16_t idx = (uint16_t)(buffer - CANmodule->txArray);

  CO_LOCK_CAN_SEND(CANmodule);
  if (buffer->bufferFull) {
    if (!CANmodule->firstCANtxMessage) {
      CANmodule->CANerrorStatus |= CO_CAN_ERRTX_OVERFLOW;
    }
    CO_UNLOCK_CAN_SEND(CANmodule);
    return CO_ERROR_TX_OVERFLOW;
  }

  hf_co_frame_t f;
  frame_of(buffer, &f);
  uint8_t pending = 0;
  for (uint8_t p = 0; p < HF_CO_MAX_PORTS; p++) {
    if (!port_live(d, p)) {
      continue;
    }
    hf_co_stats_atomic_t* st = &d->stats[p];
    if (!tx_allowed(d, p, idx, &f)) {
      stat_inc(&st->tx_denied);
      continue;
    }
    if (d->port[p].pending != 0U) {
      /* Keep this port's frames in buffer order behind the ones waiting. */
      pending |= (uint8_t)(1U << p);
      continue;
    }
    switch (d->port[p].ops.send(d->port[p].ctx, &f)) {
      case HF_CO_TX_OK:
        stat_inc(&st->tx_frames);
        CANmodule->firstCANtxMessage = false;
        break;
      case HF_CO_TX_QUEUE_FULL:
        stat_inc(&st->tx_queue_full);
        pending |= (uint8_t)(1U << p);
        break;
      default:
        stat_inc(&st->tx_dropped);
        break;
    }
  }
  if (pending != 0U) {
    buffer->pendingPorts = pending;
    buffer->bufferFull = true;
    CANmodule->CANtxCount++;
    for (uint8_t p = 0; p < HF_CO_MAX_PORTS; p++) {
      if ((pending & (1U << p)) != 0U) {
        d->port[p].pending++;
      }
    }
  }
  CO_UNLOCK_CAN_SEND(CANmodule);
  return CO_ERROR_NO;
}

/* Send pending frames in buffer order, per port, until a port is full. CO lock held. */
static void retry_pending(hf_co_driver_t* d, CO_CANmodule_t* m) {
  if (m->CANtxCount == 0U || m->txArray == NULL) {
    return;
  }
  for (uint8_t p = 0; p < HF_CO_MAX_PORTS; p++) {
    hf_co_port_slot_t* s = &d->port[p];
    if (s->pending == 0U || !port_live(d, p)) {
      continue;
    }
    const uint8_t bit = (uint8_t)(1U << p);
    hf_co_stats_atomic_t* st = &d->stats[p];
    for (uint16_t i = 0; i < m->txSize && s->pending != 0U; i++) {
      CO_CANtx_t* b = &m->txArray[i];
      if (!b->bufferFull || (b->pendingPorts & bit) == 0U) {
        continue;
      }
      hf_co_frame_t f;
      frame_of(b, &f);
      const hf_co_tx_result_t r = s->ops.send(s->ctx, &f);
      if (r == HF_CO_TX_QUEUE_FULL) {
        stat_inc(&st->tx_queue_full);
        break; /* still full: try again on the next TX-done / process call */
      }
      if (r == HF_CO_TX_OK) {
        stat_inc(&st->tx_frames);
        stat_inc(&st->tx_retried);
        m->firstCANtxMessage = false;
      } else {
        stat_inc(&st->tx_dropped);
      }
      b->pendingPorts = (uint8_t)(b->pendingPorts & (uint8_t)~bit);
      s->pending--;
      if (b->pendingPorts == 0U) {
        b->bufferFull = false;
        m->CANtxCount--;
      }
    }
  }
}

void CO_CANclearPendingSyncPDOs(CO_CANmodule_t* CANmodule) {
  hf_co_driver_t* d = driver_of(CANmodule);
  if (d == NULL) {
    return;
  }
  bool deleted = false;
  CO_LOCK_CAN_SEND(CANmodule);
  /* Frames already handed to a port cannot be recalled, so bufferInhibitFlag
   * is never set; only frames still pending are removed. */
  CANmodule->bufferInhibitFlag = false;
  if (CANmodule->CANtxCount != 0U) {
    for (uint16_t i = 0; i < CANmodule->txSize; i++) {
      CO_CANtx_t* b = &CANmodule->txArray[i];
      if (b->bufferFull && b->syncFlag) {
        for (uint8_t p = 0; p < HF_CO_MAX_PORTS; p++) {
          if ((b->pendingPorts & (1U << p)) != 0U && d->port[p].pending != 0U) {
            d->port[p].pending--;
          }
        }
        b->pendingPorts = 0;
        b->bufferFull = false;
        CANmodule->CANtxCount--;
        deleted = true;
      }
    }
  }
  if (deleted) {
    CANmodule->CANerrorStatus |= CO_CAN_ERRTX_PDO_LATE;
  }
  CO_UNLOCK_CAN_SEND(CANmodule);
}

/* ── Error status ───────────────────────────────────────────────────────── */

/* Level bits from one port's counters. */
static uint16_t level_bits(const hf_co_port_status_t* s) {
  uint16_t bits = 0;
  const bool bus_off = (s->state == HF_CO_BUS_OFF) || (s->tec > 255U);
  if (bus_off) {
    return CO_CAN_ERRTX_BUS_OFF | CO_CAN_ERRTX_PASSIVE;
  }
  if (s->tec >= 128U) {
    bits |= CO_CAN_ERRTX_PASSIVE | CO_CAN_ERRTX_WARNING;
  } else if (s->tec >= 96U) {
    bits |= CO_CAN_ERRTX_WARNING;
  }
  if (s->rec >= 128U) {
    bits |= CO_CAN_ERRRX_PASSIVE | CO_CAN_ERRRX_WARNING;
  } else if (s->rec >= 96U) {
    bits |= CO_CAN_ERRRX_WARNING;
  }
  /* A port that reports the state without counters. */
  if (s->state == HF_CO_BUS_ERROR_PASSIVE && (bits & (CO_CAN_ERRTX_PASSIVE | CO_CAN_ERRRX_PASSIVE)) == 0U) {
    bits |= CO_CAN_ERRTX_PASSIVE | CO_CAN_ERRTX_WARNING;
  }
  if (s->state == HF_CO_BUS_ERROR_WARNING && (bits & (CO_CAN_ERRTX_WARNING | CO_CAN_ERRRX_WARNING)) == 0U) {
    bits |= CO_CAN_ERRTX_WARNING;
  }
  return bits;
}

/* Update CANerrorStatus; returns the ports that need a recovery request. CO lock held. */
static uint8_t update_error_status(hf_co_driver_t* d, CO_CANmodule_t* m) {
  uint16_t level = 0;
  bool rx_overflow = false;
  uint8_t recover = 0;
  for (uint8_t p = 0; p < HF_CO_MAX_PORTS; p++) {
    if (!port_live(d, p)) {
      continue;
    }
    hf_co_port_slot_t* s = &d->port[p];
    hf_co_port_status_t ps;
    memset(&ps, 0, sizeof(ps));
    ps.state = HF_CO_BUS_ERROR_ACTIVE;
    const bool have = (s->ops.status != NULL) && s->ops.status(s->ctx, &ps);
    const uint32_t overruns =
        (have ? ps.rx_overruns : 0U) + atomic_load_explicit(&d->stats[p].rx_ring_overruns, memory_order_relaxed);
    if (overruns != s->last_overruns) {
      s->last_overruns = overruns;
      rx_overflow = true;
    }
    if (!have || (s->flags & HF_CO_PORT_ERROR_SOURCE) == 0U) {
      continue;
    }
    const uint16_t bits = level_bits(&ps);
    level |= bits;
    if ((bits & CO_CAN_ERRTX_BUS_OFF) != 0U) {
      if (!s->was_bus_off || s->recover_countdown == 0U) {
        recover |= (uint8_t)(1U << p);
        s->recover_countdown = (HF_CO_BUSOFF_RETRY_CALLS > 0U) ? (HF_CO_BUSOFF_RETRY_CALLS - 1U) : 0U;
      } else {
        s->recover_countdown--;
      }
      s->was_bus_off = true;
    } else {
      s->was_bus_off = false;
    }
  }

  uint16_t st = m->CANerrorStatus;
  /* Transient bits live through one process call (so CO_EM_process sees
   * them), then clear unless they were raised again. */
  st = (uint16_t)(st & (uint16_t)~(d->transient_seen & HF_CO_ERR_TRANSIENT));
  if (rx_overflow) {
    st |= CO_CAN_ERRRX_OVERFLOW;
  }
  st = (uint16_t)((st & (uint16_t)~HF_CO_ERR_LEVEL) | level);
  d->transient_seen = (uint16_t)(st & HF_CO_ERR_TRANSIENT);
  if (rx_overflow) {
    /* raised in this call: let it live through the next one */
    d->transient_seen = (uint16_t)(d->transient_seen & (uint16_t)~CO_CAN_ERRRX_OVERFLOW);
  }
  m->CANerrorStatus = st;
  return recover;
}

void CO_CANmodule_process(CO_CANmodule_t* CANmodule) {
  hf_co_driver_t* d = driver_of(CANmodule);
  if (d == NULL) {
    return;
  }
  (void)hf_co_driver_rx_drain(d, 0);
  (void)atomic_exchange_explicit(&d->tx_done_mask, 0U, memory_order_acquire);

  CO_LOCK_CAN_SEND(CANmodule);
  retry_pending(d, CANmodule);
  const uint8_t recover = update_error_status(d, CANmodule);
  CO_UNLOCK_CAN_SEND(CANmodule);

  for (uint8_t p = 0; p < HF_CO_MAX_PORTS; p++) {
    if ((recover & (1U << p)) != 0U && port_live(d, p) && d->port[p].ops.recover != NULL) {
      stat_inc(&d->stats[p].recover_requests);
      (void)d->port[p].ops.recover(d->port[p].ctx);
    }
  }
}

/* ── Module setup ───────────────────────────────────────────────────────── */

void CO_CANsetConfigurationMode(void* CANptr) { (void)CANptr; }

void CO_CANsetNormalMode(CO_CANmodule_t* CANmodule) {
  if (CANmodule != NULL) {
    CANmodule->CANnormal = true;
  }
}

CO_ReturnError_t CO_CANmodule_init(CO_CANmodule_t* CANmodule, void* CANptr, CO_CANrx_t rxArray[], uint16_t rxSize,
                                   CO_CANtx_t txArray[], uint16_t txSize, uint16_t CANbitRate) {
  (void)CANbitRate; /* the port owns the bit rate */
  if (CANmodule == NULL || CANptr == NULL || rxArray == NULL || txArray == NULL) {
    return CO_ERROR_ILLEGAL_ARGUMENT;
  }
  lock_create(CANmodule);
  hf_co_driver_t* d = (hf_co_driver_t*)CANptr;

  CO_LOCK_CAN_SEND(CANmodule);
  d->module = CANmodule;
  CANmodule->CANptr = CANptr;
  CANmodule->rxArray = rxArray;
  CANmodule->rxSize = rxSize;
  CANmodule->txArray = txArray;
  CANmodule->txSize = txSize;
  CANmodule->CANerrorStatus = 0;
  CANmodule->CANnormal = false;
  CANmodule->useCANrxFilters = false; /* software dispatch over rxArray */
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
    txArray[i].pendingPorts = 0U;
  }
  for (uint8_t p = 0; p < HF_CO_MAX_PORTS; p++) {
    d->port[p].pending = 0U;
  }
  d->transient_seen = 0U;
  CO_UNLOCK_CAN_SEND(CANmodule);
  return CO_ERROR_NO;
}

void CO_CANmodule_disable(CO_CANmodule_t* CANmodule) {
  hf_co_driver_t* d = driver_of(CANmodule);
  if (CANmodule == NULL) {
    return;
  }
  CO_LOCK_CAN_SEND(CANmodule);
  CANmodule->CANnormal = false;
  if (CANmodule->txArray != NULL) {
    for (uint16_t i = 0U; i < CANmodule->txSize; i++) {
      CANmodule->txArray[i].bufferFull = false;
      CANmodule->txArray[i].pendingPorts = 0U;
    }
  }
  CANmodule->CANtxCount = 0U;
  if (d != NULL) {
    for (uint8_t p = 0; p < HF_CO_MAX_PORTS; p++) {
      d->port[p].pending = 0U;
    }
  }
  CO_UNLOCK_CAN_SEND(CANmodule);
}

CO_ReturnError_t CO_CANrxBufferInit(CO_CANmodule_t* CANmodule, uint16_t index, uint16_t ident, uint16_t mask,
                                    bool_t rtr, void* object, void (*CANrx_callback)(void* object, void* message)) {
  if (CANmodule == NULL || object == NULL || CANrx_callback == NULL || index >= CANmodule->rxSize) {
    return CO_ERROR_ILLEGAL_ARGUMENT;
  }
  CO_CANrx_t* buffer = &CANmodule->rxArray[index];
  buffer->object = object;
  buffer->ident = (uint16_t)((ident & 0x07FFU) | (rtr ? 0x0800U : 0U));
  buffer->mask = (uint16_t)((mask & 0x07FFU) | 0x0800U);
  buffer->CANrx_callback = CANrx_callback;
  return CO_ERROR_NO;
}

CO_CANtx_t* CO_CANtxBufferInit(CO_CANmodule_t* CANmodule, uint16_t index, uint16_t ident, bool_t rtr, uint8_t noOfBytes,
                               bool_t syncFlag) {
  if (CANmodule == NULL || index >= CANmodule->txSize) {
    return NULL;
  }
  CO_CANtx_t* buffer = &CANmodule->txArray[index];
  buffer->ident = ((uint32_t)ident & 0x07FFU) | (rtr ? 0x0800U : 0U);
  buffer->DLC = (noOfBytes > CO_CAN_MAX_LEN) ? (uint8_t)CO_CAN_MAX_LEN : noOfBytes;
  buffer->bufferFull = false;
  buffer->syncFlag = syncFlag;
  buffer->pendingPorts = 0U;
  return buffer;
}
