# concurrent_lib
This is my implementations of some concurrent data structures and thread managements written for my projects 

### Usage:
thread pools:
```cpp
#include "thread_pools.h"
//initiate thread pool that doesn't return a future object:
concurrent_lib::thread_pools::no_future_with_locks pool(/*num of threads*/);

//add "void()" task to pool,  simple example:
pool.submit([]() {  std::cout << "task complited" << std::endl;   });

//initiate thread pool that returns future object:
concurrent_lib::thread_pools::futures_with_locks pool_2(/* num of threads */);

// add task to pool:
std::future</*return_value*/> pool_2.submit_task(/*task*/, /*arguments*/);
//simple example:
std::future<int> pool_2.submit_task([](  int val  ) -> int {  return val * 2;   },  7);
```
thread-safe hash map:
```cpp
#include "concurrent_hashMap.h" 

concurrent_lib::hash_map::map_with_locks</*type_of_key*/, /*type_of_value*/>   h_map(/*number of buckets*/);

//add elements to map:
h_map.setValue(key, value);

//get element form map:
/*value_type*/ output;
h_map.getValue(key, output);

//remove elemet
h_map.removeValue(key);

//get vector of elements from map
std::vector<std::pair</*key_type*/, /*value_type*/>> output(  h_map.getVector()  );

//clear map
h_map.removeAll();
```
simple thread-safe queue based on locks:
```cpp
#include "queues.h"

concurrent_lib::queue::with_locks<type_of_elements> queue{};

//push element to queue:
queue.push(/*type_of_element*/);

//wait if queue is empty and pop from queue:
/*type_of_element*/ output;
queue.wait_and_pop(output);

//try pop from queue ,if queue is empty returns false
/*type_of_element*/ output;
bool isPopped = queue.try_pop(output);
```

