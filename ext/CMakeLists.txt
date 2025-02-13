# To fix build waring
set(ARG_DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
include(FetchContent)
## SDL3
option(BQ_SDL_FETCH "Fetch the SDL3 dependency via cmake" ON)
if(BQ_SDL_FETCH)
  FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG release-3.2.0
  )
  FetchContent_MakeAvailable(SDL3)
else()
  find_package(SDL3 REQUIRED)
endif()
# SDL3 image
option(BQ_SDL_IMAGE_FETCH "Fetch SDL3 Image dependency via cmake" ON)
if(BQ_SDL_IMAGE_FETCH)
  FetchContent_Declare(SDL_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
    GIT_TAG prerelease-3.1.1
  )
  FetchContent_MakeAvailable(SDL_image)
else()
  find_package(SDL_image REQUIRED)
endif()
## SDL_ttf
option(BQ_SDL_TTF_FETCH "Fetch SDL3 TTF dependency via cmake" ON)
if(BQ_SDL_TTF_FETCH)
  FetchContent_Declare(
    SDL_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
    GIT_TAG main
  )
  FetchContent_MakeAvailable(SDL_ttf)
else()
  find_package(SDL_ttf REQUIRED)
endif()
## SDL_mixer
option(BQ_SDL_MIXER_FETCH "Fetch SDL3 Mixer dependency via cmake" ON)
if(BQ_SDL_MIXER_FETCH)
  FetchContent_Declare(
    SDL_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer
    GIT_TAG main
  )
  FetchContent_MakeAvailable(SDL_mixer)
else()
  find_package(SDL_mixer REQUIRED)
endif()
