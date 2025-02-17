/** @file Window.cpp
 * @brief ...
 * @todo remove this or maybe move SharedResource to std, also maybe rename.
 * @copyright Copyright (c) 2025 BadQuanta.
 * This file is part of the bq distribution
 * (https://github.com/badquanta/bqpp).
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
#include <bq/sdl/Application.hpp>
#include <bq/sdl/Surface.hpp>
#include <bq/sdl/Window.hpp>
namespace bq::sdl
{
/**
 * @brief Title for windows if unspecified.
 */
std::string Window::DEFAULT_TITLE = "UNNAMED WINDOW";
/**
 * @brief Default width for windows if unspecified.
 */
int Window::DEFAULT_WIDTH = 640;
/**
 * @brief Default height for windows if unspecified.
 */
int Window::DEFAULT_HEIGHT = 480;
/**
 * @brief Default flags for windows if unspecified.
 */
SDL_WindowFlags Window::DEFAULT_FLAGS = 0;
/**
 */
void Window::Destroy(SDL_Window *w)
{
    if (w)
    {
        SDL_DestroyWindow(w);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }
}
// Use SDL_CreateWindowWithProperties
Window::Window(SDL_PropertiesID p) : SharedResource<SDL_Window>(nullptr, Destroy)
{
    if (not SDL_InitSubSystem(SDL_INIT_VIDEO))
    {
        // Application::Fail();
        throw(std::string(SDL_GetError()));
    }
    SDL_Window *winptr = SDL_CreateWindowWithProperties(p);
    if (not winptr)
    {
        // Application::Fail();
        throw(std::string(SDL_GetError()));
    }
    resource.reset(winptr, Destroy);
}
// Use SDL_CreateWindow
Window::Window(const char *t, int w, int h, SDL_WindowFlags f, deleter_callback_t d)
    : SharedResource<SDL_Window>(nullptr, d)
{
    if (not SDL_InitSubSystem(SDL_INIT_VIDEO))
    {
        Application::Fail();
        // throw(std::string(SDL_GetError()));
    }
    SDL_Window *winptr = SDL_CreateWindow(t, w, h, f);
    if (not winptr)
    {
        Application::Fail();
        // throw(std::string(SDL_GetError()));
    }
    resource.reset(winptr, d);
}

} // namespace bq::sdl
