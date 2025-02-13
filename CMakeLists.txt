cmake_minimum_required(VERSION 3.5)
project(bqpp)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message("${PROJECT_NAME} requires C++${CMAKE_CXX_STANDARD}")
# CMP0076: This policy determines how CMake handles target sources when using the `target_sources()` command.
# When this policy is set to NEW, CMake allows `target_sources()` to add sources to targets that are not yet defined.
# When this policy is set to OLD, `target_sources()` requires the target to be defined before adding sources to it.
cmake_policy(SET CMP0076 NEW)
# ARG_DOWNLOAD_EXTRACT_TIMESTAMP: This argument is used to specify the timestamp for the download and extract process.
# It ensures that the downloaded files are extracted based on the provided timestamp, which can be useful for caching
# This setting is used for version control purposes.
# Setting it to true enables version control features.
# Setting it to false disables version control features.
# and version control purposes.
set(ARG_DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
# Set the location where "HEADER" files will be assembled for installation.
set(BQ_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include")
# Ensure this include directory exists.
make_directory("${BQ_STD_INCLUDE_DIR}")
# enable/disable building of main targets
option(BQPP_BUILD "Build bqpp" ON)
message("${PROJECT_NAME} option BUILD_BQPP: ${BUILD_BQPP}")
#
if(BQPP_BUILD)
  message("${PROJECT_NAME} external dependencies....")
  add_subdirectory("ext")
  message("${PROJECT_NAME} src....")
  add_subdirectory("src")
endif()
# should control the building of any tests.
option(BQPP_TESTS "Build tests" OFF)
message("${PROJECT_NAME} option BUILD_TESTS: ${BQPP_TESTS}")
if(BQPP_TESTS)
  enable_testing()
  set(ARG_DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )
  FetchContent_MakeAvailable(googletest)
  include(GoogleTest)

  # GUT = Google Unit Tests
  function(add_gut aName)
    if(BQPP_TESTS)
      add_executable("${aName}" "${aName}.cpp")
      target_link_libraries("${aName}" PUBLIC bqStd GTest::gtest_main)
      target_include_directories("${aName}" PUBLIC ${CMAKE_BINARY_DIR}/include)
      gtest_discover_tests("${aName}" WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
      message("Unit test ${aName} defined.")
    else()
      #message("Skipping unit test ${KlassName}")
    endif()
  endfunction()
  add_subdirectory("tests")
else()
  message("${PROJECT_NAME} skipping tests")
endif()

#############################################################################
option(BQPP_DOCS "Build documentation" OFF)
option(BQPP_DOCS_MAIN_MD "Markdown file to use for main page" "${CMAKE_SOURCE_DIR}/README.md")
message("${PROJECT_NAME} option BUILD_DOCS: ${BUILD_DOCS}")
if(BQPP_DOCS)
  find_package(Doxygen REQUIRED dot)
  set(DOXYGEN_QUIET NO)
  #set(DOXYGEN_WARN_AS_ERROR YES)

  set(DOXYGEN_GENERATE_HTML YES)
  set(DOXYGEN_RECURSIVE YES)
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${BQPP_DOCS_MAIN_MD})
  set(DOXYGEN_EXTRACT_ALL YES)
  set(DOXYGEN_HAVE_DOT YES)
  set(DOXYGEN_DOT_MULTI_TARGETS YES)
  set(DOXYGEN_DOT_CLEANUP YES)
  set(DOXYGEN_DOT_NUM_THREADS 4)
  set(DOXYGEN_UML_LOOK NO)
  doxygen_add_docs(
    docs
    ${BQPP_DOCS_MAIN_MD}
    "${CMAKE_SOURCE_DIR}/CMake.md"
    "${CMAKE_SOURCE_DIR}/CMakeLists.txt"
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/tests"
    ALL
    COMMENT "Generate documentation"
  )
endif()

