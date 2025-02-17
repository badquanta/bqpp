cmake_minimum_required(VERSION 3.5)
project(bqpp)
# Set the project version
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
# project meta info
set(PROJECT_NAME "bqpp")
set(PROJECT_DESCRIPTION "BadQuanta's C++ Prototyping Playground")
set(PROJECT_HOMEPAGE_URL "https://github.com/badquanta/bqpp")
# Project Settings & Policies
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message("${PROJECT_NAME} requires C++${CMAKE_CXX_STANDARD}")
##############################################################################
# Policies
##############################################################################
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
message("${PROJECT_NAME} ARG_DOWNLOAD_EXTRACT_TIMESTAMP: ${ARG_DOWNLOAD_EXTRACT_TIMESTAMP}")
##############################################################################
# Set the location where "HEADER" files will be assembled for installation.
set(BQ_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include")
message("${PROJECT_NAME} assembling includes within: ${BQ_INCLUDE_DIR}")
# Ensure this include directory exists.
make_directory("${BQ_STD_INCLUDE_DIR}")
##############################################################################
# Options
##############################################################################
# enable/disable building of main targets
option(BQPP_BUILD "Build bqpp" ON)
message("${PROJECT_NAME} option BQPP_BUILD: ${BQPP_BUILD}")
# enable/disable code coverage reporting
option(BQPP_CODE_COVERAGE "Enable coverage reporting" ON)
message("${PROJECT_NAME} option BQPP_CODE_COVERAGE: ${BQPP_CODE_COVERAGE}")
# enable/disable anything that depends on SDL3
option(BQPP_SDL "Enable SDL3" OFF)
message("${PROJECT_NAME} option BQPP_SDL: ${BQPP_SDL}")
# enable/disable building of demos
option("BQPP_BUILD_DEMOS" "Build demos" OFF)
message("${PROJECT_NAME} option BQPP_BUILD_DEMOS: ${BQPP_BUILD_DEMOS}")
# enable/disable unit tests
option(BQPP_TESTS "Build tests" OFF)
message("${PROJECT_NAME} option BUILD_TESTS: ${BQPP_TESTS}")
# enable/disable documentation
option(BQPP_DOCS "Build documentation" OFF)
message("${PROJECT_NAME} option BUILD_DOCS: ${BQPP_DOCS}")

##############################################################################
# Code Coverage
##############################################################################
if(BQPP_CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  message("${PROJECT_NAME} Code coverage enabled")
  include("./cmake/CODE_COVERAGE.cmake")
  # Add required flags (GCC & LLVM/Clang)  
  #target_compile_options(${EXECUTABLE_NAME} INTERFACE
else()
  message("${PROJECT_NAME} Code coverage disabled")
endif()

##############################################################################
# External Dependencies
#############################################################################
if(BQPP_BUILD)
  message("${PROJECT_NAME} external dependencies....")
  add_subdirectory("ext")
endif()



##############################################################################
# Main Targets
##############################################################################
# Collect ALL include files in this list.
list(APPEND BQPP_SOURCE_FILES "")
# Collect ALL source files in this list.
list(APPEND BQPP_INCLUDE_FILES "")
message("${PROJECT_NAME} src....")
#
add_subdirectory("src")

##############################################################################
# Unit Tests
##############################################################################
if(BQPP_TESTS)
  enable_testing()
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
##############################################################################
# Documentation
##############################################################################
set(BQPP_DOCS_MAIN_MD "${CMAKE_SOURCE_DIR}/README.md")
message("${PROJECT_NAME} option BQPP_DOCS_MAIN_MD: ${BQPP_DOCS_MAIN_MD}")
if(BQPP_DOCS)
  # Find Doxygen, and ensure the dot tool is available.
  find_package(Doxygen REQUIRED dot)
  # Include the doxygen options, they are verbose.
  include(cmake/DOXYGEN_OPTIONS.cmake)
  # Add the documentation target
  doxygen_add_docs(
    docs
    ${BQPP_DOCS_MAIN_MD}
    "${CMAKE_SOURCE_DIR}/CHANGELOG.md"
    "${CMAKE_SOURCE_DIR}/bqStd.md"
    "${CMAKE_SOURCE_DIR}/CMake.md"
    "${CMAKE_SOURCE_DIR}/CMakeLists.txt"
    "${CMAKE_BINARY_DIR}/include"
    "${CMAKE_SOURCE_DIR}/src"
    ${BQPP_SOURCE_FILES}0
    #"${CMAKE_SOURCE_DIR}/tests"
    ALL
    COMMENT "Generate documentation"
  )
  add_custom_target(clean-docs COMMAND rm -rf html WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/)
endif()

