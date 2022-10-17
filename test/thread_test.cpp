#include "../include/thread/thread.hpp"

#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;
void f1() {
  for (long long int i = 0; i < 1e10; ++i) {
    asm volatile("" ::: "memory");
    // cout << "" << endl;
    //  sleep(100);
  }
}
int main() {
  /*
neos::Thread::thread_handle th1([]() {
  cout << "th1" << endl;
  // sleep(1);
});
neos::Thread::SetScheduling(th1, SCHED_RR, 1);
neos::Thread::SetCpuid(th1, 7);
cout << neos::Thread::GetCpuid(th1) << endl;
cout << neos::Thread::GetScheduling(th1) << endl;
neos::Thread::atomic<int> aint = 0;
cout << aint.load(std::memory_order_relaxed) << endl;
th1.join();
*/
  /*
neos::Thread::manual_thread ist(f1);
    ist.SetCpuid(0);
    ist.Start();
    ist.Join();
    */

  neos::Thread::suspend_thread is(f1);
  is.SetCpuid(7);
  is.start();
  is.Join();
  /*
    neos::Thread::thread_handle th1([]() {
      for (int i = 0; i < 1e16; ++i) {
        asm volatile("" ::: "memory");
        //cout << "" << endl;
        // sleep(100);
      }// sleep(1);
    });
    neos::Thread::SetCpuid(th1, 7);
    th1.join();
    */
  return 0;
}