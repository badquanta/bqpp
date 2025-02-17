message("${PROJECT_NAME} configuring ${CMAKE_CURRENT_SOURCE_DIR}")
add_subdirectory("bqStd")


if(BQPP_SDL)
  add_subdirectory("bqSdl")
else()
  message("${PROJECT_NAME} skipping bqSdl")
endif()
add_subdirectory("bqDui")


if(BQPP_BUILD_DEMOS)
  add_subdirectory("demo")
endif()

