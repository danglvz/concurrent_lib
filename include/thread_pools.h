#include <queue>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>



namespace concurrent_lib::thread_pools{
    class no_futeres_with_locks{
        std::queue<std::function<void()>> tasks;
        std::vector<std::thread> threads;
        std::atomic_bool done;
        std::condition_variable cond;
        mutable std::mutex mut;

    public:
        explicit no_futeres_with_locks(unsigned int num_of_threads);
        ~no_futeres_with_locks();
        void submit(std::function<void()>);

    };
}