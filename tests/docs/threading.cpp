// Threading snippets for docs/threading.md. Compiled by the host build (target hf_core_doc_snippets).

#include <cstdint>

#include "BaseThread.h"
#include "RtosMutex.h"
#include "SfmHandler.h"

// --8<-- [start:shared-bus]
// Two flow meters on one I2C bus: give both handlers the same mutex so a
// read on one never interleaves with a transaction on the other.
struct FlowMeters {
  FlowMeters(BaseI2c& inlet_dev, BaseI2c& outlet_dev)
      : inlet(inlet_dev, SfmHandlerConfig{}, &bus_mutex),
        outlet(outlet_dev, SfmHandlerConfig{}, &bus_mutex) {}

  RtosMutex bus_mutex;  // declared first: constructed before the handlers
  SfmHandler inlet;
  SfmHandler outlet;
};
// --8<-- [end:shared-bus]

// --8<-- [start:own-lock]
class Counter {
 public:
  void Add(uint32_t n) noexcept {
    MutexLockGuard lock(mutex_);   // released on every return path
    total_ += n;
  }
  uint32_t Total() noexcept {
    MutexLockGuard lock(mutex_, 10);  // wait at most 10 ms
    return lock.IsLocked() ? total_ : 0;
  }

 private:
  RtosMutex mutex_;  // recursive: a locked method may call another one
  uint32_t total_{0};
};
// --8<-- [end:own-lock]

// --8<-- [start:thread]
class FlowTask : public BaseThread {
 public:
  explicit FlowTask(SfmHandler& meter) noexcept : BaseThread("flow"), meter_(meter) {}

 protected:
  bool Initialize() noexcept override {  // creates the task on a static stack
    return CreateBaseThread(stack_, sizeof(stack_), 5, 5, 0, OS_AUTO_START);
  }
  bool ResetVariables() noexcept override { samples_ = 0; return true; }
  bool Setup() noexcept override { return meter_.EnsureInitialized(); }
  uint32_t Step() noexcept override {
    if (meter_.ReadMeasurement()) {
      ++samples_;
    }
    return 100;  // ms until the next Step()
  }
  bool Cleanup() noexcept override { return true; }

 private:
  SfmHandler& meter_;
  uint32_t samples_{0};
  uint8_t stack_[4096]{};
};

// Start it from the code that owns the handler:
//   FlowTask task(meter);
//   task.EnsureInitialized() && task.StartThreadAndWaitToVerify(1000);
// --8<-- [end:thread]
