#ifndef CONCURRENT_LIB_QUEUES_H
#define CONCURRENT_LIB_QUEUES_H

#include <memory>
#include <queue>
#include <atomic>
#include <condition_variable>
#include <mutex>

namespace concurrent_lib::queue{



    template<typename TYPE>
    class with_locks {
        std::queue<TYPE> queue;
        std::mutex mut;
        std::condition_variable cond;
    public:
        bool empty() {
            std::scoped_lock<std::mutex> sl(mut);
            return queue.empty();
        }

        void push(TYPE item) {
            std::scoped_lock<std::mutex> sl(mut);
            queue.push(std::move(item));
            cond.notify_one();
        }

        void wait_and_pop(TYPE &output) {
            std::unique_lock<std::mutex> ul(mut);
            cond.wait(ul, [this](){  return !this->queue.empty(); });
            output = std::move(queue.front());
            queue.pop();
        }

        bool try_pop(TYPE &output){
            std::scoped_lock<std::mutex> sl(mut);
            if ( !queue.empty() ) {
                output = std::move(queue.front());
                queue.pop();
                return true;
            }
            else {
                return false;
            }
        }
    };


    template<typename T>
    class simple_lock_free{
        struct node{
            std::shared_ptr<T> data;
            node *next;
            node() : next(nullptr) {}
        };

        std::atomic<node*> head, tail;

        node *pop_head(){
            node* const old_head{head.load() };
            if ( old_head == tail.load() ){
                return nullptr;
            }

            head.store(old_head->next);
            return old_head;    
        }

    public:
        explicit simple_lock_free() : head(new node), tail(head.load()){}

        ~simple_lock_free() {
            while (node* const old_head = head.load())
            {
                head.store(old_head->next);
                delete old_head;
            }
        }

        std::shared_ptr<T> pop(){
            node *old_head = pop_head();
            if (!old_head){
                return std::shared_ptr<T>();
            }

            std::shared_ptr<T> const output(old_head->data);
            delete old_head;
            return output; 
        }

        void push (T new_value){
            std::shared_ptr<T> new_data(std::make_shared<T>(new_value));
            node *p = new node;
            node* const old_tail = tail.load();
            old_tail->data.swap(new_data);
            old_tail->next = p;
            tail.store(p); 
        }

        bool empty(){
            if ( head.load() == tail.load() ) {
                return true;
            } else {
                return true;
            }
        }
    };
}









#endif



