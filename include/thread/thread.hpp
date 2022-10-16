#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <pthread.h>
#include <stdint.h>

#include <condition_variable>
#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>

namespace neos {

using thread_handle = std::thread;

using mutex = std::mutex;
using recursive_mutex = std::recursive_mutex;
using condition_variable = std::condition_variable;
using condition_variable_any = std::condition_variable_any;
template <typename T>
using atomic = std::atomic<T>;

class Thread : public std::thread {
 public:
  /*
  brief : set thread priority.
  params : th : thread_handle, policy : thread schedule, priority : thread
  priority return : true - set successs , false - set failed
   */
  static bool SetScheduling(thread_handle& th, int policy, int priority);
  static int GetScheduling(thread_handle& th);
  static bool SetCpuid(thread_handle& th, int cpuid);
  static int GetCpuid(thread_handle& th);

 private:
};
}  // namespace neos

#endif