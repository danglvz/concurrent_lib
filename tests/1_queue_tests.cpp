#include <gtest/gtest.h>
#include <thread>
#include "queues.h"

using namespace concurrent_lib::queue;

TEST(queue_tests, simple_lock_free_test_1){
    with_locks<int> queue{};
    std::vector<int> expect_results{1,2,3,4,5,6};

    for (auto &i: expect_results){
        queue.push(i);
    }
    
    for (auto &i: expect_results){
        int temp;
        queue.try_pop(temp);
        ASSERT_EQ(i, temp);
    }
}

TEST(queu_tests, simple_lock_free_thread_test){
    with_locks<int> queue{};
    std::vector<int> expect_results{1,2,3,4,5}, output;

    std::atomic_bool done{false};

    std::thread popper_thread([&](){ 
        int temp{};
        while(!done){
            if (queue.try_pop(temp)){
                output.emplace_back(temp);
            }
        }
     });

    std::this_thread::sleep_for(std::chrono::seconds(1));

    for ( auto &i: expect_results ){
            queue.push(i);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    done = true;
    popper_thread.join();

    ASSERT_EQ(expect_results, output);
}

