option(BQPP_BUILD_STD "build libbqStd" ON)

message("${PROJECT_NAME} configuring bqStd in ${CMAKE_BINARY_DIR}")
set(BQ_STD_INCLUDE_DIR "${BQ_INCLUDE_DIR}/bq")
message("BQ_STD_INCLUDE_DIR: ${BQ_STD_INCLUDE_DIR}")
make_directory("${BQ_STD_INCLUDE_DIR}")
add_library(bqStd)
target_include_directories(bqStd PUBLIC "${BQ_INCLUDE_DIR}")
# 
#
function(BQ_STD_INCLUDE include)
  message("${PROJECT_NAME} bqStd include file ${include}")
  list(APPEND BQ_STD_INCLUDES "${BQ_STD_INCLUDE_DIR}/${include}")
  list(APPEND BQPP_INCLUDE_FILES "${BQ_STD_INCLUDE_DIR}/${include}")
  configure_file("${include}" "${BQ_STD_INCLUDE_DIR}/${include}" COPYONLY)
  target_sources(bqStd PUBLIC "${BQ_STD_INCLUDE_DIR}/${include}")
endfunction()
#
function(BQ_STD_INCLUDE_INPUT include)
  message("${PROJECT_NAME} bqStd include input file ${include}")
  list(APPEND BQ_STD_INCLUDES "${include}")
  list(APPEND BQPP_INCLUDE_FILES "${include}")
  configure_file("${include}.in" "${BQ_STD_INCLUDE_DIR}/${include}" COPYONLY)
  target_sources(bqStd PUBLIC "${BQ_STD_INCLUDE_DIR}/${include}")
endfunction()
#
function(BQ_STD_SOURCE source)
  message("${PROJECT_NAME} bqStd source file ${source}")
  list(APPEND BQ_STD_SOURCE_FILES "${source}")
  list(APPEND BQPP_SOURCE_FILES "${source}")
  target_sources(bqStd PUBLIC "${source}")
endfunction()
#configure_file(build.hpp.in "${BQ_STD_INCLUDE_DIR}/build.hpp")
#configure_file(SharedResource.hpp "${BQ_STD_INCLUDE_DIR}/SharedResource.hpp")
BQ_STD_INCLUDE_INPUT(build.hpp)
BQ_STD_INCLUDE(SharedResource.hpp)
BQ_STD_INCLUDE(std.hpp)
BQ_STD_SOURCE(std.cpp)
#target_sources(bqStd PUBLIC
#  std.cpp
#)
add_library(bq::std ALIAS bqStd)


