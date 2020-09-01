#include <gtest/gtest.h>
#include <map>
#include "concurrent_hashMap.h"


TEST(hashmap_tests, simple_test){
    using namespace concurrent_lib::hash_map;
    std::vector<int> expect_values{111,222,333};
    std::vector<int> keys{7,5,6};
    map_with_locks<int, int> hm(10);
    for ( size_t i{}; i < keys.size(); ++i ){
        hm.setValue(keys[i], expect_values[i]);
    }

    int value_{};


    for(size_t i{}; i < keys.size(); ++i){
        hm.getValue(keys[i],value_);
        ASSERT_EQ(expect_values[i], value_);
    }

    std::map<int,int> expect_vec{{7,111},{5,222},{6,333}}, result;
    for ( auto &item: hm.getVectorAndClear()){
        result.insert(item);
    }
    ASSERT_EQ(expect_vec, result);

    ASSERT_FALSE(hm.getValue(keys[0], value_));
}