#include <bq/SharedResource.hpp>
#include <gtest/gtest.h>

using SharedInt = bq::SharedResource<int>;

class MyInt : public bq::SharedResource<int>
{
  public:
    using SharedResource::SharedResource;
};

TEST(testSharedResource, basic)
{
    int myInt{6};
    MyInt a(&myInt, MyInt::null_deleter);
}