/** @file testSharedResource.hpp
 * @brief Unit test for SharedResource template class.
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
#include <bq/SharedResource.hpp>
#include <gtest/gtest.h>
// It is apparently required to declare a concrete class that inherits from
// SharedResource in order to access `null_deleter` static member function.
class MyInt : public bq::SharedResource<int>
{
  public:
    // Inherit constructors from SharedResource
    using SharedResource::null_deleter;
    using SharedResource::SharedResource;
};
//
TEST(testSharedResource, null_deleter)
{
    int myInt{6};
    MyInt a(&myInt, MyInt::null_deleter);
    EXPECT_TRUE(a);
    EXPECT_EQ(a, &myInt);
    EXPECT_EQ(*a, 6);
}
//
TEST(testSharedResource, default_constructor)
{
    MyInt a;
    EXPECT_EQ(a, nullptr);
    EXPECT_FALSE(a);
}
//
TEST(testSharedResource, explicit_nullptr)
{
    MyInt a(nullptr);
    EXPECT_EQ(a, nullptr);
    EXPECT_FALSE(a);
}
//
struct SomeStruct
{
    int a{0};
    float b{0.0f};
    const char *s{nullptr};
};
// Test wrapping a more-complex struct (so that we can test the `->` operator).
class WrapSomeStruct : public bq::SharedResource<SomeStruct>
{
  public:
    using SharedResource::SharedResource;
};
// Basically just to test the `->` operator.
TEST(testSharedResource, someStruct_abc)
{
    SomeStruct ss{1, 2.0f, "three"};
    WrapSomeStruct wss(&ss, WrapSomeStruct::null_deleter);
    EXPECT_TRUE(wss);
    EXPECT_EQ(wss->a, 1);
    EXPECT_EQ(wss->b, 2.0f);
    EXPECT_STREQ(wss->s, "three");
}