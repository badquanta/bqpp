# bqpp - BadQuanta's Prototype playground

My personal C++ playground.

## status of 0.1.1

Initial project repository start. See: [CHANGELOG](./CHANGELOG.md)

## roadmap

This roadmap needs to be fully fleshed out.

* 0.1 ... - Bq STD Stubbing.  `<-- We are here.`
* 0.2 ... - Basic SDL Integration.
* 0.2 ... - Basic ImGUI Integration.
* 0.3 ... - Basic integrated demos.
* 0.3.0 - OpenGL integration.
* 0.9.0 - Worthwhile demos.
* 1.0.0 - nebulous target, yet to be fully defined

## build

Configure cmake and build. @todo: test this.

    git clone <this-repo>
    mkdir some_build_dir && cd some_build_dir 
    cmake /path/to/bqpp && cmake build

For details about CMake configuration see `CMake.md`.  Major components can be optionally disabled, reducing the amount of external dependencies that might need to be fetched.

### testing

Ensure `BQPP_TESTS` cmake option is enabled, see CMake

### code coverage

Ensure `lcov` is installed.  If CMAKE option `BQPP_COVERAGE`, the build system will have targets `COVERAGE_REPORT`, `OPEN_COVERAGE_HTML`, & `CLEAN_COVERAGE`.  Any tests or executions will write collection information which can be assembled into an HTML report and the viewed.  Ensure `CLEAN_COVERAGE` is run between re-compilations or testing sessions. See [CMake.md](./CMake.md) for details about about enabling coverage.  

## repository contents

This repository contains libraries, build system specifications, user documentation, unit tests, and demos.

### libraries

Within `src` are the directories: `bqStd`, `bqSdl`, `bqDui`, etc.
These are libraries which the code is organized based on third party dependencies.

* `src/bqStd` - `bqStd` : Anything that depends only on the standard runtime libraries.
* `src/bqSdl` - `libbqSdl.md` : __comming soon__ Components using SDL
* `src/bqDui` - `libbqDui.md` : __comming soon__ Components using Dear ImGui, does not depend on SDL.
  * `src/bqDuiSdl` - `libbqDuiSdl.md` : __comming soon__ Backend coupling of Dear ImGui with SDL; but without a front-end.
    * `src/bqDuiSdlRenderer` - `libbqDuiSdlRenderer.md` : __comming soon__  Front-ent SDL coupling with SDL Renderer.
