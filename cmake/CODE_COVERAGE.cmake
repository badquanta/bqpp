message("Hello from CODE_COVERAGE.cmake")

add_compile_options(
  -O0 # no optimization
  -g # generate debug info
  --coverage # sets all required flags
)
#target_link_options(${EXECUTABLE_NAME} INTERFACE --coverage)
add_link_options(--coverage)
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
