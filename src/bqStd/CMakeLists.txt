option(BQPP_BUILD_STD "build libbqStd" ON)

message("${PROJECT_NAME} configuring bqStd in ${CMAKE_BINARY_DIR}")
set(BQ_STD_INCLUDE_DIR "${BQ_INCLUDE_DIR}/bq")
message("BQ_STD_INCLUDE_DIR: ${BQ_STD_INCLUDE_DIR}")
make_directory("${BQ_STD_INCLUDE_DIR}")
add_library(bqStd)
target_include_directories(bqStd PUBLIC "${BQ_INCLUDE_DIR}")


function(BQ_STD_INCLUDE include)
  message("${PROJECT_NAME} bqStd include file ${include}")
  list(APPEND BQ_STD_INCLUDES "${BQ_STD_INCLUDE_DIR}/${include}")
  configure_file("${include}" "${BQ_STD_INCLUDE_DIR}/${include}" COPYONLY)
  target_sources(bqStd PUBLIC "${BQ_STD_INCLUDE_DIR}/${include}")
endfunction()


configure_file(build.hpp.in "${BQ_STD_INCLUDE_DIR}/build.hpp")
#configure_file(SharedResource.hpp "${BQ_STD_INCLUDE_DIR}/SharedResource.hpp")
BQ_STD_INCLUDE(SharedResource.hpp)
BQ_STD_INCLUDE(std.hpp)
target_sources(bqStd PUBLIC
  std.cpp
)
add_library(bq::std ALIAS bqStd)


