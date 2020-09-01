
#include <iostream>
#include <algorithm>
#include <list>
#include <cassert>
#include <vector>
#include <functional>
#include <memory>
#include <shared_mutex>
#include <thread>
#include <queue>


namespace concurrent_lib::hash_map{

    template<typename Key, typename Value>
    class map_with_locks{
        class bucket{
            using bucket_value = std::pair<Key, Value>;
            using bucket_data = std::list<bucket_value>;
            using bucket_iterator = typename bucket_data::iterator;


            bucket_iterator find( const Key &key ){
                return std::find_if(data.begin(), data.end(),
                                    [&]( const bucket_value &pair ){
                                        return pair.first == key;
                                    });
            }

        public:

            mutable std::shared_mutex s_mut;

            bucket_data data;

            void setValue(const Key &key, const Value &val){
                std::unique_lock<std::shared_mutex> ul(s_mut);
                auto it = find(key);
                if ( it == data.end()) {
                    data.emplace_back(key, val);
                } else {
                    it->second = val;
                }
            }

            bool getValue( const Key &key, Value &val ){
                std::shared_lock<std::shared_mutex> sl(s_mut);
                auto it = find(key);
                if ( it == data.end() ){
                    return false;
                } else {
                    val = it->second;
                    return true;
                }
            }

            void removeValue( const Key &key ){
                std::unique_lock<std::shared_mutex> ul(s_mut);
                auto it = find(key);
                if ( it != data.end() ){
                    data.erase(it);
                }
            }

        };
        using Hash = std::hash<Key>;
        std::vector<std::unique_ptr<bucket>> buckets;
        size_t num_of_buckets;
        Hash hasher;


        bucket &get_bucket(const Key &key){
            size_t num{ hasher(key)%num_of_buckets };
            return *buckets[num];
        }


    public:
        explicit map_with_locks( size_t num_buckets): num_of_buckets(num_buckets), hasher{}{
            for ( size_t i{}; i < num_buckets; ++i ){
                buckets.emplace_back(new bucket());
            }
        }
        void setValue(const Key &key, const Value &val){
            get_bucket(key).setValue(key,val);
        }

        bool getValue( const Key &key, Value &val ){
            return get_bucket(key).getValue(key, val);
        }

        void removeValue( const Key &key ){
            get_bucket(key).removeValue(key);
        }

        void removeAll(){
            for ( auto &bucket_: buckets ){
                bucket_->data.clear();
            }
        }

        std::vector<std::pair<Key, Value>> getVectorAndClear( ){
            std::vector<std::pair<Key, Value>> output;
            std::vector<std::unique_lock<std::shared_mutex>> u_locks;

            for ( size_t num{}; num < buckets.size(); ++num ){
                u_locks.emplace_back(buckets[num]->s_mut);
            }

            for (size_t num{}; num < buckets.size(); ++num){
                for (auto &pair:buckets[num]->data){
                    output.emplace_back(std::move(pair));
                }
                buckets[num]->data.clear();
            }

            return output;
        }


    };

}