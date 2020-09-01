add_library(concurrent_lib "")

find_package(Threads REQUIRED)




file(GLOB CONCURRENT_SRC_
        "*.h"
        "*.cpp")

target_sources(concurrent_lib PRIVATE ${CONCURRENT_SRC_} )
target_include_directories(concurrent_lib PUBLIC ${concurr_lib_SOURCE_DIR}/include)        
target_link_libraries(concurrent_lib PUBLIC ${CMAKE_THREAD_LIBS_INIT})

