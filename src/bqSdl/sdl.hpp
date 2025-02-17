#pragma once
/** @file sdl.hpp
 * @brief ...
 * @copyright Copyright (c) 2025 BadQuanta.
 * This file is part of the bq distribution
 * (https://github.com/badquanta/bqpp.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
// #include <SDL3_ttf/SDL_ttf.h> Will be included by bq/sdl/Font.hpp
// #include <bq/sdl/Applet.hpp> Will be included by bq/sdl/Application.hpp
#include <bq/sdl/Application.hpp>
#include <bq/sdl/Clipboard.hpp>
#include <bq/sdl/Color.hpp>
#include <bq/sdl/Font.hpp>
#include <bq/sdl/Joystick.hpp>
#include <bq/sdl/Log.hpp>
#include <bq/sdl/Renderer.hpp>
#include <bq/sdl/Subsystem.hpp>
#include <bq/sdl/Surface.hpp>
#include <bq/sdl/Texture.hpp>
#include <bq/sdl/Thread.hpp>
#include <bq/sdl/Window.hpp>
#include <bq/sdl/app/ResultCallbacks.hpp>
#include <bq/sdl/events.hpp>
#include <list>
#include <unordered_set>

/**
 * This MACRO is used to define the "SDL_App..." functions.
 */
#define BQH_APP_MAIN(APP_CLASS_NAME)                                                                                   \
    SDL_AppResult SDL_AppInit(void **a_State, int a_ArgC, char *a_ArgV[])                                              \
    {                                                                                                                  \
        APP_CLASS_NAME *app = new APP_CLASS_NAME{};                                                                    \
        *a_State = app;                                                                                                \
        return app->parseArguments(a_ArgC, a_ArgV);                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    SDL_AppResult SDL_AppIterate(void *a_State)                                                                        \
    {                                                                                                                  \
        if (!a_State)                                                                                                  \
            return SDL_APP_FAILURE;                                                                                    \
        return ((APP_CLASS_NAME *)a_State)->iterate();                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    SDL_AppResult SDL_AppEvent(void *a_State, SDL_Event *a_Event)                                                      \
    {                                                                                                                  \
        if (!a_State)                                                                                                  \
            return SDL_APP_FAILURE;                                                                                    \
        return ((APP_CLASS_NAME *)a_State)->event(a_Event);                                                            \
    }                                                                                                                  \
                                                                                                                       \
    void SDL_AppQuit(void *a_State, SDL_AppResult result)                                                              \
    {                                                                                                                  \
        delete (APP_CLASS_NAME *)a_State;                                                                              \
        bq::sdl::Log::Application.debug("SDL_AppQuit result: %d, %s:%d", result, __FILE__, __LINE__);                  \
    }

namespace bq::sdl
{

} // namespace bq::sdl
