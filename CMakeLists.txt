cmake_minimum_required(VERSION 3.5)
project(bqpp)
# Set the project version
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
# set project meta
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
message("${PROJECT_NAME} option BUILD_BQPP: ${BUILD_BQPP}")
# enable/disable code coverage reporting
option(BQPP_CODE_COVERAGE "Enable coverage reporting" ON)
message("${PROJECT_NAME} option BUILD_CODE_COVERAGE: ${BQPP_CODE_COVERAGE}")
##############################################################################
# Code Coverage
##############################################################################
if(BQPP_CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  message("${PROJECT_NAME} Code coverage enabled")
  # Add required flags (GCC & LLVM/Clang)  
  #target_compile_options(${EXECUTABLE_NAME} INTERFACE
  add_compile_options(
    -O0 # no optimization
    -g # generate debug info
    --coverage # sets all required flags
  )
  #target_link_options(${EXECUTABLE_NAME} INTERFACE --coverage)
  add_link_options(--coverage)
  # Ensure that the testing option is set, and all test types are enable
  set(BUILD_TESTS ON)
  set(BUILD_EXAMPLES ON)
  set(BUILD_UNIT_TESTS ON)
  message("${PROJECT_NAME} example & unit testing enabled for code coverage")
  add_custom_command(OUTPUT coverage.info
    DEPENDS ${BQH_SDL_SOURCES}
    COMMAND lcov --capture --ignore-errors "inconsistent" --directory . --output-file coverage.info
    COMMAND lcov --remove coverage.info '/usr/*' --output-file coverage.info
    COMMAND lcov --remove coverage.info '*gtest*' --output-file coverage.info
  )
  add_custom_target(COVERAGE_REPORT
    DEPENDS coverage.info
    COMMAND lcov --list coverage.info
  )
  add_custom_target(COVERAGE_HTML
    DEPENDS coverage.info
    COMMAND genhtml coverage.info --ignore-errors "inconsistent" --legend --demangle-cpp -f -q --output-directory coverage
  )
  add_custom_target(OPEN_COVERAGE_HTML
    DEPENDS COVERAGE_HTML
    COMMAND ${CMAKE_COMMAND} -E chdir coverage xdg-open index.html
  )
  add_custom_target(CLEAN_COVERAGE
    COMMAND lcov --directory . --zerocounters
    COMMAND rm -f coverage.info
  )
else()
  message("${PROJECT_NAME} Code coverage disabled")
endif()
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
##############################################################################
# Documentation
##############################################################################
option(BQPP_DOCS "Build documentation" OFF)
message("${PROJECT_NAME} option BUILD_DOCS: ${BQPP_DOCS}")
set(BQPP_DOCS_MAIN_MD "${CMAKE_SOURCE_DIR}/README.md")
message("${PROJECT_NAME} option BUILD_DOCS_MAIN_MD: ${BQPP_DOCS_MAIN_MD}")
if(BQPP_DOCS)
  find_package(Doxygen REQUIRED dot)
  ####################################################### GENERAL SETTINGS
  set(DOXYGEN_QUIET NO)
  set(DOXYGEN_RECURSIVE YES)
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${BQPP_DOCS_MAIN_MD})
  ######################################################### SCOPE SETTINGS
  set(DOXYGEN_EXTRACT_ALL YES) # Everything
  set(DOXYGEN_JAVADOC_AUTOBRIEF YES) # brief is comment first line
  set(DOXYGEN_TAB_SIZE 2) # Tab size
  set(DOXYGEN_BUILTIN_STL_SUPPORT YES) # Support for std:: namespace
  set(DOXYGEN_TEMPLATE_RELATIONS NO) #
  set(DOXYGEN_REFERENCED_BY_RELATION YES) #
  set(DOXYGEN_REFERENCES_RELATION YES) #
  set(DOXYGEN_REFERENCES_LINK_SOURCE NO) #

  ########################################################## HTML SETTINGS
  set(DOXYGEN_GENERATE_HTML YES)
  set(DOXYGEN_SEPARATE_MEMBER_PAGES NO)
  set(DOXYGEN_TIMESTAMP YES)
  set(DOXYGEN_SOURCE_BROWSER YES)
  set(DOXYGEN_SOURCE_TOOLTIPS NO)
  set(DOXYGEN_HAVE_DOT YES)
  set(DOXYGEN_DOT_MULTI_TARGETS YES)
  set(DOXYGEN_DOT_CLEANUP YES)
  set(DOXYGEN_DOT_NUM_THREADS 4)
  set(DOXYGEN_UML_LOOK NO)
  set(DOXYGEN_STRIP_FROM_PATH ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})
  ########################################################## C++ Preprocessing
  set(DOXYGEN_ENABLE_PREPROCESSING YES)
  set(DOXYGEN_MACRO_EXPANSION YES)
  set(DOXYGEN_SKIP_FUNCTION_MACROS NO)
  set(DOXYGEN_EXPAND_ONLY_PREDEF NO)
  ##################################################################### Graphs
  set(DOXYGEN_DIRECTORY_GRAPH YES)
  set(DOXYGEN_CALL_GRAPH YES)
  set(DOXYGEN_CALLER_GRAPH YES)
  set(DOXYGEN_INHERIT_DOCS YES)
  set(DOXYGEN_HTML_DYNAMIC_SECTIONS NO)

  set(DOXYGEN_CODE_FOLDING NO)
  set(DOXYGEN_HIDE_UNDOC_RELATIONS NO)
  set(DOXYGEN_EXAMPLE_PATH ${CMAKE_SOURCE_DIR}/tests)
  doxygen_add_docs(
    docs
    ${BQPP_DOCS_MAIN_MD}
    "${CMAKE_SOURCE_DIR}/bqStd.md"
    "${CMAKE_SOURCE_DIR}/CMake.md"
    "${CMAKE_SOURCE_DIR}/CMakeLists.txt"
    "${CMAKE_SOURCE_DIR}/src"
    #"${CMAKE_SOURCE_DIR}/tests"
    ALL
    COMMENT "Generate documentation"
  )
endif()

