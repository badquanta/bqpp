message("test bqStd")
enable_testing()
add_gut(testSharedResource)
target_link_libraries(testSharedResource PUBLIC bqStd)
