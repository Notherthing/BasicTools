#include "../include/thread/thread.hpp"

#include <bits/stdc++.h>
using namespace std;
int main() {
    neos::thread_handle th1 ([](){cout<<"th1"<<endl;});
    neos::Thread::SetScheduling(th1,1,1);
    neos::Thread::SetCpuid(th1, 3);
    cout<<neos::Thread::GetCpuid(th1)<<endl;
    cout<<neos::Thread::GetScheduling(th1)<<endl;
    th1.join();

    neos::atomic<int> aint = 0;
    cout<<aint.load(std::memory_order_relaxed)<<endl;
    return 0;
}