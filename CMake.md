# CMake

`CMakePresets.json` provides some handy defaults:

- Release: build the libraries and executables for distribution (defaults, no debug info)
- Debug: build with debug information.
- Coverage: build with debug & coverage information.
- DocumentationOnly: only build doxygen documentation.
- UnitTests: build with unit testing enabled.

## options

- BQPP_CODE_COVERAGE: enable code coverage report generation
- BQPP_TESTS: enable testing
- BQPP_BUILD: build main targets (src)
- BQPP_DOCS: generate documentation from repository with doxygen

- `CMakeLists.txt` BQPP_BUILD: __ON__
        Master on/off switch for building any/all of the bqpp source code. Useful if one only wants to build documentation or resources.
  - BQPP_BUILD_STD: __ON__
        Turns on/off the building of `libbqStd`.
  - BQPP_BUILD_SDL: __OFF__
        etc...
  - BQPP_BUILD_DUI: __OFF__
  - BQPP_BUILD_DUI_SDL: __OFF__
        ___NOTE:___ ABSTRACT.
  - BQPP_BUILD_DUI_SDL_RENDERER: __OFF__

- BQPP_TESTS: __OFF__
        Enables/disables any testing, unit or demo (integration).
  - BQPP_UNIT_TESTS: __OFF__
          Enables/disables Google Unit Tests.
  - BQPP_DEMO_TESTS: __OFF__
          Enable/disables demo tests.
