#include <gtest/gtest.h>
#include "thread_pools.h"
#include  <chrono>

using namespace concurrent_lib::thread_pools;


TEST(thread_pool_with_locks, destruct_test){
    no_futures_with_locks tp{200};
}

TEST(thread_pool_with_locks, fake_task_test){
    no_futures_with_locks tp{10};
    for (int i{}; i < 36; ++i){
        tp.submit([](){  std::this_thread::sleep_for(std::chrono::seconds(1));  });
    }
}


TEST(thread_pool_futures_with_locks, tasks_test){
    futures_with_locks pool(20);
    std::vector<int> expect_val;
    std::vector<std::future<int>> futures;
    for ( int i{}; i < 40; ++i ){
        futures.emplace_back(pool.submit_task([]( int dig ) -> int { return dig * 2;}, i));
        expect_val.emplace_back(i * 2);
    }

    for ( size_t i{}; i < futures.size(); ++i ){
        ASSERT_EQ(expect_val[i], futures[i].get());
    }


}