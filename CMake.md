# CMake

This repository uses CMake to configure build systems for this project.
`CMakePresets.json` provides handy defaults

## Presets

- Release
  binaryDir: ```./build```
  cacheVariables:
  - CMAKE_BUILD_TYPE: ```"Release"```
- ```DocumentationOnly```

## Options

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

### BUILD_BQ_DUI_SDL_RENDERER: __OFF__

Turns on/off the building of `libbqSdl`

## BUILD_TESTS: __ON__

Requires BUILD_BQPP __ON__. Controls building any tests (unit or integration).

## BUILD_UNIT_TESTS: __ON__

Requires BUILD_TESTS. Controls building of Google Unit Tests. Each set of unit tests may further depend on specific modules being built in order to be enabled.

## BUILD_INTEGRATION_TESTS: __OFF__

Requires BUILD_TESTS. Each integration test may require other build options to be enabled to be built.
