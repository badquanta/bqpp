message("${PROJECT_NAME} configuring ${CMAKE_CURRENT_SOURCE_DIR}")
add_subdirectory("bqStd")
add_subdirectory("bqSdl")
add_subdirectory("bqDui")
option("BQPP_BUILD_DEMOS" "Build demos" OFF)
if(BQPP_BUILD_DEMOS)
  add_subdirectory("demo")
endif()

