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
 * @ingroup bqStd
 */
namespace bq
{
/**
 * @brief Base template for shared pointer resource wrapper classes.
 * @details
 * SharedResource is a base for wrappers around shared pointers to structures.
 * It provides convenient operators to interact with the underlying resource.
 *
 * @tparam ResourceType The type of the resource being managed.
 *
 * @example testSharedResource.cpp
 * @ingroup bqStd
 */
template <typename ResourceType> class SharedResource
{
  public:
    /** @brief The resource that is monitored by this chaperone class.  */
    std::shared_ptr<ResourceType> resource;
    /** @brief Ensures `SharedResource` acts like `ResourceType*` references. */
    operator ResourceType *() const
    {
        return resource.get();
    }
    /** @brief Ensures `SharedResource` acts like `ResourceType*` references. */
    ResourceType *operator->() const
    {
        return resource.get();
    }
    /** @brief Ensures `SharedResource` acts like `ResourceType*` references. */
    bool operator==(const nullptr_t) const
    {
        return resource == nullptr;
    }
    /** @brief Ensures `SharedResource` acts like `ResourceType*` references. */
    operator bool() const
    {
        return resource != nullptr;
    }
    /** @brief deleter_callback_t defines interface for deleter */
    typedef void (*deleter_callback_t)(ResourceType *);
    /** @brief Simply enforces requirement for deleter */
    SharedResource(ResourceType *t, deleter_callback_t d) : resource(t, d)
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
     * where going out of scope should not deallocate.
     * @param ignored
     **/
    static void null_deleter(ResourceType *)
    {
        return;
    }
};
} // namespace bq
