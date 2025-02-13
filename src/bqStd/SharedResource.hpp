#pragma once
/** @file SharedResource.hpp
 * @brief Base template for shared pointer resource wrapper classes.
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
#include <memory>
/**
 * - SharedResource.hpp - SharedResource is a base for wrappers around shared
 * pointers to SDL structures.
 */
namespace bq
{
/**
 * @brief base class for helper classes that wrap shared pointers to SDL
 * structures.
 * @note SharedResource does __not__ use virtual methods.
 * @note default constructor == nullptr reference
 * @note automatically casts to typename T*
 * @note automatically casts to bool,
 * - true == (this.resource.git()!=nullptr);
 * - false == (this.resource.get()==nullptr);
 */
template <typename T> class SharedResource
{
  public:
    /** @brief resource shared pointer.  */
    std::shared_ptr<T> resource;
    /** @brief SharedResources act like pointers to that resource. */
    operator T *() const
    {
        return resource.get();
    }
    /** @brief SharedResources act like pointers to that resource. */
    T *operator->() const
    {
        return resource.get();
    }
    /** @brief SharedResources can be compared to nullptr. */
    bool operator==(const nullptr_t) const
    {
        return resource == nullptr;
    }
    /** @brief SharedResources act as a boolean in if statements */
    operator bool() const
    {
        return resource != nullptr;
    }
    // using std::shared_ptr<T>::shared_ptr;
    /** @brief deleter_callback_t a function pointer type for deleting the
     * resource. */
    typedef void (*deleter_callback_t)(T *);
    /** @brief Requires that a deleter be specified */
    SharedResource(T *t, deleter_callback_t d) : resource(t, d)
    {
    }
    /** @brief Explicitly setting the SharedResource to nullptr is allowed */
    SharedResource(nullptr_t) : resource(nullptr)
    {
    }
    /** @brief The default constructor is identical to constructor the
     * SharedResource with nullptr */
    SharedResource() : resource(nullptr)
    {
    }
    /** @brief No-op (do nothing) deleter. Useful for non-managing instances
     * where going out of scope should not deallocate. */
    static void null_deleter(T *t)
    {
        return;
    }
};
} // namespace bq
