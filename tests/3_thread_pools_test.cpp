#include <gtest/gtest.h>
#include "thread_pools.h"
#include  <chrono>

using namespace concurrent_lib::thread_pools;


TEST(thread_pool_with_locks, destruct_test){
    no_futeres_with_locks tp{200};
}

TEST(thread_pool_with_locks, fake_task_test){
    no_futeres_with_locks tp{10};
    for (int i{}; i < 36; ++i){
        tp.submit([](){  std::this_thread::sleep_for(std::chrono::seconds(1));  });
    }
}