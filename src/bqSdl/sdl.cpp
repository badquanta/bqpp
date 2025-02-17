/** @file sdl.cpp
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
#include <bq/sdl.hpp>

namespace bq::sdl
{

StepID_t next_app_id = 1;
StepID_t GetNewResourceID()
{
    return next_app_id++;
}

#define RETURN_IF_NOT_SDL_APP_CONTINUE(x)                                                                              \
    {                                                                                                                  \
        SDL_AppResult result = x;                                                                                      \
        if (result != SDL_APP_CONTINUE)                                                                                \
        {                                                                                                              \
            return result;                                                                                             \
        }                                                                                                              \
    }

} // namespace bq::sdl