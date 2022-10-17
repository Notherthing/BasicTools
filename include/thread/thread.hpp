#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <pthread.h>
#include <stdint.h>

#include <condition_variable>
#include <cstring>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>

namespace neos {

namespace Thread {
using thread_handle = std::thread;
using mutex = std::mutex;
using shared_mutex = std::shared_mutex;
using recursive_mutex = std::recursive_mutex;
using condition_variable = std::condition_variable;
using condition_variable_any = std::condition_variable_any;
template <typename T>
using atomic = std::atomic<T>;
template <typename MTX>
using lock_guard = std::lock_guard<MTX>;
template <typename MTX>
using unique_lock = std::unique_lock<MTX>;
/*
brief : set thread priority.
params : th : thread_handle, policy : thread schedule, priority : thread
priority return : true - set successs , false - set failed
 */
bool SetScheduling(thread_handle& th, int priority, int policy = SCHED_RR);
int GetScheduling(thread_handle& th);
bool SetCpuid(thread_handle& th, int cpuid);
int GetCpuid(thread_handle& th);

class ManualThread {
  mutex state_mutex;
  enum { INITIAL, STARTED, ABORTED } state_;
  condition_variable state_condvar;
  thread_handle impl_;
  int cpuid_ = -1;
  int priority_ = -1;

 public:
  template <class F, class... Args>
  explicit ManualThread(F&& f, Args&&... args)
      : state_mutex(),
        state_(INITIAL),
        state_condvar(),
        impl_([&state_mutex = state_mutex, &state_ = state_,
               &state_condvar = state_condvar,
               routine = std::bind(std::forward<F>(f),
                                   std::forward<Args>(args)...)]() {
          {
            std::unique_lock state_mutex_lock(state_mutex);
            state_condvar.wait(state_mutex_lock,
                               [&state_]() { return state_ != INITIAL; });
          }
          if (state_ == STARTED) routine();
        }) {}
  void Start();
  ~ManualThread();
  bool SetCpuid(int cpuid);
  bool SetScheduling(int priority);
  int get_cpuid() noexcept {
    std::lock_guard state_mutex_lock(state_mutex);
    return cpuid_;
  }
  int get_priority() noexcept {
    std::lock_guard state_mutex_lock(state_mutex);
    return priority_;
  }
  void Join() { impl_.join(); }
  // void Detach() { impl_.detach(); }
};
using manual_thread = ManualThread;

class SuspendedThread {
 private:
  std::promise<bool> starter;
  std::thread impl_;
  int cpuid_ = -1;
  int priority_ = -1;

 public:
  template <class F, class... Args>
  explicit SuspendedThread(F&& f, Args&&... args)
      : starter(),
        impl_([starter_future = starter.get_future(),
               routine = std::bind(std::forward<F>(f),
                                   std::forward<Args>(args)...)]() mutable {
          if (starter_future.get()) routine();
        }) {}
  void start() {
    starter.set_value(true);
    // impl_.join();
  }
  ~SuspendedThread() {
    try {
      starter.set_value(false);
    } catch (const std::future_error& exc) {
      if (exc.code() != std::future_errc::promise_already_satisfied) {
        std::cerr << "exc.code() != std::future_errc::promise_already_satisfied"
                  << std::strerror(errno) << std::endl;
      }
      return;  // already “started”, no need to do anything
    }
    impl_.join();  // if thread has not started, auto join for gc
  }
  bool SetCpuid(int cpuid);
  bool SetScheduling(int priority);
  int get_cpuid() noexcept { return cpuid_; }
  int get_priority() noexcept { return priority_; }
  void Join() { impl_.join(); }
};
using suspend_thread = SuspendedThread;
}  // namespace Thread
}  // namespace neos
#endif