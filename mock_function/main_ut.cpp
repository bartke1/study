#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "sut.hpp"
#include "foo_library_mock.hpp"
#include "function_mocks.hpp"

DECLARE_FMOCK(foo, void, int);
//DEFINE_FMOCK(foo);

//template<typename X>
//struct foo_MOCK_SUFFIX;
//
//template<>
//struct foo_MOCK_SUFFIX<void(int)>
//{
//    foo_MOCK_SUFFIX()
//    {
//        obj = this;
//    }
//
//    virtual ~foo_MOCK_SUFFIX()
//    {
//        obj = nullptr;
//    }
//
//    MOCK_METHOD1(foo, void(int));
//
//    static foo_MOCK_SUFFIX<void(int)>* obj;
//};
//
foo_MOCK_SUFFIX<void(int)>* foo_MOCK_SUFFIX<void(int)>::obj = nullptr;

//template<>
//foo_MOCK_SUFFIX<void(float)>* foo_MOCK_SUFFIX<void(float)>::obj = nullptr;

void foo(int x)
{
    foo_MOCK_SUFFIX<void(int)>::obj->foo(x);
}

//void foo(float x)
//{
//    foo_MOCK_SUFFIX<void(float)>::obj->foo(x);
//}

//DECLARE_FMOCK(foo, void, float);
//DEFINE_FMOCK(foo);

//MOCK_METHOD1(foo, void(), dupa);

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

class SutTest : public ::testing::Test {
};



TEST_F(SutTest, Dupa)
{
    foo_MOCK_SUFFIX<void(int)> x;
//    foo_MOCK_SUFFIX<void(float)> y;
    EXPECT_CALL(x, foo(3)).Times(2);
    sut(2, 3);
//    EXPECT_CALL(y, foo(2.1f)).Times(1);
//    foo(2.1f);
}
