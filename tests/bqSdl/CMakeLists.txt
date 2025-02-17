message("${PROJECT_NAME} configuring bqSdl tests")
add_gut(testWindow)
target_link_libraries(testWindow PUBLIC bqSdl)
