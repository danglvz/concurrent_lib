#include <queue>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>
#include "queues.h"


namespace concurrent_lib::thread_pools{
    class no_futures_with_locks{
        std::queue<std::function<void()>> tasks;
        std::vector<std::thread> threads;
        std::atomic_bool done;
        std::condition_variable cond;
        mutable std::mutex mut;

    public:
        explicit no_futures_with_locks(unsigned int num_of_threads);
        ~no_futures_with_locks();
        void submit(std::function<void()>);

    };

    class futures_with_locks : protected no_futures_with_locks{

    public:
        futures_with_locks(unsigned int num_threads) : no_futures_with_locks(num_threads){}

        template<typename F, typename... Args>
        auto submit_task(F&& func, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>{
            using return_type = typename std::result_of<F(Args...)>::type;

            auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(func), std::forward<Args>(args)...));

            std::future<return_type> res = task->get_future();

            submit([task](){  (*task)();  });

            return res;

        }
    };


    
}