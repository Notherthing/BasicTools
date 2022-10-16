#include "../../include/thread/thread.hpp"

namespace neos {
bool Thread::SetScheduling(std::thread &th, int policy, int priority) {
  sched_param sch_params;
  sch_params.sched_priority = priority;
  if (!pthread_setschedparam(th.native_handle(), policy, &sch_params)) {
    return true;
  }
  std::cerr << "Failed to set Thread scheduling :" << std::strerror(errno)
            << std::endl;

  return false;
}

bool Thread::SetCpuid(std::thread &th, int cpuid) {
  cpu_set_t cpu_set;
  CPU_ZERO(&cpu_set);
  CPU_SET(cpuid, &cpu_set);
  if (!pthread_setaffinity_np(th.native_handle(), sizeof(cpu_set_t),
                              &cpu_set)) {
    return true;
  }
  std::cerr << "Failed to set Thread cpu setaffinity :" << std::strerror(errno)
            << std::endl;
  return false;
}

int Thread::GetCpuid(std::thread &th) {
  cpu_set_t cpu_set;
  CPU_ZERO(&cpu_set);
  if (!pthread_getaffinity_np(th.native_handle(), sizeof(cpu_set_t),
                              &cpu_set)) {
    for (int i = 0; i < CPU_SETSIZE; ++i) {
      if (CPU_ISSET(i, &cpu_set)) {
        return i;
      }
    }
  }
  return -1;
}

int Thread::GetScheduling(thread_handle &th) {
  sched_param sch_params;
  int policy = -1;
  if (!pthread_getschedparam(th.native_handle(), &policy, &sch_params)) {
    return sch_params.sched_priority;
  }
  std::cerr << "Failed to set Thread scheduling :" << std::strerror(errno)
            << std::endl;

  return -1;
}
}  // namespace neos