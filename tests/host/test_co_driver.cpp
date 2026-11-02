/**
 * @file test_co_driver.cpp
 * @brief CO_driver_hardfoc.c: TX fan-out and retry, RX ring dispatch, router
 *        policy hooks, error-status mapping and bus-off recovery.
 *
 * The tests drive the driver through the CANopenNode driver API
 * (CO_CANmodule_init, CO_CANrx/txBufferInit, CO_CANsend,
 * CO_CANmodule_process) with scriptable fake ports attached through
 * CoPortBinding, exactly as an application does.
 */

#include <cstring>
#include <deque>
#include <vector>

#include "301/CO_driver.h"
#include "HfCoPortBinding.h"
#include "HostTest.h"

using hf::can::BusState;
using hf::can::CanFrame;
using hf::can::CoPortBinding;
using hf::can::ICanPort;
using hf::can::PortCaps;
using hf::can::PortStatus;
using hf::can::RxSink;
using hf::can::TxResult;

namespace {

class FakePort final : public ICanPort {
 public:
  PortCaps Caps() const override {
    PortCaps c;
    c.max_len = 8;
    c.nominal_bps = 500000;
    return c;
  }
  TxResult Send(const CanFrame& f) override {
    ++send_calls;
    if (result != TxResult::Ok) {
      return result;
    }
    if (room == 0) {
      return TxResult::QueueFull;
    }
    if (room > 0) {
      --room;
    }
    sent.push_back(f);
    return TxResult::Ok;
  }
  void SetRxSink(RxSink s, void* c) override {
    sink = s;
    ctx = c;
  }
  PortStatus Status() const override { return status; }
  bool Recover() override {
    ++recover_calls;
    return true;
  }
  bool Inject(uint32_t id, std::initializer_list<uint8_t> data, uint8_t flags = 0) {
    CanFrame f;
    f.id = id;
    f.flags = flags;
    f.len = static_cast<uint8_t>(data.size());
    std::size_t i = 0;
    for (uint8_t b : data) {
      f.data[i++] = b;
    }
    return sink != nullptr && sink(ctx, f);
  }

  std::vector<CanFrame> sent;
  int room{-1};  // frames the port still takes (-1 = unlimited)
  TxResult result{TxResult::Ok};
  PortStatus status{};
  int send_calls{0};
  int recover_calls{0};
  RxSink sink{nullptr};
  void* ctx{nullptr};
};

struct RxLog {
  struct Entry {
    void* object;
    uint16_t ident;
    uint8_t dlc;
    uint8_t data[8];
  };
  static std::vector<Entry>& Get() {
    static std::vector<Entry> log;
    return log;
  }
  static void Callback(void* object, void* message) {
    Entry e{};
    e.object = object;
    e.ident = CO_CANrxMsg_readIdent(message);
    e.dlc = CO_CANrxMsg_readDLC(message);
    std::memcpy(e.data, CO_CANrxMsg_readData(message), 8);
    Get().push_back(e);
  }
};

/** One driver, one CANmodule, two fake ports (0 = error source, 1 = not). */
struct Rig {
  Rig() {
    RxLog::Get().clear();
    drv = hf_co_driver_acquire();
    REQUIRE(drv != nullptr);
    REQUIRE(bind0.Attach(drv, 0, port0, true));
    REQUIRE(bind1.Attach(drv, 1, port1, false));
    REQUIRE(CO_CANmodule_init(&module, drv, rx, 4, tx, 4, 500) == CO_ERROR_NO);
    CO_CANsetNormalMode(&module);
  }
  ~Rig() {
    bind0.Detach();
    bind1.Detach();
    hf_co_driver_release(drv);
  }
  CO_CANtx_t* Tx(uint16_t index, uint16_t ident, uint8_t len, bool sync = false) {
    CO_CANtx_t* b = CO_CANtxBufferInit(&module, index, ident, false, len, sync);
    for (uint8_t i = 0; i < len; ++i) {
      b->data[i] = static_cast<uint8_t>(index * 16U + i);
    }
    return b;
  }
  hf_co_port_stats_t Stats(uint8_t port) const {
    hf_co_port_stats_t s;
    hf_co_driver_get_stats(drv, port, &s);
    return s;
  }

  hf_co_driver_t* drv{nullptr};
  FakePort port0;
  FakePort port1;
  CoPortBinding bind0;
  CoPortBinding bind1;
  CO_CANmodule_t module{};
  CO_CANrx_t rx[4]{};
  CO_CANtx_t tx[4]{};
};

int g_obj_a = 0;
int g_obj_b = 0;

}  // namespace

HF_TEST(Init_RejectsMissingDriver) {
  CO_CANmodule_t m{};
  CO_CANrx_t rx[1]{};
  CO_CANtx_t tx[1]{};
  CHECK_EQ(CO_CANmodule_init(&m, nullptr, rx, 1, tx, 1, 500), CO_ERROR_ILLEGAL_ARGUMENT);
}

HF_TEST(Pool_IsBounded) {
  hf_co_driver_t* d[HF_CO_MAX_DRIVERS];
  for (auto& x : d) {
    x = hf_co_driver_acquire();
    CHECK(x != nullptr);
  }
  CHECK(hf_co_driver_acquire() == nullptr);
  for (auto& x : d) {
    hf_co_driver_release(x);
  }
  hf_co_driver_t* again = hf_co_driver_acquire();
  CHECK(again != nullptr);
  hf_co_driver_release(again);
}

HF_TEST(Tx_FansOutToEveryPort) {
  Rig r;
  CO_CANtx_t* b = r.Tx(0, 0x185, 3);
  CHECK_EQ(CO_CANsend(&r.module, b), CO_ERROR_NO);
  REQUIRE(r.port0.sent.size() == 1U);
  REQUIRE(r.port1.sent.size() == 1U);
  CHECK_EQ(r.port0.sent[0].id, 0x185U);
  CHECK_EQ(r.port0.sent[0].len, 3U);
  CHECK_EQ(r.port0.sent[0].flags, 0U);
  CHECK_EQ(r.port0.sent[0].data[2], 2U);
  CHECK_EQ(r.port1.sent[0].data[1], 1U);
  CHECK(!b->bufferFull);
  CHECK_EQ(r.module.CANtxCount, 0U);
  CHECK(!r.module.firstCANtxMessage);
}

HF_TEST(Tx_RtrBitBecomesFlag) {
  Rig r;
  CO_CANtx_t* b = CO_CANtxBufferInit(&r.module, 1, 0x705, true, 0, false);
  CHECK_EQ(CO_CANsend(&r.module, b), CO_ERROR_NO);
  REQUIRE(r.port0.sent.size() == 1U);
  CHECK_EQ(r.port0.sent[0].id, 0x705U);
  CHECK_EQ(r.port0.sent[0].flags, HF_CO_FRAME_RTR);
}

HF_TEST(Tx_QueueFullStaysPendingAndRetriesInBufferOrder) {
  Rig r;
  r.port0.room = 0;  // port 0 full, port 1 free
  CO_CANtx_t* b2 = r.Tx(2, 0x582, 8);
  CO_CANtx_t* b0 = r.Tx(0, 0x080, 0);
  CHECK_EQ(CO_CANsend(&r.module, b2), CO_ERROR_NO);
  CHECK_EQ(CO_CANsend(&r.module, b0), CO_ERROR_NO);
  CHECK(b2->bufferFull);
  CHECK(b0->bufferFull);
  CHECK_EQ(r.module.CANtxCount, 2U);
  CHECK_EQ(hf_co_driver_tx_pending(r.drv), 2U);
  CHECK_EQ(r.port1.sent.size(), 2U);  // the free port is not held up
  CHECK_EQ(r.Stats(0).tx_queue_full, 1U);  // the second frame queued behind the first without a send

  // A pending buffer cannot be overwritten.
  CHECK_EQ(CO_CANsend(&r.module, b2), CO_ERROR_TX_OVERFLOW);
  CHECK((r.module.CANerrorStatus & CO_CAN_ERRTX_OVERFLOW) != 0U);

  // Still full: nothing moves, nothing lost.
  CO_CANmodule_process(&r.module);
  CHECK_EQ(r.port0.sent.size(), 0U);
  CHECK_EQ(r.module.CANtxCount, 2U);

  // Room for one: the lower buffer index (higher priority) goes first.
  r.port0.room = 1;
  CO_CANmodule_process(&r.module);
  REQUIRE(r.port0.sent.size() == 1U);
  CHECK_EQ(r.port0.sent[0].id, 0x080U);
  CHECK(!b0->bufferFull);
  CHECK(b2->bufferFull);

  r.port0.room = -1;
  hf_co_driver_tx_done(r.drv, 0);
  CO_CANmodule_process(&r.module);
  REQUIRE(r.port0.sent.size() == 2U);
  CHECK_EQ(r.port0.sent[1].id, 0x582U);
  CHECK_EQ(r.port0.sent[1].data[7], 0x27U);
  CHECK(!b2->bufferFull);
  CHECK_EQ(r.module.CANtxCount, 0U);
  CHECK_EQ(r.Stats(0).tx_retried, 2U);
  CHECK_EQ(r.port1.sent.size(), 2U);  // retries only go to the port that was full
}

HF_TEST(Tx_NewFramesQueueBehindPendingOnes) {
  Rig r;
  r.port0.room = 0;
  CO_CANtx_t* b3 = r.Tx(3, 0x703, 1);
  CHECK_EQ(CO_CANsend(&r.module, b3), CO_ERROR_NO);
  r.port0.room = -1;  // room again, but b3 has not been retried yet
  CO_CANtx_t* b1 = r.Tx(1, 0x181, 2);
  CHECK_EQ(CO_CANsend(&r.module, b1), CO_ERROR_NO);
  CHECK_EQ(r.port0.sent.size(), 0U);  // held so port 0 keeps buffer order
  CHECK(b1->bufferFull);
  CO_CANmodule_process(&r.module);
  REQUIRE(r.port0.sent.size() == 2U);
  CHECK_EQ(r.port0.sent[0].id, 0x181U);
  CHECK_EQ(r.port0.sent[1].id, 0x703U);
}

HF_TEST(Tx_BusOffAndNotAllowedDropForThatPort) {
  Rig r;
  r.port0.result = TxResult::BusOff;
  r.port1.result = TxResult::NotAllowed;
  CO_CANtx_t* b = r.Tx(0, 0x185, 1);
  CHECK_EQ(CO_CANsend(&r.module, b), CO_ERROR_NO);
  CHECK(!b->bufferFull);
  CHECK_EQ(r.Stats(0).tx_dropped, 1U);
  CHECK_EQ(r.Stats(1).tx_dropped, 1U);
}

HF_TEST(Tx_PolicyChoosesPorts) {
  Rig r;
  hf_co_driver_set_policy(
      r.drv, nullptr,
      [](void*, uint8_t port, uint16_t tx_index, const hf_co_frame_t*) { return !(port == 1 && tx_index == 2); },
      nullptr);
  CHECK_EQ(CO_CANsend(&r.module, r.Tx(2, 0x5C5, 8)), CO_ERROR_NO);
  CHECK_EQ(r.port0.sent.size(), 1U);
  CHECK_EQ(r.port1.sent.size(), 0U);
  CHECK_EQ(r.Stats(1).tx_denied, 1U);
  CHECK_EQ(CO_CANsend(&r.module, r.Tx(1, 0x185, 1)), CO_ERROR_NO);
  CHECK_EQ(r.port1.sent.size(), 1U);
}

HF_TEST(Tx_ClearPendingSyncPdosDropsOnlySyncFrames) {
  Rig r;
  r.port0.room = 0;
  CO_CANtx_t* sync_pdo = r.Tx(1, 0x285, 6, true);
  CO_CANtx_t* hb = r.Tx(3, 0x705, 1, false);
  CHECK_EQ(CO_CANsend(&r.module, sync_pdo), CO_ERROR_NO);
  CHECK_EQ(CO_CANsend(&r.module, hb), CO_ERROR_NO);
  CO_CANclearPendingSyncPDOs(&r.module);
  CHECK(!sync_pdo->bufferFull);
  CHECK(hb->bufferFull);
  CHECK_EQ(r.module.CANtxCount, 1U);
  CHECK((r.module.CANerrorStatus & CO_CAN_ERRTX_PDO_LATE) != 0U);
  r.port0.room = -1;
  CO_CANmodule_process(&r.module);
  REQUIRE(r.port0.sent.size() == 1U);
  CHECK_EQ(r.port0.sent[0].id, 0x705U);
}

HF_TEST(Tx_DetachDropsThatPortsPendingFrames) {
  Rig r;
  r.port0.room = 0;
  CO_CANtx_t* b = r.Tx(0, 0x185, 1);
  CHECK_EQ(CO_CANsend(&r.module, b), CO_ERROR_NO);
  CHECK(b->bufferFull);
  r.bind0.Detach();
  CHECK(!b->bufferFull);
  CHECK_EQ(r.module.CANtxCount, 0U);
}

HF_TEST(Rx_DispatchesThroughTheRingByIdentMaskAndRtr) {
  Rig r;
  CHECK_EQ(CO_CANrxBufferInit(&r.module, 0, 0x000, 0x7FF, false, &g_obj_a, &RxLog::Callback), CO_ERROR_NO);
  CHECK_EQ(CO_CANrxBufferInit(&r.module, 1, 0x700, 0x780, false, &g_obj_b, &RxLog::Callback), CO_ERROR_NO);
  CHECK(r.port0.Inject(0x000, {0x01, 0x05}));
  CHECK(r.port1.Inject(0x70A, {0x05}));
  CHECK(r.port0.Inject(0x70B, {}, HF_CO_FRAME_RTR));  // RTR does not match a data-frame consumer
  CHECK(r.port0.Inject(0x123, {1, 2, 3}));             // no consumer
  CHECK(r.port0.Inject(0x1234567, {1}, HF_CO_FRAME_EXT));
  CHECK_EQ(hf_co_driver_rx_pending(r.drv), 5U);
  CHECK(RxLog::Get().empty());  // nothing runs in the producer's context

  CO_CANmodule_process(&r.module);
  CHECK_EQ(hf_co_driver_rx_pending(r.drv), 0U);
  REQUIRE(RxLog::Get().size() == 2U);
  const auto& log = RxLog::Get();
  bool saw_nmt = false;
  bool saw_hb = false;
  for (const auto& e : log) {
    if (e.object == &g_obj_a) {
      saw_nmt = e.ident == 0x000U && e.dlc == 2U && e.data[0] == 0x01U && e.data[1] == 0x05U;
    }
    if (e.object == &g_obj_b) {
      saw_hb = e.ident == 0x70AU && e.dlc == 1U && e.data[0] == 0x05U && e.data[1] == 0U;
    }
  }
  CHECK(saw_nmt);
  CHECK(saw_hb);
  CHECK_EQ(r.Stats(0).rx_unhandled, 3U);
  CHECK_EQ(r.Stats(0).rx_dispatched, 1U);
  CHECK_EQ(r.Stats(1).rx_dispatched, 1U);
}

HF_TEST(Rx_DiscardedUntilNormalMode) {
  Rig r;
  CHECK_EQ(CO_CANrxBufferInit(&r.module, 0, 0x000, 0x7FF, false, &g_obj_a, &RxLog::Callback), CO_ERROR_NO);
  r.module.CANnormal = false;
  CHECK(r.port0.Inject(0x000, {0x01, 0x00}));
  CO_CANmodule_process(&r.module);
  CHECK(RxLog::Get().empty());
  CHECK_EQ(hf_co_driver_rx_pending(r.drv), 0U);
}

HF_TEST(Rx_TooLongFramesAreRejectedAtPush) {
  Rig r;
  hf_co_frame_t f{};
  f.id = 0x181;
  f.len = 12;
  f.flags = HF_CO_FRAME_FDF;
  CHECK(!hf_co_driver_rx_push(r.drv, 0, &f));
  CHECK_EQ(r.Stats(0).rx_rejected, 1U);
}

HF_TEST(Rx_PolicyCanRefuseAConsumer) {
  Rig r;
  CHECK_EQ(CO_CANrxBufferInit(&r.module, 2, 0x6C5, 0x7FF, false, &g_obj_b, &RxLog::Callback), CO_ERROR_NO);
  hf_co_driver_set_policy(
      r.drv,
      [](void*, uint8_t port, uint16_t rx_index, const void* consumer, const hf_co_frame_t*) {
        return !(port == 0 && rx_index == 2 && consumer == &g_obj_b);
      },
      nullptr, nullptr);
  CHECK(r.port0.Inject(0x6C5, {0x40, 0x00, 0x10, 0x00}));
  CHECK(r.port1.Inject(0x6C5, {0x40, 0x00, 0x10, 0x00}));
  CO_CANmodule_process(&r.module);
  CHECK_EQ(RxLog::Get().size(), 1U);
  CHECK_EQ(r.Stats(0).rx_denied, 1U);
  CHECK_EQ(r.Stats(1).rx_dispatched, 1U);
}

HF_TEST(Rx_RingOverrunRaisesRxOverflowForOneCycle) {
  Rig r;
  CHECK_EQ(CO_CANrxBufferInit(&r.module, 0, 0x181, 0x7FF, false, &g_obj_a, &RxLog::Callback), CO_ERROR_NO);
  for (unsigned i = 0; i < HF_CO_RX_RING_LEN + 6U; ++i) {
    (void)r.port0.Inject(0x181, {static_cast<uint8_t>(i)});
  }
  CHECK_EQ(r.Stats(0).rx_ring_overruns, 6U);
  CO_CANmodule_process(&r.module);
  CHECK_EQ(RxLog::Get().size(), HF_CO_RX_RING_LEN);
  CHECK((r.module.CANerrorStatus & CO_CAN_ERRRX_OVERFLOW) != 0U);
  CO_CANmodule_process(&r.module);  // still visible to the next CO_EM_process
  CHECK((r.module.CANerrorStatus & CO_CAN_ERRRX_OVERFLOW) != 0U);
  CO_CANmodule_process(&r.module);
  CHECK((r.module.CANerrorStatus & CO_CAN_ERRRX_OVERFLOW) == 0U);
}

HF_TEST(Rx_OfferDoesNotCountOverruns) {
  Rig r;
  hf_co_frame_t f{};
  f.id = 0x181;
  f.len = 1;
  for (unsigned i = 0; i < HF_CO_RX_RING_LEN; ++i) {
    CHECK(hf_co_driver_rx_offer(r.drv, 0, &f));
  }
  CHECK(!hf_co_driver_rx_offer(r.drv, 0, &f));
  CHECK_EQ(r.Stats(0).rx_ring_overruns, 0U);
}

HF_TEST(Error_CountersMapToCanErrorStatus) {
  Rig r;
  r.port0.status.state = BusState::ErrorWarning;
  r.port0.status.tec = 100;
  CO_CANmodule_process(&r.module);
  CHECK_EQ(r.module.CANerrorStatus & 0x0FFFU, CO_CAN_ERRTX_WARNING);

  r.port0.status.state = BusState::ErrorPassive;
  r.port0.status.tec = 130;
  r.port0.status.rec = 97;
  CO_CANmodule_process(&r.module);
  CHECK_EQ(r.module.CANerrorStatus,
           static_cast<uint16_t>(CO_CAN_ERRTX_WARNING | CO_CAN_ERRTX_PASSIVE | CO_CAN_ERRRX_WARNING));

  r.port0.status.tec = 0;
  r.port0.status.rec = 128;
  CO_CANmodule_process(&r.module);
  CHECK_EQ(r.module.CANerrorStatus, static_cast<uint16_t>(CO_CAN_ERRRX_WARNING | CO_CAN_ERRRX_PASSIVE));

  r.port0.status = PortStatus{};
  r.port0.status.state = BusState::ErrorActive;
  CO_CANmodule_process(&r.module);
  CHECK_EQ(r.module.CANerrorStatus, 0U);

  // Port 1 is not an error source: its counters do not count.
  r.port1.status.state = BusState::BusOff;
  r.port1.status.tec = 256;
  CO_CANmodule_process(&r.module);
  CHECK_EQ(r.module.CANerrorStatus, 0U);
  CHECK_EQ(r.port1.recover_calls, 0);
}

HF_TEST(Error_StateOnlyPortsStillReport) {
  Rig r;
  r.port0.status.state = BusState::ErrorPassive;
  CO_CANmodule_process(&r.module);
  CHECK((r.module.CANerrorStatus & CO_CAN_ERRTX_PASSIVE) != 0U);
}

HF_TEST(Error_BusOffRequestsRecoveryAndRepeats) {
  Rig r;
  r.port0.status.state = BusState::BusOff;
  CO_CANmodule_process(&r.module);
  CHECK((r.module.CANerrorStatus & CO_CAN_ERRTX_BUS_OFF) != 0U);
  CHECK_EQ(r.port0.recover_calls, 1);
  for (unsigned i = 0; i < HF_CO_BUSOFF_RETRY_CALLS; ++i) {
    CO_CANmodule_process(&r.module);
  }
  CHECK_EQ(r.port0.recover_calls, 2);
  CHECK_EQ(r.Stats(0).recover_requests, 2U);

  r.port0.status.state = BusState::ErrorActive;
  CO_CANmodule_process(&r.module);
  CHECK((r.module.CANerrorStatus & CO_CAN_ERRTX_BUS_OFF) == 0U);
  CHECK_EQ(r.port0.recover_calls, 2);
}

HF_TEST(Error_TransientBitsSurviveOneProcessCall) {
  Rig r;
  r.port0.room = 0;
  CO_CANtx_t* sync_pdo = r.Tx(1, 0x285, 6, true);
  CHECK_EQ(CO_CANsend(&r.module, sync_pdo), CO_ERROR_NO);
  CO_CANclearPendingSyncPDOs(&r.module);
  CO_CANmodule_process(&r.module);  // CO_process(): CO_EM_process runs after this and sees the bit
  CHECK((r.module.CANerrorStatus & CO_CAN_ERRTX_PDO_LATE) != 0U);
  CO_CANmodule_process(&r.module);
  CHECK((r.module.CANerrorStatus & CO_CAN_ERRTX_PDO_LATE) == 0U);
}

HF_TEST(Wake_CalledOnPushAndTxDone) {
  Rig r;
  static int wakes = 0;
  wakes = 0;
  hf_co_driver_set_wake(r.drv, [](void* ctx) { ++*static_cast<int*>(ctx); }, &wakes);
  CHECK(r.port0.Inject(0x181, {1}));
  hf_co_driver_tx_done(r.drv, 1);
  CHECK_EQ(wakes, 2);
}

HF_TEST(Lock_IsRecursive) {
  Rig r;
  CO_LOCK_OD(&r.module);
  CO_LOCK_CAN_SEND(&r.module);
  CHECK_EQ(CO_CANsend(&r.module, r.Tx(0, 0x185, 1)), CO_ERROR_NO);  // takes the lock again
  CO_UNLOCK_CAN_SEND(&r.module);
  CO_UNLOCK_OD(&r.module);
  CHECK_EQ(r.port0.sent.size(), 1U);
}
