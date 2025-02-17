message("${PROJECT_NAME} configuring bqSdl")
set(BQ_SDL_INCLUDE_DIR "${BQ_INCLUDE_DIR}/bq")
message("BQ_SDL_INCLUDE_DIR: ${BQ_INCLUDE_DIR}/bqSdl")
make_directory("${BQ_SDL_INCLUDE_DIR}")



function(BQ_SDL_INCLUDE include)
  message("${PROJECT_NAME} bqSdl include file ${include}")
  list(APPEND BQ_SDL_INCLUDES "${BQ_SDL_INCLUDE_DIR}/${include}")
  list(APPEND BQPP_INCLUDE_FILES "${BQ_SDL_INCLUDE_DIR}/${include}")
  configure_file("${include}" "${BQ_SDL_INCLUDE_DIR}/${include}" COPYONLY)
  target_sources(bqSdl PUBLIC "${BQ_SDL_INCLUDE_DIR}/${include}")
endfunction()

function(BQ_SDL_SOURCE source)
  message("${PROJECT_NAME} bqSdl source file ${source}")
  list(APPEND BQ_SDL_SOURCE_FILES "${source}")
  list(APPEND BQPP_SOURCE_FILES "${source}")
  target_sources(bqSdl PUBLIC "${source}")
endfunction()

add_library(bqSdl)
target_link_libraries(bqSdl PUBLIC bq::std SDL3-shared SDL3_image-shared SDL3_ttf-shared SDL3_mixer-shared)
add_library(bq::sdl ALIAS bqSdl)
target_include_directories(bqSdl PUBLIC "${BQ_INCLUDE_DIR}")

BQ_SDL_INCLUDE(sdl.hpp)
BQ_SDL_SOURCE(sdl.cpp)

BQ_SDL_INCLUDE(sdl/app/ResultCallbacks.hpp)

BQ_SDL_INCLUDE(sdl/Applet.hpp)
BQ_SDL_SOURCE(sdl/Applet.cpp)

BQ_SDL_INCLUDE(sdl/Application.hpp)
BQ_SDL_SOURCE(sdl/Application.cpp)

BQ_SDL_INCLUDE(sdl/Clipboard.hpp)

BQ_SDL_INCLUDE(sdl/Color.hpp)

BQ_SDL_INCLUDE(sdl/events.hpp)
BQ_SDL_SOURCE(sdl/events.cpp)

BQ_SDL_INCLUDE(sdl/Font.hpp)
BQ_SDL_SOURCE(sdl/Font.cpp)

BQ_SDL_INCLUDE(sdl/Joystick.hpp)

BQ_SDL_INCLUDE(sdl/Log.hpp)
BQ_SDL_INCLUDE(sdl/Log.cpp)

BQ_SDL_INCLUDE(sdl/mix.hpp)

BQ_SDL_INCLUDE(sdl/PixelFormat.hpp)

BQ_SDL_INCLUDE(sdl/Properties.hpp)
BQ_SDL_SOURCE(sdl/Properties.cpp)

BQ_SDL_INCLUDE(sdl/Rect.hpp)

BQ_SDL_INCLUDE(sdl/Renderer.hpp)
BQ_SDL_SOURCE(sdl/Renderer.cpp)

BQ_SDL_INCLUDE(sdl/ResultCallbacks.hpp)

BQ_SDL_INCLUDE(sdl/Subsystem.hpp)
BQ_SDL_SOURCE(sdl/Subsystem.cpp)

BQ_SDL_INCLUDE(sdl/Surface.hpp)
BQ_SDL_SOURCE(sdl/Surface.cpp)

BQ_SDL_INCLUDE(sdl/Texture.hpp)
BQ_SDL_SOURCE(sdl/Texture.cpp)

BQ_SDL_INCLUDE(sdl/Thread.hpp)

BQ_SDL_INCLUDE(sdl/Window.hpp)
BQ_SDL_SOURCE(sdl/Window.cpp)

