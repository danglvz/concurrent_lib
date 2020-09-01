find_package(GTest REQUIRED) 
add_executable(concurr_lib_tests "")

file ( GLOB concurr_lib_tests_src_
        "*.cpp" )

target_sources(concurr_lib_tests PRIVATE ${concurr_lib_tests_src_} )        
target_include_directories(concurr_lib_tests PRIVATE 
                            ${concurr_lib_SOURCE_DIR}/include
                            ${GTEST_INCLUDE_DIRS} )


target_link_libraries(concurr_lib_tests PRIVATE 
                        ${GTEST_LIBRARIES} 
                        ${GTEST_MAIN_LIBRARIES}
                        concurrent_lib) 


add_test(concurr_lib_tests concurr_lib_tests )


