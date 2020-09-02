#include <iostream>
#include "thread_pools.h"


using namespace concurrent_lib::thread_pools;

no_futeres_with_locks::no_futeres_with_locks(unsigned int num_of_threads) {
    for (unsigned int num{}; num < num_of_threads; ++num){
        threads.emplace_back([this](){

            for (;;){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> ul(this->mut);
                    cond.wait(ul, [this](){  return !this->tasks.empty() || done ; });
                    if (done && this->tasks.empty()){
                        return;
                    } else {
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                }
                task();
            }
        });
    }
}

void no_futeres_with_locks::submit(std::function<void()> task){
    std::scoped_lock<std::mutex> sl(mut);
    tasks.push(task);
    cond.notify_one();
}

no_futeres_with_locks::~no_futeres_with_locks(){
    done = true;
    cond.notify_all();
    for ( auto &thread: threads ){
        thread.join();
    }
}



